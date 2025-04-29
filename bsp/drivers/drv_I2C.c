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

/* ȫ��I2C��� */
I2C_HandleTypeDef hi2c2;

/* I2C1��ʼ������ - ����CubeMX���ɴ������� */
static int rt_hw_i2c2_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
    
    /* ����I2Cʱ��Դ */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C2;
    PeriphClkInit.I2c2ClockSelection = RCC_I2C2CLKSOURCE_PCLK1;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
        rt_kprintf("I2C2 clock configuration failed\n");
        return -RT_ERROR;
    }
    
    /* ʹ��GPIO��I2Cʱ�� */
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_I2C2_CLK_ENABLE();
    
    /* ����GPIO���� - ����ԭ��ͼͼ2��ʾ������ */
    /* I2C_SCL��I2C_SDA��ԭ��ͼ����PB13��PB14����������Ҫȷ��ʵ��ʹ�õ���I2C1����I2C2 */
    /* ���ʹ�õ���I2C1����Ҫ������ȷ������ */
    GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_14;  // ����I2C1��SCL=PB8, SDA=PB9
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;       // ��©���ģʽ
    GPIO_InitStruct.Pull = GPIO_NOPULL;           // ������(��Ϊ��·���������ⲿ��������)
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C2;    // I2C1���ù���
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    
    /* I2C1���� */
    hi2c2.Instance = I2C2;
    hi2c2.Init.Timing = 0x30909DEC;               // ֱ��ʹ��CubeMX���ɵ�Timingֵ
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
    
    /* ����ģ���˲��� */
    if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
    {
        rt_kprintf("I2C2 analog filter configuration failed\n");
        return -RT_ERROR;
    }
    
    /* ���������˲��� */
    if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK)
    {
        rt_kprintf("I2C2 digital filter configuration failed\n");
        return -RT_ERROR;
    }
    
    rt_kprintf("I2C2 initialized successfully\n");
    return RT_EOK;
}

/* ע�ᵽRT-Thread��ʼ������ */
INIT_BOARD_EXPORT(rt_hw_i2c2_init);