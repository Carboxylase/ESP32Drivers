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
                        gpioOutputConfig_t gpioOutputConfig)
{
    volatile __uint32_t *gpioFuncOutSelCfgReg = (volatile __uint32_t*)(GPIO_FUNC_X_OUT_SELF_CFG_REG_Base_Address + 0x4UL*pinNum);

    if (pinNum > 48)
    {
        return;
    }

    *gpioFuncOutSelCfgReg &= 0;

    *gpioFuncOutSelCfgReg |= gpioOutputConfig.functionNum << 0;

    *gpioFuncOutSelCfgReg |= gpioOutputConfig.invertOutput << 9;

    *gpioFuncOutSelCfgReg |= gpioOutputConfig.useGpioOutputEnable << 10;

    *gpioFuncOutSelCfgReg |= gpioOutputConfig.invertOutputEnable << 11;
}

void gpioPinSettings(__uint32_t pinNum,
                        gpioPinConfig_t gpioPinConfig)
{
    if (pinNum > 48)
    {
        return;
    }

    volatile __uint32_t *gpioSettingsReg = (volatile __uint32_t*)(GPIO_PIN_N_REG_Base_Address + 0x4U*pinNum);

    // reset configs
    *gpioSettingsReg &= 0;

    // 1 = synchronize input with the APB clk
    *gpioSettingsReg |= gpioPinConfig.phase2SyncGpioInputWithApb << 0;

    // 1 = use open drain, 0 = use normal
    *gpioSettingsReg |= gpioPinConfig.useOpenDrainOutput << 2;

    // 1 = synchonize with peripheral clk
    *gpioSettingsReg |= gpioPinConfig.phase1SyncGpioInputWithApb << 3;

    // 0 = no interrupt, 1 = rising edge, 2 = falling edge, 3 = any edge, 4 = low level trigger, 5 = high level trigger
    *gpioSettingsReg |= gpioPinConfig.gpioInterruptType << 7;

    // 1 = wakes up CPU from light-sleep
    *gpioSettingsReg |= gpioPinConfig.gpioWakeupCpuEnable << 10;

    // 1 = cpu interrupt enabled
    *gpioSettingsReg |= gpioPinConfig.gpioCpuInterruptEnabe << 13;

    // 1 = cpu non-maskable interrupt enabled
    *gpioSettingsReg |= gpioPinConfig.gpioCpuNonMaskInterruptEnable << 14;

}


// bruh I don't even care no more -- complete this function later
void gpioIoMuxCfg(__uint32_t pinNum,
                    ioMuxConfig_t ioMuxConfig)
{
    if (ioMuxConfig.sleepGpioDriveStrength > 3)
    {
        return;
    }
    if (ioMuxConfig.gpioDriveStrength > 3)
    {
        return;
    }
    if (pinNum > 21 && pinNum < 26)
    {
        return;
    }
    
    volatile __uint32_t *gpioIoMuxCfgReg = (volatile __uint32_t*)(IO_MUX_N_REG_Base_Address + 0x4U*pinNum);

    *gpioIoMuxCfgReg &= 0;

    *gpioIoMuxCfgReg |= ioMuxConfig.sleepOutputEnable << 0;

    *gpioIoMuxCfgReg |= ioMuxConfig.sleepModeEnable << 1;

    *gpioIoMuxCfgReg |= ioMuxConfig.sleepWeakPullDownEnable << 2;

    *gpioIoMuxCfgReg |= ioMuxConfig.sleepWeakPullDownEnable << 3;

    *gpioIoMuxCfgReg |= ioMuxConfig.sleepInputEnable << 4;

    *gpioIoMuxCfgReg |= ioMuxConfig.sleepGpioDriveStrength << 5;

    *gpioIoMuxCfgReg |= ioMuxConfig.weakPullDownEnable << 7;

    *gpioIoMuxCfgReg |= ioMuxConfig.weakPullUpEnable << 8;

    *gpioIoMuxCfgReg |= ioMuxConfig.inputEnable << 9;

    *gpioIoMuxCfgReg |= ioMuxConfig.gpioDriveStrength << 10;

    *gpioIoMuxCfgReg |= ioMuxConfig.mcuSel << 12;

    *gpioIoMuxCfgReg |= ioMuxConfig.inputFilterEnable << 15;
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

    volatile __uint32_t temp;

    if ( pinNum <= 31)
    {
        temp = 0x1UL << pinNum;
    }
    else
    {
        temp = 0x1UL << (pinNum - 31);
    }

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

