#include "include/i2c.h"

void i2cInit(i2cStructure *i2c, 
            __uint8_t sdaSampleLevel, 
            __uint8_t ackLevel, 
            __uint8_t isMaster, 
            __uint8_t firstTxLSB,
            __uint8_t firstRxLSB)
{
    // Reset the configurations
    i2c->I2C_CTR_REG &= 0;

    // Enable the APB clock
    i2c->I2C_CTR_REG |= 1 << 8;
    // Set open-drain Rx and Tx (mandatory)
    i2c->I2C_CTR_REG |= 1 << 0;
    i2c->I2C_CTR_REG |= 1 << 1;
    // Configure everthing else
    i2c->I2C_CTR_REG |= sdaSampleLevel << 2;
    i2c->I2C_CTR_REG |= ackLevel << 3;
    i2c->I2C_CTR_REG |= isMaster << 4;
    i2c->I2C_CTR_REG |= firstTxLSB << 6;
    i2c->I2C_CTR_REG |= firstRxLSB << 7;
    i2c->I2C_CTR_REG |= 1 << 14;
}

void i2cSclCfg(i2cStructure *i2c,
                __uint32_t lowLevelCycles,
                __uint32_t dataHoldNegClk,
                __uint32_t highLevelCycles,
                __uint32_t startCLKRiseSDAFall,
                __uint32_t stopSetupCycle,
                __uint32_t stopHoldClkCycles,
                __uint32_t statusFSMTimeout,
                __uint32_t mainFSMTimeout)
{
    // Set low level width
    i2c->I2C_SCL_LOW_PERIOD_REG = lowLevelCycles;

    // Set how many cycles we hold the SDA line after the CLK falling edge
    i2c->I2C_SDA_HOLD_REG = dataHoldNegClk;

    // Set high level width
    i2c->I2C_SCL_HIGH_PERIOD_REG = highLevelCycles;

    // Set when we sample SDA when CLK is high
    i2c->I2C_SDA_SAMPLE_REG = highLevelCycles / 2 + 1;

    // Set the time between the SDA faling edge and CLK falling edge
    i2c->I2C_SCL_START_HOLD_REG = highLevelCycles - startCLKRiseSDAFall;

    // Set the time between the SCL rising edge to the falling edge of SDA on start up
    i2c->I2C_SCL_RSTART_SETUP_REG = startCLKRiseSDAFall;

    // Set the time between the rising edge of the CLK and the rising edge of SDA for stop
    i2c->I2C_SCL_STOP_SETUP_REG = stopSetupCycle;

    // Set the time for how long SDA is held after it goes high for stop
    i2c->I2C_SCL_STOP_HOLD_REG = stopHoldClkCycles;

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
    i2c->I2C_CLK_CONF_REG = 0;

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

    i2c->I2C_CLK_CONF_REG |= clkNumerator << 9;

    i2c->I2C_CLK_CONF_REG |= clkDenominator << 14;


}

void setCommand(i2cStructure *i2c,
                    __uint32_t commandNum,
                    __uint8_t opcode,
                    __uint8_t ackValue,
                    __uint8_t ackExp,
                    __uint8_t ackCheckEn,
                    __uint8_t numBytes)
{

    volatile __uint32_t *command;
    switch(commandNum)
    {
        case 0:
            command = &i2c->I2C_COMD0_REG;
            break;
        case 1:
            command = &i2c->I2C_COMD1_REG;
            break;
        case 2:
            command = &i2c->I2C_COMD2_REG;
            break;
        case 3:
            command = &i2c->I2C_COMD3_REG;
            break;
        case 4:
            command = &i2c->I2C_COMD4_REG;
            break;
        case 5:
            command = &i2c->I2C_COMD5_REG;
            break;
        case 6:
            command = &i2c->I2C_COMD6_REG;
            break;
        case 7:
            command = &i2c->I2C_COMD7_REG;
            break;
        default:
            printf("Invalid command register\n");
            return;
    }

    if (ackCheckEn > 1)
    {
        printf("The ackCheckEn value is not of bool type: %d", ackCheckEn);
        return;
    }
    if (ackExp > 1)
    {
        printf("The ackExp is not a bool value: %d", ackExp);
        return;
    }
    if (ackValue > 1)
    {
        printf("The ackValue is not a bool value: %d", ackValue);
    }
    if (opcode > 6)
    {
        printf("Invalid opcode: %d", opcode);
    }
    
    // BRUH YOU MIGHT WANT TO CHECK IF YOU ARE CLEARING OR SETTING A BIT FUKKKKKKKK
    
    // clear the old command
    *command &= 0;

    *command |= numBytes << 0;

    *command |= ackCheckEn << 8;

    *command |= ackExp << 9;

    *command |= ackValue << 10;

    *command |= opcode << 11;
}

void i2cClearRxRAM(i2cStructure *i2c)
{
    i2c->I2C_FIFO_CONF_REG |= 1 << 12;
    i2c->I2C_FIFO_CONF_REG &= ~(1 << 12);
}

void i2cClearTxRAM(i2cStructure *i2c)
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
        buffer[i] = *(rxRAM + i*4);
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

    __uint32_t *txRAM = (__uint32_t*)(i2c + 0x180U);

    for (int i = 0; i < numByte; i++)
    {
        *txRAM = buffer[i];
        txRAM += i*4;
    }

}

