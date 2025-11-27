#ifndef GPIO_H
#define GPIO_H

#include <stdio.h>


// GPIO Matrix Function Values
#define I2CEXT0_SCL 89
#define I2CEXT0_SDA 90
#define I2CEXT1_SCL 91
#define I2CEXT1_SDA 92

// GPIO Base Address
#define GPIO_Base_Address 0x60004000UL

// GPIO Pin Input Function Selection Base
#define GPIO_FUNC_Y_IN_SEL_CFG_REG_Base_Address (0x0154UL + GPIO_Base_Address) //0x0154

// GPIO Pin Output Function Selection Base
#define GPIO_FUNC_X_OUT_SELF_CFG_REG_Base_Address (0x0554UL + GPIO_Base_Address)

// GPIO Config Register Base
#define GPIO_PIN_N_REG_Base_Address (0x0074UL + GPIO_Base_Address)

// GPIO IO Mux Reg
#define GPIO_IO_MUX_N_REG_Base_Address (0x0010UL + GPIO_Base_Address)

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

void gpioSetInputPin(__uint32_t pinNum,
                        __uint32_t functionNum,
                        __uint8_t useGpioMatrix,
                        __uint8_t invertInput);

void gpioSetOutputPin(__uint32_t pinNum,
                        __uint32_t functionNum,
                        __uint8_t invertOutput,
                        __uint8_t useGpioOutputEnable,
                        __uint8_t invertOutputEnable);

void gpioPinSettings(__uint32_t pinNum,
                        __uint8_t syncPeriClk,
                        __uint8_t syncBusClk,
                        __uint8_t useOpenDrainOutput,
                        __uint8_t interruptType,
                        __uint8_t pinWakeupEnable,
                        __uint8_t cpuInterruptEnable,
                        __uint8_t nonMaskInterruptEnable);


void gpioIoMuxCfg(__uint32_t pinNum,
                    __uint8_t pullDownEnable,
                    __uint8_t pullUpEnable,
                    __uint8_t inputEnable,
                    __uint8_t driveStrength,
                    __uint8_t mcuSel,
                    __uint8_t filterEnable);

void gpioClkEnble();

void gpioClkDisable();

void gpioOutputEnable(__uint32_t pinNum);

void gpioOutputDisable(__uint32_t pinNum);

#endif // GPIO_H

