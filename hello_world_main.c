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
    printf("Hello world!\n");

    /* Print chip information */
    esp_chip_info_t chip_info;
    uint32_t flash_size;
    esp_chip_info(&chip_info);
    printf("This is %s chip with %d CPU core(s), %s%s%s%s, ",
           CONFIG_IDF_TARGET,
           chip_info.cores,
           (chip_info.features & CHIP_FEATURE_WIFI_BGN) ? "WiFi/" : "",
           (chip_info.features & CHIP_FEATURE_BT) ? "BT" : "",
           (chip_info.features & CHIP_FEATURE_BLE) ? "BLE" : "",
           (chip_info.features & CHIP_FEATURE_IEEE802154) ? ", 802.15.4 (Zigbee/Thread)" : "");

    unsigned major_rev = chip_info.revision / 100;
    unsigned minor_rev = chip_info.revision % 100;
    printf("silicon revision v%d.%d, ", major_rev, minor_rev);
    if(esp_flash_get_size(NULL, &flash_size) != ESP_OK) {
        printf("Get flash size failed");
        return;
    }

    printf("%" PRIu32 "MB %s flash\n", flash_size / (uint32_t)(1024 * 1024),
           (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

    printf("Minimum free heap size: %" PRIu32 " bytes\n", esp_get_minimum_free_heap_size());

    // for (int i = 10; i >= 0; i--) {
    //     printf("Restarting in %d seconds...\n", i);
    //     vTaskDelay(1000 / portTICK_PERIOD_MS);
    // }
    // printf("Restarting now.\n");
    // fflush(stdout);
    // esp_restart();

    // __uint8_t *apb_clk = (__uint8_t*)0x60026000U;

    // *apb_clk |= 1 << 11;
    // test_set_gpio();
    // test_tx_ram();

    // for (int i = 10; i >= 0; i--) {
    //     printf("Restarting in %d seconds...\n", i);
    //     vTaskDelay(1000 / portTICK_PERIOD_MS);
    // }
    //     printf("Restarting now.\n");
    // fflush(stdout);
    // esp_restart();

    // i2cStructure *i2c0 = I2C0;
    
    // __uint8_t sdaSampleLevel = 1;
    // __uint8_t ackLevel = 0;
    // __uint8_t isMaster = 1;
    // __uint8_t transmitLSBFirst = 0;
    // __uint8_t receiveLSBFirst = 0;
    // i2cInit(i2c0, sdaSampleLevel, ackLevel, isMaster, transmitLSBFirst, receiveLSBFirst);

    printf("Starting setup..,\n");

    i2cStructure *i2c = (i2cStructure*) I2C0;
    __uint32_t sdaPinNum = 8;
    __uint32_t sclPinNum = 9;

    mpu6050Controller mpu6050Ctrl;
    mpu6050Ctrl.i2c = i2c;
    mpu6050Ctrl.sdaPinNum = sdaPinNum;
    mpu6050Ctrl.sclPinNum = sclPinNum;

    init6050Module(&mpu6050Ctrl);

    while(true);
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
    __uint32_t bufferArr[] = {0x001UL, 0x002UL, 0x003UL, 0x04UL};
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