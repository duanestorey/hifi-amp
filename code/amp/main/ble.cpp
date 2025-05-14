#include "ble.h"

static const char* TAG = "BLEDiagnostics";

BLEDiagnostics::BLEDiagnostics(const std::string& deviceName)
    : _deviceName(deviceName) {}

void BLEDiagnostics::start() {
    initBLE();

    // Register static GATT services
    ble_svc_gap_init();
    ble_svc_gatt_init();
    ble_svc_gap_device_name_set(_deviceName.c_str());

    nimble_port_freertos_init(host_task);
}

void BLEDiagnostics::initBLE() {
    ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT));
    ESP_ERROR_CHECK(esp_nimble_hci_init());
    nimble_port_init();
}

void BLEDiagnostics::host_task(void *param) {
    nimble_port_run();  // Blocks forever
    nimble_port_freertos_deinit();
}

std::string BLEDiagnostics::variantToString(const DiagnosticValue& value) {
    return std::visit([](auto&& arg) -> std::string {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, float>) return std::to_string(arg);
        else if constexpr (std::is_same_v<T, int>) return std::to_string(arg);
        else return arg;
    }, value);
}

int BLEDiagnostics::gatt_access_cb(uint16_t conn_handle, uint16_t attr_handle,
                                   struct ble_gatt_access_ctxt *ctxt, void *arg) {
    auto* self = static_cast<BLEDiagnostics*>(arg);
    for (const auto& [name, handle] : self->_charHandles) {
        if (handle == attr_handle) {
            std::string val = variantToString(self->_parameters[name]);
            os_mbuf_append(ctxt->om, val.data(), val.size());
            return 0;
        }
    }
    return BLE_ATT_ERR_UNLIKELY;
}

void BLEDiagnostics::updateParameter(const std::string& name, DiagnosticValue value) {
    _parameters[name] = value;

    if (_charHandles.count(name)) {
        notify(name, value);
        return;
    }

    // Register a new characteristic
    ble_uuid128_t char_uuid = BLE_UUID128_INIT(
        0xA0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        static_cast<uint8_t>(_charHandles.size() >> 8),
        static_cast<uint8_t>(_charHandles.size() & 0xFF),
        0x00, 0x01
    );

    ble_gatt_chr_def chr = {};
    chr.uuid = (ble_uuid_t*)&char_uuid;
    chr.access_cb = gatt_access_cb;
    chr.arg = this;
    chr.flags = BLE_GATT_CHR_F_READ | BLE_GATT_CHR_F_NOTIFY;

    static ble_gatt_chr_def chrs[2];
    chrs[0] = chr;
    chrs[1] = {};  // null terminator

    static ble_uuid128_t svc_uuid = BLE_UUID128_INIT(
        0x12, 0x34, 0x56, 0x78, 0x90, 0xab,
        0xcd, 0xef, 0xfe, 0xdc, 0xba, 0x09,
        0x87, 0x65, 0x43, 0x21
    );

    ble_gatt_svc_def svc = {};
    svc.type = BLE_GATT_SVC_TYPE_PRIMARY;
    svc.uuid = (ble_uuid_t*)&svc_uuid;
    svc.characteristics = chrs;

    ESP_ERROR_CHECK(ble_gatts_count_cfg(&svc));
    ESP_ERROR_CHECK(ble_gatts_add_svcs(&svc));
    ESP_ERROR_CHECK(ble_gatts_start());

    uint16_t val_handle = 0;
    chrs[0].val_handle = &val_handle;

    ESP_ERROR_CHECK(ble_gatts_count_cfg(&svc));
    ESP_ERROR_CHECK(ble_gatts_add_svcs(&svc));
    ESP_ERROR_CHECK(ble_gatts_start());

    _charHandles[name] = val_handle;
    ESP_LOGI(TAG, "Registered BLE characteristic for '%s'", name.c_str());

    notify(name, value);
}

void BLEDiagnostics::notify(const std::string& name, const DiagnosticValue& value) {
    auto it = _charHandles.find(name);
    if (it == _charHandles.end()) return;

    std::string valStr = variantToString(value);
    struct os_mbuf* om = ble_hs_mbuf_from_flat(valStr.data(), valStr.size());
    ble_gatts_notify_custom(BLE_HS_CONN_HANDLE_NONE, it->second, om);
}