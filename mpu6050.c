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
    __uint8_t pullDownEnableScl = 0;
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
    __uint8_t sdaSampleLevel = 0;
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

    __uint8_t numPulses = 30;
    sclEnablePulse(mpu6050Ctrl->i2c, numPulses);
}

void mpu6050Write(mpu6050Controller *mpu6050Ctrl,
                    __uint32_t *buffer,
                    __uint8_t numBytes)
{
    // set     i2c->I2C_CTR_REG |= 1 << 5;the GPIO to be Output (gpioSetOutputPin)
    __uint32_t functionNum = I2CEXT0_SDA;
    __uint8_t invertOutput = 0;
    __uint8_t usePeriOutputEnable = 1;
    __uint8_t invertOutputEnable = 0;
    gpioSetOutputPin(mpu6050Ctrl->sdaPinNum,
                        functionNum,
                        invertOutput,
                        usePeriOutputEnable,
                        invertOutputEnable);

    // enable output (gpioOutputEnable)
    gpioOutputEnable(mpu6050Ctrl->sdaPinNum);

    // gpioPinSetting
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

    // gpioIoMuxCfg
    __uint8_t pullDownEnable = 0;
    __uint8_t pullUpEnable = 0;
    __uint8_t inputEnable = 0;
    __uint8_t driveStrength = 0;
    __uint8_t mcuSel = 1;
    __uint8_t filterEnable = 1;
    gpioIoMuxCfg(mpu6050Ctrl->sdaPinNum,
                    pullDownEnable,
                    pullUpEnable,
                    inputEnable,
                    driveStrength,
                    mcuSel,
                    filterEnable);

    // write to tx buffer
    writeTxRAM(mpu6050Ctrl->i2c,numBytes,buffer);

    // write to commands 
    //UHHHH idk maybe just set the commands seperately from this function
    i2cCommands opcode = RSTART;
    __uint8_t masterReadAckVal = 0;
    __uint8_t masterWriteAckVal = 0;
    __uint8_t checkReceivedAckVal = 0;
    __uint8_t numberBytes = 0;
    __uint8_t commandNum = 0;
    setCommand(mpu6050Ctrl->i2c,
                opcode,
                masterReadAckVal,
                masterWriteAckVal,
                checkReceivedAckVal,
                numberBytes,
                commandNum);

    opcode = WRITE;
    numberBytes = numBytes;
    commandNum = 1;
    setCommand(mpu6050Ctrl->i2c,
                opcode,
                masterReadAckVal,
                masterWriteAckVal,
                checkReceivedAckVal,
                numberBytes,
                commandNum);

    opcode = STOP;
    numberBytes = 0;
    commandNum = 2;
    setCommand(mpu6050Ctrl->i2c,
                opcode,
                masterReadAckVal,
                masterWriteAckVal,
                checkReceivedAckVal,
                numberBytes,
                commandNum);
    
    // set the tx bit
    mpu6050Ctrl->i2c->I2C_CTR_REG |= 0x1UL << 5;

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