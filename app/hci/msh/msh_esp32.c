
/*
 * lan_port.c
 *
 *  Created on: 2019��5��9��
 *      Author: root
 */
#include <board.h>
#include <rtthread.h>
#include "app_lib.h"
#include "esp_cmd.h"



#define DBG_TAG "misc"
#define DBG_LVL DBG_LOG   
#include <rtdbg.h>


/*
********************************************************************************
********************************************************************************
*/

/*
********************************************************************************
********************************************************************************
*/ 
/*
********************************************************************************
Function      : show_usage
Description   :  
Input         :
Output        :
Return        :
Others        :
********************************************************************************
*/
static void show_usage(void)
{
     rt_kprintf("Please input '<factory|heart|worktime >' \n"); 
}
/*
********************************************************************************
Function      : esp_cmd
Description   :   A5 04 00 00 12 01 17 5A
Input         :
Output        :
Return        :
Others        :
********************************************************************************
*/
void esp_cmd(int argc, char** argv)
{
    if (argc < 2 ) {
        show_usage();
        return;
    }

    if(strcmp(argv[1], "bleinit") == 0) {
        esp_btser_init();
    }else if(strcmp(argv[1], "mqtt") == 0) {
        if(strcmp(argv[2], "pub") == 0) {
            esp_mqtt_send("hellword",strlen("hellword"));
        }
        
    }else if(strcmp(argv[1], "http") == 0) {
        if(strcmp(argv[2], "download") == 0) {
            https_download("http://ir.hongwaimaku.com/vipdownloadpar.php?kfid=020677&mac=ad993deffed32786&tokens=biPv8NNfefYc0c5f4724aY1d1aP2Ao4Y","/irdata.bin");
        }else if(strcmp(argv[2], "start") == 0) {
            https_service_init();
        }
    }else if(strcmp(argv[1], "wifi_connect")==0){

        char ssid[32] = {0};
        char pwd[16] = {0};

         // 从永久存储中读取WiFi配置
        PPItemRead(PP_WIFI_NAME, ssid, PPItemSize(PP_WIFI_NAME));
        PPItemRead(PP_WIFI_PWD, pwd, PPItemSize(PP_WIFI_PWD));
        //设置wifi参数
        esp_set_wifi(ssid, pwd);
         //初始化wifi连接
        esp_set_function(__esp_wifi_sta_init,0,0);

          //等待连接完成
        int retry = 0;
        while(!esp_is_initok() && retry < 10){

        rt_kprintf("wating for wifi connect...\n");
        rt_thread_delay(1000);
        retry++;

    }
    if(esp_is_initok()){

      rt_kprintf("wifi connect success\n!");
    }else{
        rt_kprintf("wifi connect fail\n");
    }

    }
}   
MSH_CMD_EXPORT(esp_cmd,esp_cmd); 
