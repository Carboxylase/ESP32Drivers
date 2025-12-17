#include "include/i2c.h"
#include "include/system.h"
#include "include/debugTools.h"

void i2cInit(i2cStructure *i2c, 
            __uint8_t sdaSampleLevel, 
            __uint8_t ackLevel, 
            __uint8_t isMaster, 
            __uint8_t firstTxLSB,
            __uint8_t firstRxLSB)
{
    // enable the system clock for I2C
    volatile __uint32_t *enableClk = (volatile __uint32_t*)SYSTEM_PERIP_CLK_EN0_REG;

    *enableClk |= 1 << 7;

    volatile __uint32_t *clearRst = (volatile __uint32_t*)SYSTEM_PERIP_RST_EN0_REG;

    *clearRst &= ~(1 << 7);

    // reset the register for the I2C config
    volatile __uint32_t *setReg = (volatile __uint32_t*)&(i2c->I2C_CTR_REG);

    *setReg &= 0;

    // Enable the APB clock
    *setReg |= 1 << 8;
    // Set open-drain Rx and Tx (mandatory)
    *setReg |= 0 << 0;
    *setReg |= 0 << 1;
    // Configure everthing else
    *setReg |= sdaSampleLevel << 2;
    *setReg |= ackLevel << 3;
    *setReg |= isMaster << 4;
    *setReg |= firstTxLSB << 6;
    *setReg |= firstRxLSB << 7;

    // 7-bit address broadcasting enabled
    *setReg |= 1 << 14;


    // reset the SCL state machine
    // *setReg |= 1 << 10;
    // *setReg &= ~(1 << 10);

    // synchronize the i2c registers with the two clocks
    *setReg |= 1 << 11;
}

void i2cSclCfg(i2cStructure *i2c,
                __uint16_t sclLowPeriod,
                __uint16_t sdaHoldPeriod,
                __uint16_t sclHighPeriod,
                __uint8_t sclWaitHighPeriod,
                __uint16_t sclStartHoldPeriod,
                __uint8_t statusFSMTimeout,
                __uint8_t mainFSMTimeout)
{

    if (sclLowPeriod > 511)
    {
        return;
    }
    if (sdaHoldPeriod > 511)
    {
        return;
    }
    if (sclHighPeriod > 511)
    {

    }
    if (sclWaitHighPeriod > 127)
    {
        return;
    }
    if (sclStartHoldPeriod > 511)
    {
        return;
    }
    
    // Set low level width
    i2c->I2C_SCL_LOW_PERIOD_REG = sclLowPeriod;

    // Set how many cycles we hold the SDA line after the CLK falling edge
    i2c->I2C_SDA_HOLD_REG = sdaHoldPeriod;

    // Set high level width
    i2c->I2C_SCL_HIGH_PERIOD_REG |= sclHighPeriod << 0;

    i2c->I2C_SCL_HIGH_PERIOD_REG |= sclWaitHighPeriod << 9;

    // Set when we sample SDA when CLK is high
    i2c->I2C_SDA_SAMPLE_REG = sclHighPeriod / 2 + 1;

    // Set the time between the SDA faling edge and CLK falling edge
    i2c->I2C_SCL_START_HOLD_REG = sclStartHoldPeriod;

    // Set the status FSM cycle timeout, should not be greater then 23
    if (statusFSMTimeout > 23)
    {
        i2c->I2C_SCL_ST_TIME_OUT_REG = 23;
    }
    else
    {
        i2c->I2C_SCL_ST_TIME_OUT_REG = statusFSMTimeout;
    }

    // Set the main FSM cycle timeout, should not be greater than 23
    if (mainFSMTimeout > 23)
    {
        i2c->I2C_SCL_MAIN_ST_TIME_OUT_REG = 23;
    }
    else
    {
        i2c->I2C_SCL_MAIN_ST_TIME_OUT_REG = mainFSMTimeout;
    }
    
}

void i2cAPBClkCfg(i2cStructure *i2c,
                    __uint8_t useRCFastClk,
                    __uint32_t clkIntegral,
                    __uint32_t clkNumerator,
                    __uint32_t clkDenominator)
{

    if (useRCFastClk > 1)
    {
        printf("Invalid clock selector for ABP clock: %u\n", useRCFastClk);
        return;
    }
    if (clkIntegral > 255)
    {
        printf("Invalid clock intergral for APB clock: %lu\n", clkIntegral);
        return;
    }
    if (clkNumerator > 63)
    {
        printf("Invalid clock Div A for APB clock: %lu\n", clkNumerator);
        return;
    }
    if (clkDenominator > 63)
    {
        printf("Invalid clock Div B for APB clock: %lu\n", clkDenominator);
        return;
    }

    // Clear the clock config
    i2c->I2C_CLK_CONF_REG &= 0;

    // Set clock used -> 1 = rc fast clock, 0 = xtal
    if (useRCFastClk)
    {
        i2c->I2C_CLK_CONF_REG |= 1 << 20;
    }
    else
    {
        i2c->I2C_CLK_CONF_REG &= ~(1 << 20);
    }

    // enable the clock
    i2c->I2C_CLK_CONF_REG |= 1 << 21;

    //
    i2c->I2C_CLK_CONF_REG |= clkIntegral << 0;

    i2c->I2C_CLK_CONF_REG |= clkNumerator << 8;

    i2c->I2C_CLK_CONF_REG |= clkDenominator << 14;


}

void i2cClearRxFIFO(i2cStructure *i2c)
{
    i2c->I2C_FIFO_CONF_REG |= 1 << 12;
    i2c->I2C_FIFO_CONF_REG &= ~(1 << 12);
}

void i2cClearTxFIFO(i2cStructure *i2c)
{
    i2c->I2C_FIFO_CONF_REG |= 1 << 13;
    i2c->I2C_FIFO_CONF_REG &= ~(1 << 13);
}

void readRxRAM(i2cStructure *i2c, __uint32_t numBytes, __uint32_t *buffer)
{
    __uint32_t *rxRAM = (__uint32_t*)(i2c + 0x100U);
    
    if (numBytes > 32)
    {
        printf("Number of bytes to read exceeds Buffer size: %lu\n", numBytes);
        return;
    }

    for (int i = 0; i < numBytes; i++)
    {
        buffer[i] = *(rxRAM + i);
    }
}

// 
void writeTxRAM(i2cStructure *i2c, __uint32_t numByte, __uint32_t *buffer)
{
    if (numByte > 32)
    {
        printf("Requested number of bytes to write exceed RAM size: %lu\n", numByte);
        return;
    }

    // Set the Tx RAM into Non-FIFO mode (direct access)
    i2c->I2C_FIFO_CONF_REG |= 1 << 10;

    __uint8_t *i2cAddr = (__uint8_t*)i2c;
    // __uint8_t txRamAddr = i2cAddr + 0x100UL;

    volatile __uint32_t *txRAM = (volatile __uint32_t*)(i2cAddr + 0x100UL);

    for (int i = 0; i < numByte; i++)
    {
        *txRAM &= 0;
        *txRAM |= buffer[i];
        txRAM += 1;  
    }

}

void setCommand(i2cStructure *i2c,
                    i2cCommands opcode,
                    __uint8_t masterReadAckVal,
                    __uint8_t masterWriteAckVal,
                    __uint8_t checkReceivedAckVal,
                    __uint8_t numBytes,
                    __uint8_t commandNum)
{
    if (opcode > 6)
    {
        return;
    }
    if (masterReadAckVal > 1)
    {
        return;
    }
    if (masterWriteAckVal > 1)
    {
        return;
    }
    if (checkReceivedAckVal > 1)
    {
        return;
    }
    if (commandNum > 7)
    {
        return;
    }

    volatile __uint32_t *commandReg = NULL; 

    switch (commandNum)
    {
        case 0:
            commandReg = (volatile __uint32_t *)i2c->I2C_COMD0_REG;
            break;

        case 1:
            commandReg = (volatile __uint32_t *)i2c->I2C_COMD1_REG;
            break;

        case 2:
            commandReg = (volatile __uint32_t *)i2c->I2C_COMD2_REG;
            break;

        case 3:
            commandReg = (volatile __uint32_t *)i2c->I2C_COMD3_REG;
            break;
        
        case 4:
            commandReg = (volatile __uint32_t *)i2c->I2C_COMD4_REG;
            break;

        case 5:
            commandReg = (volatile __uint32_t *)i2c->I2C_COMD5_REG;
            break;

        case 6:
            commandReg = (volatile __uint32_t *)i2c->I2C_COMD6_REG;
            break;

        case 7:
            commandReg = (volatile __uint32_t *)i2c->I2C_COMD7_REG;
            break;

        // this will also reset the CMD_DONE bit
        *commandReg &= 0;
        
        *commandReg |= numBytes << 0;

        *commandReg |= checkReceivedAckVal << 8;

        *commandReg |= masterWriteAckVal << 9;

        *commandReg |= masterReadAckVal << 10;

        *commandReg |= (__uint8_t)opcode << 11;

    }

    return;
}

void sclEnablePulse(i2cStructure *i2c, __uint8_t numPulses)
{
    if (numPulses > 31)
    {
        return;
    }

    i2c->I2C_SCL_SP_CONF_REG |= 1 << 0;
    
    i2c->I2C_SCL_SP_CONF_REG |= numPulses << 1;
}

void sclDisablePulse(i2cStructure *i2c)
{
    return;
}
