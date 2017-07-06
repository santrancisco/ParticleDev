This is a juicy script to find I2C bus address for devices (eg: LCD, etc..) and print it out to USB Serial. 

For Arduino: replace `#include "application.h"` with `#include <Wire.h>`

using our POCKET Chip we can run `i2cdetect -y 2` to find the address for I2C bus device