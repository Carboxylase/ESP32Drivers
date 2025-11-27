#include "include/gpio.h"

void gpioSetInputPin(__uint32_t pinNum,
                        __uint32_t functionNum,
                        __uint8_t useGpioMatrix,
                        __uint8_t invertInput)
{
    volatile __uint32_t *gpioFuncInSelCfgReg = (volatile __uint32_t*)(GPIO_FUNC_Y_IN_SEL_CFG_REG_Base_Address + 0x4UL*functionNum);

    *gpioFuncInSelCfgReg &= 0;

    if (useGpioMatrix > 1)
    {
        return;
    }
    if (invertInput > 1)
    {
        return;
    }

    __uint32_t val = 0;

    // val |= (pinNum & 0x3F);

    // val |= (invertInput & 1) << 6;

    // val |= (useGpioMatrix & 1) << 7;


    // val |= useGpioMatrix << 7;
    // val |= invertInput << 6;
    // val |= pinNum << 0;

    *gpioFuncInSelCfgReg |= useGpioMatrix << 7;
    *gpioFuncInSelCfgReg |= invertInput << 6;
    *gpioFuncInSelCfgReg |= pinNum << 0;

    // *gpioFuncInSelCfgReg = val;

}

void gpioSetOutputPin(__uint32_t pinNum,
                        __uint32_t functionNum,
                        __uint8_t invertOutput,
                        __uint8_t useGpioOutputEnable,
                        __uint8_t invertOutputEnable)
{
    volatile __uint32_t *gpioFuncOutSelCfgReg = (volatile __uint32_t*)(GPIO_FUNC_X_OUT_SELF_CFG_REG_Base_Address + 0x4UL*pinNum);

    if (pinNum > 48)
    {
        return;
    }
    if (invertOutput > 1)
    {
        return;
    }
    if (useGpioOutputEnable > 1)
    {
        return;
    }
    if (invertOutputEnable > 1)
    {
        return;
    }

    *gpioFuncOutSelCfgReg &= 0;

    *gpioFuncOutSelCfgReg |= functionNum << 0;

    *gpioFuncOutSelCfgReg |= invertOutput << 9;

    *gpioFuncOutSelCfgReg |= useGpioOutputEnable << 10;

    *gpioFuncOutSelCfgReg |= invertOutputEnable << 11;
}

void gpioPinSettings(__uint32_t pinNum,
                        __uint8_t syncPeriClk,
                        __uint8_t syncBusClk,
                        __uint8_t useOpenDrainOutput,
                        __uint8_t interruptType,
                        __uint8_t pinWakeupEnable,
                        __uint8_t cpuInterruptEnable,
                        __uint8_t nonMaskInterruptEnable)
{
    if (syncPeriClk > 3)
    {
        return;
    }
    if (syncBusClk > 3)
    {
        return;
    }
    if (useOpenDrainOutput > 1)
    {
        return;
    }
    if (interruptType > 5)
    {
        return;
    }
    if (pinWakeupEnable > 1)
    {
        return;
    }
    if (cpuInterruptEnable > 1)
    {
        return;
    }
    if (nonMaskInterruptEnable > 1)
    {
        return;
    }

    volatile __uint32_t *gpioSettingsReg = (volatile __uint32_t*)(GPIO_PIN_N_REG_Base_Address + 0x4U*pinNum);

    // reset configs
    *gpioSettingsReg &= 0;

    // 1 = synchronize input with the APB clk
    *gpioSettingsReg |= syncBusClk;

    // 1 = use open drain, 0 = use normal
    *gpioSettingsReg |= useOpenDrainOutput << 2;

    // 1 = synchonize with peripheral clk
    *gpioSettingsReg |= syncPeriClk << 3;

    // 0 = no interrupt, 1 = rising edge, 2 = falling edge, 3 = any edge, 4 = low level trigger, 5 = high level trigger
    *gpioSettingsReg |= interruptType << 7;

    // 1 = wakes up CPU from light-sleep
    *gpioSettingsReg |= pinWakeupEnable << 8;

    // 1 = cpu interrupt enabled
    *gpioSettingsReg |= cpuInterruptEnable << 13;

    // 1 = cpu non-maskable interrupt enabled
    *gpioSettingsReg |= nonMaskInterruptEnable << 14;

}


// bruh I don't even care no more -- complete this function later
void gpioIoMuxCfg(__uint32_t pinNum,
                    __uint8_t pullDownEnable,
                    __uint8_t pullUpEnable,
                    __uint8_t inputEnable,
                    __uint8_t driveStrength,
                    __uint8_t mcuSel,
                    __uint8_t filterEnable)
{
    if (pullDownEnable > 1)
    {
        return;
    }
    if (pullUpEnable > 1)
    {
        return;
    }
    if (inputEnable > 1)
    {
        return;
    }
    if (driveStrength > 3)
    {
        return;
    }
    if (mcuSel > 7)
    {
        return;
    }
    if (filterEnable  > 1)
    {
        return;
    }

    if (pinNum > 21 && pinNum < 26)
    {
        return;
    }
    volatile __uint32_t *gpioIoMuxCfgReg = (volatile __uint32_t*)(GPIO_IO_MUX_N_REG_Base_Address + 0x4U*pinNum);

    *gpioIoMuxCfgReg &= 0;

    *gpioIoMuxCfgReg |= pullDownEnable << 7;

    *gpioIoMuxCfgReg |= pullUpEnable << 8;

    *gpioIoMuxCfgReg |= inputEnable << 9;

    *gpioIoMuxCfgReg |= driveStrength << 10;

    *gpioIoMuxCfgReg |= mcuSel << 12;

    *gpioIoMuxCfgReg |= filterEnable << 15;
}

void gpioClkEnble()
{
    volatile __uint32_t *gpioClkReg = (volatile __uint32_t*)GPIO_CLOCK_GATE_REG_Base_Address; 
    *gpioClkReg |= 1 << 0;
}

void gpioClkDisable()
{
    volatile __uint32_t *gpioClkReg = (volatile __uint32_t*)GPIO_CLOCK_GATE_REG_Base_Address;
    *gpioClkReg &= 0 << 0;
}

void gpioOutputEnable(__uint32_t pinNum)
{
    if (pinNum > 48)
    {
        return;
    }

    volatile __uint32_t *gpioEnableReg = NULL;

    __uint32_t temp = 0x1UL << pinNum;

    if (pinNum <= 31)
    {
        gpioEnableReg = (volatile __uint32_t*)GPIO_ENABLE_W1TS_REG_Base_Address;
    }
    else
    {
        gpioEnableReg = (volatile __uint32_t*)GPIO_ENABLE_W2TS_REG_Base_Address;
    }

    *gpioEnableReg = temp;
}

void gpioOutputDisable(__uint32_t pinNum)
{
    if (pinNum > 48)
    {
        return;
    }

    volatile __uint32_t *gpioDisableReg;

    if (pinNum <= 31)
    {
        gpioDisableReg = (volatile __uint32_t*)GPIO_ENABLE_W1TC_REG_Base_Address;
    }
    else
    {
        gpioDisableReg = (volatile __uint32_t*)GPIO_ENABLE_W2TC_REG_Base_Address;
    }

    *gpioDisableReg |= 0x0UL << pinNum;
}

