#include "include/mpu6050.h"
#include "include/gpio.h"
#include "include/i2c.h"


void init6050Module(mpu6050Controller *mpu6050Ctrl)
{
    //---Setup for SDA---//
    // NOTE: I just applied the same settings of the SCL to the SDA, maybe check over this later
    gpioPinConfig_t sdaGpioPinConfig = {.phase2SyncGpioInputWithApb = NO_SYNC,
                                        .useOpenDrainOutput = FALSE,
                                        .phase1SyncGpioInputWithApb = NO_SYNC,
                                        .gpioInterruptType = DISABLED,
                                        .gpioWakeupCpuEnable = FALSE,
                                        .gpioCpuInterruptEnabe = FALSE,
                                        .gpioCpuNonMaskInterruptEnable = FALSE};

    gpioPinSettings(mpu6050Ctrl->sdaPinNum,
                        sdaGpioPinConfig);

    // connect the GPIO (sda) pin to the I2C peripheral (gpioIoMuxCfg)
    
    //---Setup for SCL---//

    // set scl GPIO pin to be output, since we scl always comes from master (gpioSetOutputPin)

    gpioOutputConfig_t sclGpioOutputConfig = {.functionNum = I2CEXT0_SCL,
                                                .invertOutput = FALSE,
                                                .useGpioOutputEnable = TRUE,
                                                .invertOutputEnable = FALSE};

    gpioSetOutputPin(mpu6050Ctrl->sclPinNum,
                        sclGpioOutputConfig);

    // enable output for the scl GPIO pin
    gpioOutputEnable(mpu6050Ctrl->sclPinNum);

    // configure GPIO settings for scl (gpioPinSettings)
    gpioPinConfig_t sclGpioPinConfig = {.phase2SyncGpioInputWithApb = NO_SYNC,
                                        .useOpenDrainOutput = FALSE,
                                        .phase1SyncGpioInputWithApb = NO_SYNC,
                                        .gpioInterruptType = DISABLED,
                                        .gpioWakeupCpuEnable = FALSE,
                                        .gpioCpuInterruptEnabe = FALSE,
                                        .gpioCpuNonMaskInterruptEnable = FALSE};

    gpioPinSettings(mpu6050Ctrl->sclPinNum,
                        sclGpioPinConfig);

    // enable output via GPIO matrix (gpioIoMuxCfg)
    ioMuxConfig_t sclIoMuxConfig = {.sleepOutputEnable = TRUE,
                                    .sleepModeEnable = FALSE,
                                    .sleepWeakPullDownEnable = FALSE,
                                    .sleepWeakPullUpEnable = FALSE,
                                    .sleepInputEnable = FALSE,
                                    .sleepGpioDriveStrength = 2,
                                    .weakPullDownEnable = FALSE,
                                    .weakPullUpEnable = FALSE,
                                    .inputEnable = FALSE,
                                    .gpioDriveStrength = 2,
                                    .mcuSel = 1,
                                    .inputFilterEnable = FALSE};

    gpioIoMuxCfg(mpu6050Ctrl->sclPinNum,
                    sclIoMuxConfig);

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
    // make the i2c controller decide if it is
    gpioOutputConfig_t sdaGpioOutputConfig = {.functionNum = I2CEXT0_SDA,
                                                .invertOutput = FALSE,
                                                .useGpioOutputEnable = FALSE,
                                                .invertOutputEnable = FALSE};

    gpioSetOutputPin(mpu6050Ctrl->sdaPinNum, sdaGpioOutputConfig);

    // enable output (gpioOutputEnable)
    gpioOutputEnable(mpu6050Ctrl->sdaPinNum);

    // gpioPinSetting
    gpioPinConfig_t sdaGpioPinConfig = {.phase2SyncGpioInputWithApb = NO_SYNC,
                                        .useOpenDrainOutput = TRUE,
                                        .phase1SyncGpioInputWithApb = NO_SYNC,
                                        .gpioInterruptType = DISABLED,
                                        .gpioWakeupCpuEnable = FALSE,
                                        .gpioCpuInterruptEnabe = FALSE,
                                        .gpioCpuNonMaskInterruptEnable = FALSE};

    gpioPinSettings(mpu6050Ctrl->sdaPinNum, sdaGpioPinConfig);

    // gpioIoMuxCfg
    ioMuxConfig_t sdaIoMuxConfig = {.sleepOutputEnable = TRUE,
                                    .sleepModeEnable = FALSE,
                                    .sleepWeakPullDownEnable = FALSE,
                                    .sleepWeakPullUpEnable = FALSE,
                                    .sleepInputEnable = TRUE,
                                    .sleepGpioDriveStrength = 2,
                                    .weakPullDownEnable = FALSE,
                                    .weakPullUpEnable = FALSE,
                                    .inputEnable = FALSE,
                                    .gpioDriveStrength = 2,
                                    .mcuSel = GPIO_MATRIX,
                                    .inputFilterEnable = FALSE};

    gpioIoMuxCfg(mpu6050Ctrl->sdaPinNum, sdaIoMuxConfig);

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