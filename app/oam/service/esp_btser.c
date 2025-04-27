/*
 * esp_btser.c
 *
 *  Created on: 2023年08月24日
 *      Author: lwp
 */
#include <stdio.h>
#include <string.h>
#include <board.h>
#include <rtthread.h>
#include "type.h"
#include "bt_service.h" 
#include "oam_thread.h" 
#include "at.h"
#include "at_client.h"
#include "esp.h"
#include "esp_bt.h"   
#include "PPItems.h"
#include "lan.h"

#define DBG_TAG "esp"
#define DBG_LVL DBG_LOG   //DBG_LOG
#include <rtdbg.h>

extern esp_at_t esp;
/*
********************************************************************************
********************************************************************************
*/ 
 
typedef struct  {
    int sd;
    int connect;
    char name[64];
    char pin[7];
} bt_ser_t;


static lan_msg_t BleMsg;
static bt_ser_t BtSer; 
#define get_ble_service()   (&BtSer)
/*
********************************************************************************
********************************************************************************
*/ 
/*
********************************************************************************
*Function    : esp_btser_send
*Description : 蓝牙发送接口函数
*Input       :
*Output      :
*Return      :
*Others      :
********************************************************************************
*/
int esp_btser_send(u8_t *msg, u32_t msglen, void *arg)
{    
    bt_ser_t *service = get_ble_service();
    
    LOG_HEX("ble-send",16,(u8_t *)msg,msglen);
    rt_kprintf("esp_btser_send data\n");
    rt_kprintf("Current mode: %s\n", esp_ls_atmode() == ESP_MODE_SPP ? "SPP" : "AT");
    return esp_ble_send(service->sd, (void *)msg, msglen, 0);
}
/*
********************************************************************************
*Function    : esp_btser_recv_process
*Description :
*Input       :
*Output      :
*Return      :
*Others      :
********************************************************************************
*/   
static int esp_btser_recv_process(void *arg)
{    
    at_client_t client=(at_client_t)arg;
    unsigned char ch;
    //25-04-25 cjt modify
//    if( esp_ls_atmode() == ESP_MODE_SPP ){
//        return -1;
//    }

    while( at_getchar(client, &ch, 1000, 1) == 0){
        rt_kprintf("0X%02X ",ch);
        lan_msg_recv( &BleMsg, ch);
    }
    return 0;
}
/*
********************************************************************************
*Function    : esp_btser_create
*Description :
*Input       :
*Output      :
*Return      : 0 if success, -1 if fail.
*Others      :
********************************************************************************
*/
static int esp_btser_create(bt_ser_t *service)
{
    if (service->sd < 0) {
        service->sd = esp_socket_create(SOCK_BLE);
        esp_set_recv(service->sd, 0);
        BleMsg.from = esp_btser_send;
        service->connect = 1;
    }

    return 0;
}
/*
********************************************************************************
*Function    : esp_btser_disable
*Description :
*Input       :
*Output      :
*Return      : 0 if success, -1 if fail.
*Others      :
********************************************************************************
*/
void esp_btser_disable(void)
{
    bt_ser_t *service = get_ble_service();
    if (service->sd >= 0) {
        service->sd = -1;
    }
}
/*
********************************************************************************
*Function    :  esp_btser_init
*Description :
*Input       :
*Output      :
*Return      :
*Others      : 
********************************************************************************
*/
int esp_btser_init(void)
{    
    bt_ser_t *service = get_ble_service();
    
    PPItemRead(PP_BLE_NAME,service->name,PPItemSize(PP_BLE_NAME));
    
    PPItemRead(PP_BLE_PIN,service->pin,PPItemSize(PP_BLE_PIN));
    
    service->sd = -1;
    
    esp_btser_create(service);
    
    esp_ble_init(service->name,service->pin,esp_btser_recv_process);
    
    return 0;
}
/****************************************************************************
 * Function    : esp_ble_test_send
 * Description : 蓝牙连接spp模式发送函数
 * Input       : msg: 发送数据
 *               msglen: 发送数据长度
 * author       : cjt
 ****************************************************************************/
static int esp_ble_test_send(const char *msg, int msglen)
{
    if(esp_ls_atmode() != ESP_MODE_SPP){
        rt_kprintf("Error: Not in spp mode!\n");
        return -1;
    }

    rt_device_t uart_device = esp.client->device;
    if (uart_device == RT_NULL) {
        rt_kprintf("Error: uart device is null!\n");
        return -2;
    }

    rt_size_t written = rt_device_write(uart_device,0,msg,msglen);

    if(written != msglen)
    {
        rt_kprintf("warning: only %d bytes of %d bytes sent\n", written, msglen);
        return -3;
    }
     return written;  
}
/************************************************************************************
 * Function    : send_test_data
 * Description : 发送测试数据函数
 * Input       : msg: 发送数据
 *             msglen: 发送数据长度
 * author       : cjt
 ************************************************************************************/
void send_test_data(void)
{
    const char *test_msg = "Hello from BLE spp mode!\r\n";
    int result;

    if(esp_ls_atmode() == ESP_MODE_SPP)
    {
        result = esp_ble_test_send(test_msg, strlen(test_msg)+2);
        if(result > 0)
        {
            rt_kprintf("successfully sent %d bytes in spp mode\n",result);
        }
        else
        {
            rt_kprintf("Failed to send test data, error code: %d\n", result);

        }

    }else
    {

        rt_kprintf("NOT in spp mode, cannot send data\n");
    }

}
MSH_CMD_EXPORT(send_test_data, send test data via BLE spp);

/********************************************************************************
*Function    :  cmd_test_lan_protocol
*Description :  测试函数解析蓝牙数据包
*Input       :
*Output      :  
*Return      :
*Others      :
*author       : cjt
********************************************************************************/

void cmd_test_lan_protocol(int argc, char** argv)
{
    // 构造测试消息，注意完整包含帧尾
    u8_t test_msg[] = {0xA5, 0x03, 0x01, 0x00, 0x03, 0x42, 0x45, 0x5A};
    
    // 调用解析函数
    lan_analy(test_msg, sizeof(test_msg), BleMsg.from, NULL);
}
MSH_CMD_EXPORT(cmd_test_lan_protocol, Test LAN protocol parser);
