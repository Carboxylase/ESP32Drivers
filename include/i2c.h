#ifndef I2C_H
#define I2C_H

#include <stdio.h>

#define I2C0 0x60013000UL
#define I2C0TxRAMStart (I2C0 + 0x100UL)
#define I2C0RxRAMStart (I2C0 + 0x180UL)

extern __uint32_t I2C0TxRAMStackPointer;

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

void i2cInit(i2cStructure *i2c, 
            __uint8_t sdaSampleLevel, 
            __uint8_t ackLevel, 
            __uint8_t isMaster, 
            __uint8_t firstTxLSB,
            __uint8_t firstRxLSB);

void i2cSclCfg(i2cStructure *i2c,
                __uint16_t sclLowPeriod,
                __uint16_t sdaHoldPeriod,
                __uint16_t sclHighPeriod,
                __uint8_t sclWaitHighPeriod,
                __uint16_t sclStartHoldPeriod,
                __uint8_t statusFSMTimeout,
                __uint8_t mainFSMTimeout);

void i2cAPBClkCfg(i2cStructure *i2c,
                    __uint8_t useRCFastClk,
                    __uint32_t clkIntegral,
                    __uint32_t clkNumerator,
                    __uint32_t clkDenominator);

void setCommand(i2cStructure *i2c,
                    __uint32_t commandNum,
                    __uint8_t opcode,
                    __uint8_t ackValue,
                    __uint8_t ackExp,
                    __uint8_t ackCheckEn,
                    __uint8_t numBytes);

void i2cClearRxRAM(i2cStructure *i2c);

void i2cClearTxRAM(i2cStructure *i2c);

void readRxRAM(i2cStructure *i2c,
                __uint32_t numBytes,
                __uint32_t *buffer);

void writeTxRAM(i2cStructure *i2c,
                __uint32_t numByte,
                __uint32_t *buffer);
                
#endif // I2C_H