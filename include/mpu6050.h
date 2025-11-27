#include <stdio.h>
#include "i2c.h"

#define MPU_Address 0x68UL

#define MPU_6050_PWR_MGMT_1_Reg 0x6BUL

#define MPU_6050_Accel_Cfg_Reg 0x1CUL

#define MPU_6050_X_Accel_High_Reg 0x3BUL
#define MPU_6050_X_Accel_Low_Reg 0x3CUL

#define MPU_6050_Y_Accel_High_Reg 0x3DUL
#define MPU_6050_Y_Accel_Low_Reg 0x3EUL

#define MPU_6050_Z_Accel_High_Reg 0x3FUL
#define MPU_6050_Z_Accel_Low_Reg 0x40UL

#define MPU_6050_Who_Am_I_Reg 0x75UL

typedef struct
{
    i2cStructure *i2c;
    __uint32_t sdaPinNum;
    __uint32_t sclPinNum;
}mpu6050Controller;

void init6050Module(mpu6050Controller *mpu6050Ctrl);

