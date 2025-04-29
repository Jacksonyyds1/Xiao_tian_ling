/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025-04-28     cjt         
 */

#include <board.h>
#include <rtthread.h>
#include <rtdevice.h>

/* 全局I2C句柄 */
I2C_HandleTypeDef hi2c2;

/* I2C1初始化函数 - 基于CubeMX生成代码适配 */
static int rt_hw_i2c2_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
    
    /* 配置I2C时钟源 */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C2;
    PeriphClkInit.I2c2ClockSelection = RCC_I2C2CLKSOURCE_PCLK1;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
        rt_kprintf("I2C2 clock configuration failed\n");
        return -RT_ERROR;
    }
    
    /* 使能GPIO和I2C时钟 */
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_I2C2_CLK_ENABLE();
    
    /* 配置GPIO引脚 - 根据原理图图2显示的引脚 */
    /* I2C_SCL和I2C_SDA在原理图中是PB13和PB14，但我们需要确认实际使用的是I2C1还是I2C2 */
    /* 如果使用的是I2C1，需要查找正确的引脚 */
    GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_14;  // 假设I2C1的SCL=PB8, SDA=PB9
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;       // 开漏输出模式
    GPIO_InitStruct.Pull = GPIO_NOPULL;           // 无上拉(因为电路板上已有外部上拉电阻)
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C2;    // I2C1复用功能
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    
    /* I2C1配置 */
    hi2c2.Instance = I2C2;
    hi2c2.Init.Timing = 0x30909DEC;               // 直接使用CubeMX生成的Timing值
    hi2c2.Init.OwnAddress1 = 0;
    hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c2.Init.OwnAddress2 = 0;
    hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
    hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    
    if (HAL_I2C_Init(&hi2c2) != HAL_OK)
    {
        rt_kprintf("I2C2 initialization failed\n");
        return -RT_ERROR;
    }
    
    /* 配置模拟滤波器 */
    if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
    {
        rt_kprintf("I2C2 analog filter configuration failed\n");
        return -RT_ERROR;
    }
    
    /* 配置数字滤波器 */
    if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK)
    {
        rt_kprintf("I2C2 digital filter configuration failed\n");
        return -RT_ERROR;
    }
    
    rt_kprintf("I2C2 initialized successfully\n");
    return RT_EOK;
}

/* 注册到RT-Thread初始化序列 */
INIT_BOARD_EXPORT(rt_hw_i2c2_init);