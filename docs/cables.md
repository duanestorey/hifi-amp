## Cables & Connectors

The following are the cables, connectors, and interconnects used with this project.

### JST-PH (2.00 Pitch)

JST-PH connectors are used for most small signal and low power signals. These connectors have a 2.0mm pitch and come in various sizes.  The maximum amount of current per pin is limited to 2A.

### JST-XH (2.54 Pitch)

JST-XH connectors are used for the all the digital audio inputs, such as SPDIF. These connectors have a 2.54mm pitch and come in various sizes.  The maximium amount of current per pin is limited to 3A.

### JST-VH (3.96mm Pitch)

JST-VH connectors are used for the all the high current power lines, as well as a few important grounds. These connectors have a 3.96m pitch and come in various sizes. The maximium amount of current per pin is limited to 10A.

### RCA Inputs and Outputs

The analog and SPDIF inputs into the amplifier use a standard RCA plug which has a shield and a hot wire. 

### XLR Audio Connector

Several XLR audio connectors are used in the project.  

#### 7-Pin Connector

A 7-pin connector is used between the amplifier and each monoblock, and has the following pin designation.

| Pin # | Description |
| ----- | ----------- |
| SH | Shield |
| 1 | Signal Ground |
| 2 | Woofer Signal |
| 3 | Tweeter Signal |
| 4 | SDA |
| 5 | SDL |
| 6 | Enable |
| 7 | 12V |


To prevent ground loops, the shield and power ground should be connected together at the amplifier end only. The shield pin should be left floating in the monoblock.

#### 5-Pin connector

A 5-pin connector is also used between the amplifier and each monoblock, and has the following pin designation.

| Pin # | Description |
| ----- | ----------- |
| SH | Shield |
| 1 | Power Ground |
| 2 | -POWER (-32V) |
| 3 | +POWER (32V) |
| 4 | 3.3V |
| 5 | Digital Ground |

To prevent ground loops, the shield and signal ground should be connected together at the amplifier end only.  The shield pin should be left floating in the monoblock.

The monoblock enable pin is an active HIGH signal that tells the monoblocks to exit standby mode and get ready to amplify sound.  In the case of the LM3886 amplifiers, it causes the -0.5ma current to be drawn from the LM3886 MUTE pin, which enables the output circuitry.

[Return to main page](/)

