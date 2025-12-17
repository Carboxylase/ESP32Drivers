/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"

#include "include/i2c.h"
#include "include/debugTools.h"
#include "include/gpio.h"
#include "include/mpu6050.h"

void test_set_gpio();

void test_tx_ram();

void gpioOutputSetup();

void gpioSetupSDA();

void app_main(void)
{
    printf("Starting setup..,\n");

    i2cStructure *i2c = (i2cStructure*) I2C0;
    __uint32_t sdaPinNum = 8;
    __uint32_t sclPinNum = 7;

    mpu6050Controller mpu6050Ctrl;
    mpu6050Ctrl.i2c = i2c;
    mpu6050Ctrl.sdaPinNum = sdaPinNum;
    mpu6050Ctrl.sclPinNum = sclPinNum;

    init6050Module(&mpu6050Ctrl);

    while(true)
    {
        // just print to the sda line

        __uint32_t bufferPtr[5] = {0x01UL, 0x02UL, 0x04UL, 0x08UL, 0x16UL};
        // writeTxRAM(mpu6050Ctrl.i2c,5,bufferPtr);
        mpu6050Write(&mpu6050Ctrl, bufferPtr, 5);
    };
}

void test_set_gpio()
{
    __uint32_t i2cSclPin = 8;
    __uint32_t gpioFunction = I2CEXT0_SCL;
    __uint8_t useGpioMatrix = 1;
    __uint8_t invert = 0;
    volatile __uint8_t *printMemAddr = (__uint8_t*)(GPIO_FUNC_Y_IN_SEL_CFG_REG_Base_Address + 0x4UL*gpioFunction);

    printf("%p", printMemAddr);

    printMemory(printMemAddr, sizeof(__uint32_t));

    gpioSetInputPin(i2cSclPin, gpioFunction, useGpioMatrix, invert);

    printMemory(printMemAddr, sizeof(__uint32_t));
}

void test_tx_ram()
{
    i2cStructure *i2c = (i2cStructure*)I2C0;
    __uint32_t numByte  = 4;
    __uint32_t *buffer;
    __uint32_t bufferArr[] = {0x0UL, 0x00UL, 0x00UL, 0x00UL};
    buffer = bufferArr;

    __uint8_t *txMemStart = (__uint8_t*) I2C0TxRAMStart;

    printf("Tx Addr: %p\n", txMemStart);
    printf("i2c start addr: %p\n", (void*)i2c);

    printMemory(txMemStart, 4*sizeof(__uint32_t));

    writeTxRAM(i2c,numByte,buffer);

    printMemory(txMemStart, 4*sizeof(__uint32_t));


}

void gpioSetupSCL()
{
    //Step 1
    __uint32_t pinNum = 10;
    __uint32_t functionNum = I2CEXT0_SCL;
    __uint8_t invertOutput = 0;
    __uint8_t useGpioOutputEnable = 1;
    __uint8_t invertOutputEnable = 0;
    gpioSetOutputPin(pinNum,
                        functionNum,
                        invertOutput,
                        useGpioOutputEnable,
                        invertOutputEnable);

    gpioOutputEnable(pinNum);

    //Step 2
    __uint8_t syncPeriClk = 1;
    __uint8_t syncBusClk = 1;
    __uint8_t useOpenDrainOutput = 1;
    __uint8_t interruptType = 0;
    __uint8_t pinWakeupEnable = 0;
    __uint8_t cpuInterruptEnable = 0;
    __uint8_t nonMaskableInterruptEnable = 0;

    gpioPinSettings(pinNum,
                        syncPeriClk,
                        syncBusClk,
                        useOpenDrainOutput,
                        interruptType,
                        pinWakeupEnable,
                        cpuInterruptEnable,
                        nonMaskableInterruptEnable);

    //Setp 3
    __uint8_t pullDownEnable = 1;
    __uint8_t pullUpEnable = 0;
    __uint8_t inputEnable = 0;
    __uint8_t driveStrength = 1;
    __uint8_t mcuSel = 1;
    __uint8_t filterEnable = 1;
    gpioIoMuxCfg(pinNum,
                    pullDownEnable,
                    pullUpEnable,
                    inputEnable,
                    driveStrength,
                    mcuSel,
                    filterEnable);
    return;
}

void gpioSetupSDA()
{
    return;
}

void i2cSetup()
{
    return;
}