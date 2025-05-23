 /*
 *  app_led.h
 *
 *  Created on: 2023年07月17日
 *      Author: lwp
 */
#ifndef __APP_LED_H_
#define __APP_LED_H_

 
#include "app_gpio.h"
/*
********************************************************************************
********************************************************************************
*/ 
typedef enum
{
    LED_CONN2INTER =0,  /*连接互联网*/
    LED_CONNOFINTER =1, /*已连接互联网*/
    LED_BATTERLOWER , /*电池电量过低*/
    LED_POWERON     ,/*设备开机*/
    LED_CHARGING    ,/*充电正在进行*/
    LED_CHARGEFULL,/*设备充满电*/
    LED_POWEROFF  ,/*设备关机过程*/
    LED_SUITOFDOG ,/*适合狗*/
    LED_NOTOFDOG  ,/*不适合狗或不适合狗*/
}LED_TYPE;
/*
********************************************************************************
********************************************************************************
*/ 

#endif
/*__APP_LED_H_ */