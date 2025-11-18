#ifndef TCAN4550_H_
#define TCAN4550_H_

#include <Arduino.h>
#include "TCAN4x5x_Reg.h"
#include "TCAN4x5x_Data_Structs.h"

#define TCAN4550_DEVICE_VERIFY_CONFIGURATION_WRITES

typedef enum { TEST_MODE_NORMAL, TEST_MODE_PHY, TEST_MODE_CONTROLLER } Test_Mode;

typedef enum { 
    MODE_NORMAL     = REG_BITS_DEVICE_MODE_DEVICEMODE_NORMAL, 
    MODE_STANDBY    = REG_BITS_DEVICE_MODE_DEVICEMODE_STANDBY, 
    MODE_SLEEP      = REG_BITS_DEVICE_MODE_DEVICEMODE_SLEEP
} Device_Mode;

typedef enum { 
    WDT_60MS    = REG_BITS_DEVICE_MODE_WD_TIMER_60MS,
    WDT_600MS   = REG_BITS_DEVICE_MODE_WD_TIMER_600MS, 
    WDT_3S      = REG_BITS_DEVICE_MODE_WD_TIMER_3S, 
    WDT_6S      = REG_BITS_DEVICE_MODE_WD_TIMER_6S
} WDT_Timeout_Value;

typedef enum { 
    ACTION_INTERRUPT            = REG_BITS_DEVICE_MODE_WDT_ACTION_INT,
    ACTION_INTERRUPT_INH_PULSE  = REG_BITS_DEVICE_MODE_WDT_ACTION_INH_PULSE,
    ACTION_WDT_PULSE            = REG_BITS_DEVICE_MODE_WDT_ACTION_WDT_PULSE
} WDT_Timout_Action;

typedef enum { 
    CLK_40MHZ   = REG_BITS_DEVICE_MODE_WD_CLK_40MHZ, 
    CLK_20MHZ   = REG_BITS_DEVICE_MODE_WD_CLK_20MHZ
} WDT_Clock_Ref;


// ~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*
//                            Non-MCAN Device Functions
// ~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*
extern bool Device_Configure(TCAN4x5x_DEV_CONFIG *deviceConfig);
extern TCAN4x5x_DEV_CONFIG Device_ReadConfig();
extern bool Device_SetMode(Device_Mode mode);
extern bool Device_EnableTestMode(Test_Mode mode);
extern bool Device_DisableTestMode();
extern Device_Mode Read_DeviceMode();

extern bool WDT_Configure(WDT_Timeout_Value timeout_value, WDT_Timout_Action timeout_action, WDT_Clock_Ref);
extern bool WDT_Enable();
extern bool WDT_Disable();
extern bool WDT_Reset();

#endif