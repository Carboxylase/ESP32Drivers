#include "include/mpu6050.h"
#include "include/gpio.h"
#include "include/i2c.h"


void init6050Module(mpu6050Controller *mpu6050Ctrl)
{
    //---Setup for SDA---//

    // configure GPIO settings for sda (gpioPinSettings)
    __uint8_t syncPeriClk = 1;
    __uint8_t syncBusClk = 1;
    __uint8_t useOpenDrainOutput = 1;
    __uint8_t interruptType = 0;
    __uint8_t pinWakeupEnable = 0;
    __uint8_t cpuInterruptEnable = 0;
    __uint8_t nonMaskInterruptEnable = 0;

    gpioPinSettings(mpu6050Ctrl->sdaPinNum,
                    syncPeriClk,
                    syncBusClk,
                    useOpenDrainOutput,
                    interruptType,
                    pinWakeupEnable,
                    cpuInterruptEnable,
                    nonMaskInterruptEnable);

    // connect the GPIO (sda) pin to the I2C peripheral (gpioIoMuxCfg)
    
    //---Setup for SCL---//

    // set scl GPIO pin to be output, since we scl always comes from master (gpioSetOutputPin)
    __uint32_t functionNum = I2CEXT0_SCL;
    __uint8_t invertOutput = 0;
    __uint8_t usePeriOutputEnable = 1;
    __uint8_t invertOutputEnable = 0;

    gpioSetOutputPin(mpu6050Ctrl->sclPinNum,
                        functionNum,
                        invertOutput,
                        usePeriOutputEnable,
                        invertOutputEnable);

    // enable output for the scl GPIO pin
    gpioOutputEnable(mpu6050Ctrl->sclPinNum);

    // configure GPIO settings for scl (gpioPinSettings)
    gpioPinSettings(mpu6050Ctrl->sclPinNum,
                    syncPeriClk,
                    syncBusClk,
                    useOpenDrainOutput,
                    interruptType,
                    pinWakeupEnable,
                    cpuInterruptEnable,
                    nonMaskInterruptEnable);

    // enable output via GPIO matrix (gpioIoMuxCfg)
    __uint8_t pullDownEnableScl = 1;
    __uint8_t pullUpEnableScl = 0;
    __uint8_t inputEnableScl = 0;
    __uint8_t driveStrengthScl = 1;
    __uint8_t mcuSelScl = 1;
    __uint8_t filterEnableScl = 1;
    
    gpioIoMuxCfg(mpu6050Ctrl->sclPinNum,
                    pullDownEnableScl,
                    pullUpEnableScl,
                    inputEnableScl,
                    driveStrengthScl,
                    mcuSelScl,
                    filterEnableScl);

    // enable GPIO clk (gpioClkEnable)
    gpioClkEnble();

    //---Setup I2C---//
    
    // General I2C setup
    __uint8_t sdaSampleLevel = 1;
    __uint8_t ackLevel = 1;
    __uint8_t isMaster = 1;
    __uint8_t firstTxLSB = 0;
    __uint8_t firstRxLSB = 0;
    i2cInit(mpu6050Ctrl->i2c,
            sdaSampleLevel,
            ackLevel,
            isMaster,
            firstTxLSB,
            firstRxLSB); ///// UHHHHHHH idk maybe look into this functions register again

    // I2C Clk config
    __uint16_t sclLowPeriod = 8;
    __uint16_t dataHoldPeriod = sclLowPeriod + 1;
    __uint16_t sclHighPeriod = sclLowPeriod;
    __uint8_t sclWaitHighPeriod = 0;
    __uint16_t sclStartHoldPeriod = 2;
    __uint8_t statusFSMTimeout = 0;
    __uint8_t mainFSMTimeout = 0;

    i2cSclCfg(mpu6050Ctrl->i2c,
                sclLowPeriod,
                dataHoldPeriod,
                sclHighPeriod,
                sclWaitHighPeriod,
                sclStartHoldPeriod,
                statusFSMTimeout,
                mainFSMTimeout);

    // APB Bus Clk config
    __uint8_t useRCFastClk = 0;
    __uint32_t clkIntegral = 0;
    __uint32_t clkNumerator = 1;
    __uint32_t clkDenominator = 1;

    i2cAPBClkCfg(mpu6050Ctrl->i2c,
                    useRCFastClk,
                    clkIntegral,
                    clkNumerator,
                    clkDenominator);
}

void mpu6050Write(mpu6050Controller *mpu6050Ctrl,
                    __uint32_t *buffer,
                    __uint32_t numBytes)
{
    // set the GPIO to be Output (gpioSetOutputPin)

    // enable output (gpioOutputEnable)

    // write to tx buffer

    // write to commands 

    // set the tx bit
    return;

}

__uint32_t *mpu6050Read(mpu6050Controller *mpu6050Ctrl,
                            __uint32_t *buffer,
                            __uint32_t numBytes)
{
    // set the GPIO to be input (gpioSetInputPin)

    // disable output (gpioOutputDisable)

    // set the commands 

    // set the tx bit????
    return 0;
}