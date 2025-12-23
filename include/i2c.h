#ifndef I2C_H
#define I2C_H

#include <stdio.h>
#include "utility.h"

#define I2C0 0x60013000UL
#define I2C0TxRAMStart (I2C0 + 0x100UL)
#define I2C0RxRAMStart (I2C0 + 0x180UL)

extern __uint32_t I2C0TxRAMStackPointer;

typedef enum 
{
    reserved0,
    WRITE,
    STOP,
    READ,
    END,
    reserved5,
    RSTART
}i2cCommands;

typedef struct
{
    volatile __uint32_t I2C_SCL_LOW_PERIOD_REG;         // 0x0000
    volatile __uint32_t I2C_CTR_REG;                    // 0x0004
    volatile __uint32_t I2C_SR_REG;                     // 0x0008
    volatile __uint32_t I2C_TO_REG;                     // 0x000C
    volatile __uint32_t I2C_SLAVE_ADDR_REG;             // 0x0010
    volatile __uint32_t I2C_FIFO_ST_REG;                // 0x0014
    volatile __uint32_t I2C_FIFO_CONF_REG;              // 0x0018
    volatile __uint32_t I2C_DATA_REG;                   // 0x001c
    volatile __uint32_t I2C_INT_RAW_REG;                // 0x0020
    volatile __uint32_t I2C_INT_CLR_REG;                // 0x0024
    volatile __uint32_t I2C_INT_ENA_REG;                // 0x0028
    volatile __uint32_t I2C_INT_STATUS_REG;             // 0x002C
    volatile __uint32_t I2C_SDA_HOLD_REG;               // 0x0030
    volatile __uint32_t I2C_SDA_SAMPLE_REG;             // 0x0034
    volatile __uint32_t I2C_SCL_HIGH_PERIOD_REG;        // 0x0038
    volatile __uint32_t RESERVED_1;                     // 0x003C
    volatile __uint32_t I2C_SCL_START_HOLD_REG;         // 0x0040
    volatile __uint32_t I2C_SCL_RSTART_SETUP_REG;       // 0x0044
    volatile __uint32_t I2C_SCL_STOP_HOLD_REG;          // 0x0048
    volatile __uint32_t I2C_SCL_STOP_SETUP_REG;         // 0x004C
    volatile __uint32_t I2C_FILTER_CFG_REG;             // 0x0050
    volatile __uint32_t I2C_CLK_CONF_REG;               // 0x0054
    volatile __uint32_t I2C_COMD0_REG;                  // 0x0058
    volatile __uint32_t I2C_COMD1_REG;                  // 0x005C
    volatile __uint32_t I2C_COMD2_REG;                  // 0x0060
    volatile __uint32_t I2C_COMD3_REG;                  // 0x0064
    volatile __uint32_t I2C_COMD4_REG;                  // 0x0068
    volatile __uint32_t I2C_COMD5_REG;                  // 0x006C
    volatile __uint32_t I2C_COMD6_REG;                  // 0x0070
    volatile __uint32_t I2C_COMD7_REG;                  // 0x0074
    volatile __uint32_t I2C_SCL_ST_TIME_OUT_REG;        // 0x0078
    volatile __uint32_t I2C_SCL_MAIN_ST_TIME_OUT_REG;   // 0x007C
    volatile __uint32_t I2C_SCL_SP_CONF_REG;            // 0x0080
    volatile __uint32_t I2C_SCL_STRETCH_CONF_REG;       // 0x0084
    const __uint32_t I2C_DATE_REG;                       // 0x00F8

}i2cStructure;

typedef struct
{
    __uint16_t i2cSclLowPeriod;
    __uint16_t i2cSdaHoldPeriod;
    __uint16_t i2cSdaSampleReg;
    __uint16_t i2cSclHighPeriod;
    __uint16_t i2cSclWaitHighPeriod;
    __uint16_t i2cSclStartHoldPeriod;
    __uint16_t i2cSclRstartSetupPeriod;
    __uint16_t i2cSclStopHoldPeriod;
    __uint16_t i2cSclStopSetupPeriod;
}i2cSclTimingConfig_t;

typedef struct
{
    __uint8_t i2cSclFsmTimeOutVal;
    __uint8_t i2cSclMainFsmTimeOutVal;
    __uint8_t i2cDataReceiveTimeoutVal;
    bool_t i2cDataReceiveTimeOutEnable
}i2cTimeoutConfig_t;

typedef struct
{
    bool_t sdaUseOpenDrain;
    bool_t sclUseOpenDrain;
    bool_t sampleSdaOnSclLow;
    bool_t slaveFullRxAckLevel;
    bool_t isMaster;
    bool_t startTransmission;
    bool_t sendLSBFirst;
    bool_t receiveLSBFirst;
    bool_t apbClkEnable;
    bool_t useBusArbitration;
    bool_t resetSclFsm;
    bool_t synchronizeRegisters;
    bool_t slaveAutoSendEnable;
    bool_t checkFor10BitAddress;
    bool_t generalAddressBroadcastEnable;
}i2cTransmissionConfig_t;

typedef struct
{
    __uint16_t slaveAddress;
    bool_t enableSlave10BitAddress;
    __uint16_t slaveMaxSclStretch;
    bool_t slaveSclStretchEnable;
    bool_t slaveClearSclStretch;
    bool_t slaveAckLevelControlEnable;
    bool_t slaveAckLevel;
}i2cSlaveConfig_t;

typedef struct
{
    __uint8_t rxFifoInterruptThreshold;
    __uint8_t txFifoInterruptThreshold;
    bool_t nonFifoModeEnable;
    bool_t slaveRamAddressEnable; // second byte received as slave is RAM addr
    bool_t resetRxFifo;
    bool_t resetTxFifo;
    bool_t fifoControlEnable;
}i2cFifoConfig_t;

typedef struct
{
    __uint8_t sclFilterThreshold;
    __uint8_t sdaFilterThreshold;
    bool_t sclFilterEnable;
    bool_t sdaFilterEnable;
}i2cFilterConfig_t;

typedef struct
{
    __uint8_t sclkDivNum;
    __uint8_t sclkNumerator;
    __uint8_t sclkDemoninator;
    bool_t useRcFastClk;
    bool_t enableSclk;
}i2cSclkConfig_t;

typedef struct
{
    bool_t slaveSendIdlePulse;
    __uint8_t numPulses;
    bool_t sclPowerDownEnable;
    bool_t sdaPowerDownEnable;
}i2cPowerConfig_t;

void i2cEnableSystemPeripheral();

void i2cInit(i2cStructure *i2c, 
                bool_t isMaster,
                bool_t useFifo,
                bool_t useFilter);

void i2cSclTimingConfig(i2cStructure *i2c,
                        i2cSclTimingConfig_t *sclTimingConfig);

void i2cTimeoutConfig(i2cStructure *i2c,
                        i2cTimeoutConfig_t *timeoutConfig);

void i2cTransmissionConfig(i2cStructure *i2c,
                            i2cTransmissionConfig_t *transmissionConfig);

void i2cSlaveConfig(i2cStructure *i2c,
                    i2cSlaveConfig_t *slaveConfig);

void i2cFifoConfig(i2cStructure *i2c,
                    i2cFifoConfig_t *fifoConfig);

void i2cFilterConfig(i2cStructure *i2c,
                        i2cFilterConfig_t *filterConfig);

void i2cSclkConfig(i2cStructure *i2c,
                    i2cSclkConfig_t *sclkConfig);

void i2cPowerConfig(i2cStructure *i2c,
                    i2cPowerConfig_t *powerConfig);

// void i2cSclCfg(i2cStructure *i2c,
//                 __uint16_t sclLowPeriod,
//                 __uint16_t sdaHoldPeriod,
//                 __uint16_t sclHighPeriod,
//                 __uint8_t sclWaitHighPeriod,
//                 __uint16_t sclStartHoldPeriod,
//                 __uint8_t statusFSMTimeout,
//                 __uint8_t mainFSMTimeout);

// void i2cAPBClkCfg(i2cStructure *i2c,
//                     __uint8_t useRCFastClk,
//                     __uint32_t clkIntegral,
//                     __uint32_t clkNumerator,
//                     __uint32_t clkDenominator);

void setCommand(i2cStructure *i2c,
                    i2cCommands opcode,
                    __uint8_t masterReadAckVal,
                    __uint8_t masterWriteAckVal,
                    __uint8_t checkReceivedAckVal,
                    __uint8_t numBytes,
                    __uint8_t commandNum);

void i2cClearRxRAM(i2cStructure *i2c);

void i2cClearTxRAM(i2cStructure *i2c);

void readRxRAM(i2cStructure *i2c,
                __uint32_t numBytes,
                __uint32_t *buffer);

void writeTxRAM(i2cStructure *i2c,
                __uint32_t numByte,
                __uint32_t *buffer);

// void sclEnablePulse(i2cStructure *i2c, __uint8_t numPulses);
                
#endif // I2C_H