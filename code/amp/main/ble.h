#pragma once

#include <string>
#include <map>
#include <variant>

extern "C" {
#include "esp_log.h"
#include "esp_bt.h"
#include "esp_nimble_hci.h"
#include "nimble/nimble_port.h"
#include "nimble/nimble_port_freertos.h"
#include "host/ble_hs.h"
#include "services/gap/ble_svc_gap.h"
#include "services/gatt/ble_svc_gatt.h"
}

class BLEDiagnostics {
public:
    using DiagnosticValue = std::variant<float, int, std::string>;

    explicit BLEDiagnostics(const std::string& deviceName);
    void start();
    void updateParameter(const std::string& name, DiagnosticValue value);

private:
    std::string _deviceName;
    std::map<std::string, DiagnosticValue> _parameters;
    std::map<std::string, uint16_t> _charHandles;

    static void host_task(void *param);
    static int gatt_access_cb(uint16_t conn_handle, uint16_t attr_handle,
                              struct ble_gatt_access_ctxt *ctxt, void *arg);
    static std::string variantToString(const DiagnosticValue& value);

    void initBLE();
    void notify(const std::string& name, const DiagnosticValue& value);
};