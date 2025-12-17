#ifndef GPIO_H
#define GPIO_H

#include "commonDataTypes.h"
#include <stdio.h>

// GPIO Matrix Function Values
#define I2CEXT0_SCL 89
#define I2CEXT0_SDA 90
#define I2CEXT1_SCL 91
#define I2CEXT1_SDA 92

// GPIO Base Address
#define GPIO_Base_Address 0x60004000UL

// IO Mux Base Address
#define IO_MUX_Base_Address 0x60009000UL

// GPIO Pin Input Function Selection Base
#define GPIO_FUNC_Y_IN_SEL_CFG_REG_Base_Address (0x0154UL + GPIO_Base_Address) //0x0154

// GPIO Pin Output Function Selection Base
#define GPIO_FUNC_X_OUT_SELF_CFG_REG_Base_Address (0x0554UL + GPIO_Base_Address)

// GPIO Config Register Base
#define GPIO_PIN_N_REG_Base_Address (0x0074UL + GPIO_Base_Address)

// GPIO IO Mux Reg
#define IO_MUX_N_REG_Base_Address (0x0004UL + IO_MUX_Base_Address)

// GPIO Clock Config Reg
#define GPIO_CLOCK_GATE_REG_Base_Address (0x062CUL + GPIO_Base_Address)

// GPIO Enable Reg Pin 0 -> 31
#define GPIO_ENABLE_W1TS_REG_Base_Address (0x0024UL + GPIO_Base_Address) // 24

// GPIO Disable Reg 0 -> 31
#define GPIO_ENABLE_W1TC_REG_Base_Address (0x0028UL + GPIO_Base_Address)

// GPIO Enable Reg Pin 32 -> 48
#define GPIO_ENABLE_W2TS_REG_Base_Address (0x002CUL + GPIO_Base_Address)

// GPIO Disable Reg Pin 32 -> 48
#define GPIO_ENABLE_W2TC_REG_Base_Address (0x0030UL + GPIO_Base_Address)

typedef enum
{
    NO_SYNC,
    FALLING_EDGE_SYNC,
    RISING_EDGE_SYNC
}gpioApbSyncType;

typedef enum
{
    DISABLED,
    RISING_EDGE_TRIGGER,
    FALLING_EDGE_TRIGGER,
    ANY_EDGE_TRIGGER,
    LOW_LEVEL_TRIGGER,
    HIGH_LEVEL_TRIGGER
}gpioInterruptType;

typedef enum
{
    HONESTLY_IDK,
    GPIO_MATRIX,
    DIRECT_PERIPHERAL
}ioMuxMcuSel;

typedef struct 
{
    __uint8_t functionNum;
    bool_t invertOutput;
    bool_t useGpioOutputEnable;
    bool_t invertOutputEnable;
}gpioOutputConfig_t;

typedef struct
{
    gpioApbSyncType phase2SyncGpioInputWithApb;
    bool_t useOpenDrainOutput;
    gpioApbSyncType phase1SyncGpioInputWithApb;
    gpioInterruptType gpioInterruptType;
    bool_t gpioWakeupCpuEnable;
    bool_t gpioCpuInterruptEnabe;
    bool_t gpioCpuNonMaskInterruptEnable;
}gpioPinConfig_t;

typedef struct
{
    bool_t sleepOutputEnable;
    bool_t sleepModeEnable;
    bool_t sleepWeakPullDownEnable;
    bool_t sleepWeakPullUpEnable;
    bool_t sleepInputEnable;
    __uint8_t sleepGpioDriveStrength;
    bool_t weakPullDownEnable;
    bool_t weakPullUpEnable;
    bool_t inputEnable;
    __uint8_t gpioDriveStrength;
    ioMuxMcuSel mcuSel;
    bool_t inputFilterEnable;
}ioMuxConfig_t;


void gpioSetInputPin(__uint32_t pinNum,
                        __uint32_t functionNum,
                        __uint8_t useGpioMatrix,
                        __uint8_t invertInput);

void gpioSetOutputPin(__uint32_t pinNum,
                        gpioOutputConfig_t gpioOutputConfig);

void gpioPinSettings(__uint32_t pinNum,
                        gpioPinConfig_t gpioPinConfig);


void gpioIoMuxCfg(__uint32_t pinNum,
                    ioMuxConfig_t ioMuxConfig);

void gpioClkEnble();

void gpioClkDisable();

void gpioOutputEnable(__uint32_t pinNum);

void gpioOutputDisable(__uint32_t pinNum);

#endif // GPIO_H

