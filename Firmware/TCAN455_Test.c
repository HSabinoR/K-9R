#include <SPI.h>
#include "TCAN4550_SPI.h"
#include "TCAN4x5x_Reg.h"

void setup() {

    tcan4550_SPI_begin();

    tcan4550_modify(REG_DEV_MODES_AND_PINS, REG_BITS_DEVICE_MODE_DEVICEMODE_MASK, REG_BITS_DEVICE_MODE_DEVICEMODE_STANDBY) // Sets the TCAN4550 to standby mode
    
    /// WatchDog Timer Configuration ///
    tcan4550_modify(REG_DEV_MODES_AND_PINS, REG_BITS_DEVICE_MODE_WDT_MASK, REG_BITS_DEVICE_MODE_WDT_EN); // Enables the WatchDog Timer
    tcan4550_modify(REG_DEV_MODES_AND_PINS, REG_BITS_DEVICE_MODE_WD_TIMER_MASK,REG_BITS_DEVICE_MODE_WD_TIMER_600MS) // Sets the WD timeout value to 600ms
    tcan4550_modify() // Sets the WD Clock Reference to 40MHz
    tcan4550_modify() // Sets the WD timeout action

    
}