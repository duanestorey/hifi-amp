## Design Guides

The following are the primary design principles for the amplifier that will be applied to all new boards and future revisions.

### PCBs

Whenever possible, PCBs should be 55mm x 95mm so that they can be stackable. Final PCBs can be any color, but by default matte black should be used.  Each board should have an identifier and a version number, placed on the top silkscreen layer in the bold Avenir Black font using a size of 1.5mm x 1.5mm.

### SMD

The majority of resistors used in the proejct are 0603 sized, unless larger due to power requirements. Resistors directly in the audio path are typically thin-film resistors.

Capacitors are usually 0805, as is the case for 0.1uF decoupling capacitors, and 1206 for 1uf - 10uf bulk capacitors. All decoupling and bulk capacitors are specified with a X7R dielectric.  Any SMD capacitors directly in the audio path use a NP0/C0G dielectric, sized based on availability. 

### LED lights & Current Limiting Resistors

The following voltages and LED colours should be used for consistency. In general, all LEDs are set at 2ma of current and should be 0603 size.

| Voltage | Type | Color | Resistor |
| ------- | ---- | ----- | -------- | 
| 3.3 | Digital | Blue | 150R |
| 5.0 | Digital | Red | 1.3K |
| 3.3 | Analog | Yellow | 620R |
| 9.5 | Analog | Yellow | 3.6K |
| -9.5 | Analog | Blue | 3.6K |
| 34V | Analog | Yellow |16K |
| -34V | Analog | Blue | 16K |

[Return to main page](/)

