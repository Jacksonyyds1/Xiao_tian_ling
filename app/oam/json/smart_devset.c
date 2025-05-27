/*
 * smart_devset.c
 *
 *  Created on: 2023年07月17日
 *      Author: lwp
 */
#include "app_lib.h"
#include "version.h"

/*
********************************************************************************
********************************************************************************
*/
#define DBG_TAG "smart"
#define DBG_LVL DBG_LOG 
#include <rtdbg.h>
 /*
 * dev-set 函数声明 
 */
#define ACTION_DEF(pname,operate)  
#define DEVGET_DEF(pname,operate)     
#define DEVSET_DEF(pname,operate) \
s32_t operate(cJSON* data,smart_msg_t *psmsg,u8_t offset,js_msg_t *to,void *arg);
#include "smart.def"
/*
********************************************************************************
********************************************************************************
*/

const struct _DEVSET_CMD
{      
    const char *pname;
    s32_t (*CmdAnaly)(cJSON* data,smart_msg_t *psmsg,u8_t offset,js_msg_t *to,void *arg);  /* 功能处理 */
} CmdSetTable[] = {
 
    /* cmdid, size,att,pmem,func*/
    #define ACTION_DEF(pname,operate)  
    #define DEVGET_DEF(pname,operate)     
    #define DEVSET_DEF(pname,operate)    {pname,operate},
    #include "smart.def"
};
/*
********************************************************************************
********************************************************************************
*/
#define DEF_SETCMD_FN(fn)  s32_t fn(cJSON* data,smart_msg_t *psmsg,u8_t offset,js_msg_t *to,void *arg)
/*
********************************************************************************
*Function    :  devset_respond 
*Description :  应答回复
*Input       :
*Output      :
*Return      :
*Others      :
********************************************************************************
*/
static s32_t devset_respond(const char *pcmd,int res,smart_msg_t *psmsg,js_msg_t *to,void *arg)
{
    cJSON* data;

    data = cJSON_CreateObject();

    cJSON_AddStringToObject(data,"cmd", pcmd);
 
    return smart_response_send("/dev/set",data,psmsg,res,to,arg);
}
/*
********************************************************************************
*Function    :  smart_heartbeat_set 
*Description :  心跳时间设置
*Input       :
*Output      :
*Return      :
*Others      :
********************************************************************************
*/
DEF_SETCMD_FN(smart_heartbeat_set)
{
    int  result=SMART_FAIL;
    cJSON* items;
    short tick;

    do{

        items = cJSON_GetObjectItem(data, "tick");
        if ( items == NULL ){
            LOG_E("%s tick ",__func__);
            break;
        } 

        tick = items->valueint;
          
        PPItemWrite(PP_MQT_TICK, (u8_t*)&tick,  PPItemSize(PP_MQT_TICK));   
        
        LOG_D("%s tick %d ",__func__,tick);
        
        result=SMART_OK;
        
    }while (0);
 
    return devset_respond(CmdSetTable[offset].pname,result,psmsg,to,arg);
}
/*
********************************************************************************
*Function    :  smart_airplane_set 
*Description :   
*Input       :
*Output      :
*Return      :
*Others      :
********************************************************************************
*/
DEF_SETCMD_FN(smart_airplane_set)
{
    int  result=SMART_FAIL;
    cJSON* items;
    u8_t mode;
    do{

        items = cJSON_GetObjectItem(data, "mode");
        if ( items == NULL || items->valuestring == NULL){
            LOG_E("%s tick ",__func__);
            break;
        } 
        
        LOG_D("%s mode %s ",__func__,items->valuestring);
        if(strcmp(items->valuestring, "on") == 0) {
            mode =1;
        }else if(strcmp(items->valuestring, "off") == 0) {
            mode =0;
        }
      
        PPItemWrite(PP_AIRPLANE_MODE, (u8_t*)&mode,  PPItemSize(PP_AIRPLANE_MODE));   
        
        result=SMART_OK;
    }while (0);
 
    return devset_respond(CmdSetTable[offset].pname,result,psmsg,to,arg);
}
/*
********************************************************************************
*Function    :  smart_work_mode 
*Description :   
*Input       :
*Output      :
*Return      :
*Others      :
********************************************************************************
*/
DEF_SETCMD_FN(smart_work_mode)
{
    int  result=SMART_FAIL;
    cJSON* items;
    u8_t mode;
    int res;
    rt_uint32_t hour,  minute;
    u8_t start_time[2];
    u8_t stop_time[2];
    
    do{

        items = cJSON_GetObjectItem(data, "mode");
        if ( items == NULL || items->valuestring == NULL){
            LOG_E("%s mode ",__func__);
            break;
        } 
        LOG_D("%s mode %s ",__func__,items->valuestring);
        if(strcmp(items->valuestring, "off") == 0) {
            mode =0;
        }else if(strcmp(items->valuestring, "on") == 0) {
            mode =1;
        }else if(strcmp(items->valuestring, "auto") == 0) {
            mode =2;
        }else{
            break;
        }

        items = cJSON_GetObjectItem(data, "start_time");
        if ( items == NULL || items->valuestring == NULL){
            LOG_E("%s start_time ",__func__);
            break;
        } 
        LOG_D("%s start_time %s ",__func__,items->valuestring);
        
        res =sscanf(items->valuestring,"%d-%d",&hour,&minute);
        
        if(res == 3) {
            start_time[0] = hour;
            start_time[1] = minute;
        }
        
        items = cJSON_GetObjectItem(data, "stop_time");
        if ( items == NULL || items->valuestring == NULL){
            LOG_E("%s stop_time ",__func__);
            break;
        } 
        LOG_D("%s stop_time %s ",__func__,items->valuestring);
        res =sscanf(items->valuestring,"%d-%d",&hour,&minute);
        if(res == 3) {
            stop_time[0] = hour;
            stop_time[1] = minute;
        }
        
        PPItemWrite(PP_WORK_MODE, (u8_t*)&mode,       PPItemSize(PP_WORK_MODE));   
        PPItemWrite(PP_START_TIME,(u8_t*)&start_time, PPItemSize(PP_START_TIME));   
        PPItemWrite(PP_STOP_TIME, (u8_t*)&stop_time,  PPItemSize(PP_STOP_TIME));   
      
        result=SMART_OK;
    }while (0);
 
    return devset_respond(CmdSetTable[offset].pname,result,psmsg,to,arg);
}
/*
********************************************************************************
*Function    :  smart_time_set 
*Description :   
*Input       :
*Output      :
*Return      :
*Others      :
********************************************************************************
*/
DEF_SETCMD_FN(smart_time_set)
{
    int  result=SMART_FAIL;
    cJSON* items;
    int res;
    
    do{

        items = cJSON_GetObjectItem(data, "mode");
        if ( items == NULL || items->valuestring == NULL){
            LOG_E("%s tick ",__func__);
            break;
        } 
        LOG_D("%s mode %s ",__func__,items->valuestring);
        
        items = cJSON_GetObjectItem(data, "time");
        if ( items == NULL || items->valuestring == NULL){
            LOG_E("%s time ",__func__);
            break;
        } 
        LOG_D("%s time %s ",__func__,items->valuestring);
        rt_uint32_t year,  month,  day;
        rt_uint32_t hour,  minute,  second;
        
        res =sscanf(items->valuestring,"%d-%d-%d %d-%d-%d",&year,&month,&day,&hour,&minute,&second);
        
        if(res == 6) {
            set_date(year, month, day);
        
            set_time(hour, minute, second);
        
            result=SMART_OK;
        }

    }while (0);
 
    return devset_respond(CmdSetTable[offset].pname,result,psmsg,to,arg);
}

/*
********************************************************************************
*Function    :  smart_led_set 
*Description :   
*Input       :
*Output      :
*Return      :
*Others      :
********************************************************************************
*/
DEF_SETCMD_FN(smart_led_set)
{
    int  result=SMART_FAIL;
    cJSON* items;

    do{

        items = cJSON_GetObjectItem(data, "switch");
        if ( items == NULL || items->valuestring == NULL){
            LOG_E("%s switch ",__func__);
            break;
        } 
        LOG_D("%s switch %s ",__func__,items->valuestring);
        if(strcmp(items->valuestring, "off") == 0) {
           
        }else if(strcmp(items->valuestring, "on") == 0) {
           
        }
        result=SMART_OK;
    }while (0);
 
    return devset_respond(CmdSetTable[offset].pname,result,psmsg,to,arg);
}
/*
********************************************************************************
*Function    :  smart_gps_set 
*Description :   
*Input       :
*Output      :
*Return      :
*Others      :
********************************************************************************
*/
DEF_SETCMD_FN(smart_gps_set)
{
    int  result=SMART_FAIL;
    cJSON* items;
    u8_t mode,cycle;
    u16_t period;
    
    do{

        items = cJSON_GetObjectItem(data, "mode");
        if ( items == NULL || items->valuestring == NULL){
            LOG_E("%s mode ",__func__);
            break;
        } 
        LOG_D("%s mode %s ",__func__,items->valuestring);
        if(strcmp(items->valuestring, "off") == 0) {
           
        }else if(strcmp(items->valuestring, "on") == 0) {
           
        }
        items = cJSON_GetObjectItem(data, "cycle");
        if ( items == NULL || items->valuestring == NULL){
            LOG_E("%s cycle ",__func__);
            break;
        } 
        LOG_D("%s cycle %s ",__func__,items->valuestring);

        items = cJSON_GetObjectItem(data, "period");
        if ( items == NULL ){
            LOG_E("%s period ",__func__);
            break;
        } 
        LOG_D("%s period %d ",__func__,items->valueint);
        period = items->valueint;
        PPItemWrite(PP_GPS_MODE, (u8_t*)&mode,  PPItemSize(PP_GPS_MODE));   
        PPItemWrite(PP_GPS_CYCLE,     (u8_t*)&cycle,  PPItemSize(PP_GPS_CYCLE));  
        PPItemWrite(PP_GPS_PERIOD,    (u8_t*)&period,  PPItemSize(PP_GPS_PERIOD));  
        
#if 0
        items = cJSON_GetObjectItem(data, "start_time");
        if ( items == NULL || items->valuestring == NULL){
            LOG_E("%s start_time ",__func__);
            break;
        } 
        LOG_D("%s start_time %s ",__func__,items->valuestring);
 
        items = cJSON_GetObjectItem(data, "stop_time");
        if ( items == NULL || items->valuestring == NULL){
            LOG_E("%s stop_time ",__func__);
            break;
        } 
        LOG_D("%s stop_time %s ",__func__,items->valuestring);
#endif  
        result=SMART_OK;
    }while (0);
 
    return devset_respond(CmdSetTable[offset].pname,result,psmsg,to,arg);
}
/*
********************************************************************************
*Function    :  smart_gyroscope_set 
*Description :   
*Input       :
*Output      :
*Return      :
*Others      :
********************************************************************************
*/
DEF_SETCMD_FN(smart_gyroscope_set)
{
    int  result=SMART_FAIL;
    cJSON* items;
    u8_t mode;
    u16_t period;
    
    do{

        items = cJSON_GetObjectItem(data, "mode");
        if ( items == NULL || items->valuestring == NULL){
            LOG_E("%s mode ",__func__);
            break;
        } 
        LOG_D("%s mode %s ",__func__,items->valuestring);
        if(strcmp(items->valuestring, "off") == 0) {
            mode = 0;
        }else if(strcmp(items->valuestring, "on") == 0) {
            mode = 1;
        }
        
        items = cJSON_GetObjectItem(data, "period");
        if ( items == NULL ){
            LOG_E("%s period ",__func__);
            break;
        } 
        LOG_D("%s period %d ",__func__,items->valueint);
        period = items->valueint;
        PPItemWrite(PP_GYR_MODE,  (u8_t*)&mode,  PPItemSize(PP_GYR_MODE));   
        PPItemWrite(PP_GYR_PERIOD,(u8_t*)&period,  PPItemSize(PP_GYR_PERIOD));  
        
        result=SMART_OK;
    }while (0);
 
    return devset_respond(CmdSetTable[offset].pname,result,psmsg,to,arg);
}
/*
********************************************************************************
*Function    :  smart_gyroscope_set 
*Description :   
*Input       :
*Output      :
*Return      :
*Others      :
********************************************************************************
*/
DEF_SETCMD_FN(smart_elecfence_set)
{
    int  result=SMART_FAIL;
    cJSON* items;
    u8_t value;
    u32_t distance;
    do{

        items = cJSON_GetObjectItem(data, "switch");
        if ( items == NULL || items->valuestring == NULL){
            LOG_E("%s switch ",__func__);
            break;
        } 
        LOG_D("%s switch %s ",__func__,items->valuestring);
        if(strcmp(items->valuestring, "off") == 0) {
            value = 0;
        }else if(strcmp(items->valuestring, "on") == 0) {
            value = 1;
        }
        
        //PP_ELEFEN_OFFON
        items = cJSON_GetObjectItem(data, "distance");
        if ( items == NULL || items->valuestring == NULL){
            LOG_E("%s distance ",__func__);
            break;
        } 
        LOG_D("%s distance %s ",__func__,items->valuestring);
        
        distance = items->valueint ;
        PPItemWrite(PP_ELEFEN_OFFON,  (u8_t*)&value,  PPItemSize(PP_ELEFEN_OFFON));   
        PPItemWrite(PP_ELEFEN_DISTANCE,(u8_t*)&distance,  PPItemSize(PP_ELEFEN_DISTANCE));  
        
        result=SMART_OK;
    }while (0);
 
    return devset_respond(CmdSetTable[offset].pname,result,psmsg,to,arg);
}
/*
********************************************************************************
*Function    :  smart_restore 
*Description :  恢复出厂化设置 OK
*Input       :
*Output      :
*Return      :
*Others      :
********************************************************************************
*/
DEF_SETCMD_FN(smart_restore)
{
    int  result=SMART_OK;

    do{
        LOG_D("%s ",__func__);
        PPItemsFactory();
    }while (0);

    return devset_respond(CmdSetTable[offset].pname,result,psmsg,to,arg);
}
/*
********************************************************************************
*Function    :  smart_reboot 
*Description :  系统重启 OK
*Input       :
*Output      :
*Return      :
*Others      :
********************************************************************************
*/
DEF_SETCMD_FN(smart_reboot)
{
    int  result=SMART_OK;

    do
    {
        syswatch_reboot();
        LOG_D("%s ",__func__);
    } while (0);

    return devset_respond(CmdSetTable[offset].pname,result,psmsg,to,arg);
}
/*
********************************************************************************
*Function    :  smart_mqtt_set 
*Description :  mqtt配置 
*Input       :
*Output      :
*Return      :
*Others      :
********************************************************************************
*/
DEF_SETCMD_FN(smart_mqtt_set)
{
    int  result=SMART_FAIL;
    cJSON* items;
    short port;
    char *host,*username,*password;

    do{

        items = cJSON_GetObjectItem(data, "host");
        if ( items == NULL || items->valuestring == NULL){
            LOG_E("%s host ",__func__);
            break;
        } 
        host = items->valuestring;
        items = cJSON_GetObjectItem(data, "port");
        if ( items == NULL ){
            LOG_E("%s port ",__func__);
            break;
        } 
        port = items->valueint;
        items = cJSON_GetObjectItem(data, "username");
        if ( items == NULL || items->valuestring == NULL){
            LOG_E("%s username ",__func__);
            break;
        } 
        username = items->valuestring;
        items = cJSON_GetObjectItem(data, "password");
        if ( items == NULL || items->valuestring == NULL){
            LOG_E("%s password ",__func__);
            break;
        }  
        password = items->valuestring;
    
        result=SMART_OK;
        PPItemWrite(PP_MQT_URL, (u8_t*)host,  PPItemSize(PP_MQT_URL));  
        PPItemWrite(PP_MQT_PORT, (u8_t*)&port,  PPItemSize(PP_MQT_PORT));  
        PPItemWrite(PP_MQT_USERNAME, (u8_t*)username,  PPItemSize(PP_MQT_USERNAME));   
        PPItemWrite(PP_MQT_PASSWD, (u8_t*)password,  PPItemSize(PP_MQT_PASSWD));   

    }while (0);
 
    return devset_respond(CmdSetTable[offset].pname,result,psmsg,to,arg);
}
/*
********************************************************************************
*Function    :  smart_findme 
*Description :  find me
*Input       :
*Output      :
*Return      :
*Others      :
********************************************************************************
*/
DEF_SETCMD_FN(smart_findme) 
{
    int  result=SMART_OK;

    do
    {
    
    } while (0);
   

    return devset_respond(CmdSetTable[offset].pname,result,psmsg,to,arg);
}
/*
********************************************************************************
*Function    :  smart_ota_start 
*Description :   
*Input       :
*Output      :
*Return      :
*Others      :
********************************************************************************
*/
DEF_SETCMD_FN(smart_ota_start) 
{
    cJSON* items;
    int  result=SMART_FAIL;

    do
    {
        items = cJSON_GetObjectItem(data, "url");
        if ( items == NULL || items->valuestring == NULL){
            LOG_E("%s url ",__func__);
            break;
        }
        LOG_D("%s url: %s",__func__,items->valuestring);

 
    } while (0);

    return devset_respond(CmdSetTable[offset].pname,result,psmsg,to,arg);
}
/*
********************************************************************************
*Function    :  smart_fw_rollback 
*Description :  
*Input       :
*Output      :
*Return      :
*Others      :
********************************************************************************
*/
DEF_SETCMD_FN(smart_fw_rollback) 
{
    cJSON* items;
    int  result=SMART_FAIL;

    do
    {
        items = cJSON_GetObjectItem(data, "key");
        if ( items == NULL || items->valuestring == NULL){
            LOG_E("%s key ",__func__);
            break;
        }
        LOG_D("%s key: %s",__func__,items->valuestring);
        
        ota_version_rollback();
 
    } while (0);

    return devset_respond(CmdSetTable[offset].pname,result,psmsg,to,arg);
}

/*
********************************************************************************
*Function    :  smart_card_set 
*Description :  
*Input       :
*Output      :
*Return      :
*Others      :
********************************************************************************
*/
DEF_SETCMD_FN(smart_card_set) 
{
    cJSON* items;
    int  result=SMART_FAIL;

    do
    {
        items = cJSON_GetObjectItem(data, "key");
        if ( items == NULL || items->valuestring == NULL){
            LOG_E("%s key ",__func__);
            break;
        }
        LOG_D("%s key: %s",__func__,items->valuestring);

 
    } while (0);

    return devset_respond(CmdSetTable[offset].pname,result,psmsg,to,arg);
}
/*
********************************************************************************
*Function    :  devset_analy 
*Description :  设备设置系列解析
*Input       :
*Output      :
*Return      :
*Others      :
********************************************************************************
*/
s32_t devset_analy(cJSON* payload,smart_msg_t *psmsg,js_msg_t *to,void *arg)
{
    cJSON* data;
    cJSON* cmd;

    data = cJSON_GetObjectItem(payload, "data");
    if ( data == NULL){
        LOG_E("%s data ",__func__);
        return 0;
    }
    
    cmd = cJSON_GetObjectItem(data, "cmd");
    if ( cmd == NULL || cmd->valuestring == NULL){
        LOG_E("%s cmd ",__func__);
        return 0;
    }

    for(int i=0 ; i < OS_COUNTOF(CmdSetTable) ; i++ ){
        if( strcmp(cmd->valuestring,CmdSetTable[i].pname)==0){
            if(CmdSetTable[i].CmdAnaly){
                CmdSetTable[i].CmdAnaly(data,psmsg,i,to,arg);
                LOG_I("%s name %s",__func__,cmd->valuestring);
                return 1;
            }
        }
    }
    LOG_E("%s Unable to find command [%s]",__func__,cmd->valuestring);
    return 0;
}
/*
**************************************************************************************
*Function    :  smart_wifi_positioning_set
*Description :  wifi定位设置
*Input       :  
*Output      :
*Return      : 
*Others      : 处理来自服务器的定位配置指令 27-05-2025 cjt add
**************************************************************************************
*/
DEF_SETCMD_FN(smart_wifi_positioning_set)
{
    int result=SMART_FAIL;
    cJSON* items;
    u8_t enable;
    u16_t scan_interval;
    u8_t auto_report;

    do{
        // 解析启用状态
        items = cJSON_GetObjectItem(data, "enable");
        if (items == NULL || items->valuestring == NULL) {
            LOG_E("%s enable field missing ", __func__);
            break;
        }
        if (strcmp(items->valuestring, "on") == 0) {
            enable = 1; // 启用
        } else if (strcmp(items->valuestring, "off") == 0) {
            enable = 0; // 禁用
        } else {
            LOG_E("%s invalid enable value:%s ", __func__, items->valuestring);
            break;
        }
        // 解析扫描间隔
        items = cJSON_GetObjectItem(data, "scan_interval");
        if (items == NULL) {
            scan_interval = 300; // 默认值为300秒
        }else {
            scan_interval = items->valueint;
            if(scan_interval < 60) scan_interval = 60; // 最小值为60秒

        }
        // 解析自动上报状态
        items = cJSON_GetObjectItem(data, "auto_report");
        if(items == NULL || items->valuestring == NULL) {
            LOG_E("%s auto_report field missing ", __func__);
            auto_report = 0; // 默认值为0
            break;
        }else{
            auto_report = (strcmp(items->valuestring, "on") == 0) ? 1 : 0; // 默认值为0
        }
        // 保存配置
        PPItemWrite(PP_WIFI_POS_ENABLE, (u8_t*)&enable, PPItemSize(PP_WIFI_POS_ENABLE));
        PPItemWrite(PP_WIFI_POS_SCAN_INTERVAL, (u8_t*)&scan_interval, PPItemSize(PP_WIFI_POS_SCAN_INTERVAL));
        PPItemWrite(PP_WIFI_POS_AUTO_REPORT, (u8_t*)&auto_report, PPItemSize(PP_WIFI_POS_AUTO_REPORT));

        // 启动或停止定位服务
        if(enable){
            wifi_positioning_service_start(scan_interval);
        }else{
            wifi_positioning_service_stop();
        }

        LOG_D("WiFi positioning set: enable=%d, scan_interval=%d, auto_report=%d",
              enable, scan_interval, auto_report);
        result = SMART_OK;
    }while(0);
    return devset_respond(CmdSetTable[offset].pname, result, psmsg, to, arg);
}
/*
*****************************************************************************************************
*Function    :  smart_wifi_positioning_trigger
*Description :  手动触发wifi定位
*Input       :
*Output      :
*Return      :
*Others      : 处理来自服务器的手动触发扫描指令 27-05-2025 cjt add
*****************************************************************************************************
*/
DEF_SETCMD_FN(smart_wifi_positioning_trigger)
{
    int result = SMART_FAIL;
    
    LOG_D("Manual WiFi positioning scan triggered");
        // 执行扫描
        if (esp_wifi_positioning_scan() == 0) {
            result = SMART_OK;
            LOG_D("WiFi positioning scan completed");
        } else {
            LOG_E("WiFi positioning scan failed");
        }
   

    return devset_respond(CmdSetTable[offset].pname, result, psmsg, to, arg);
}
/*
**************************************************************************************
*Function    :  smart_wifi_positioning_get
*Description :  获取wifi定位配置
*Input       :
*Output      :
*Return      :
*Others      :  27-05-2025 cjt add
**************************************************************************************
*/
DEF_SETCMD_FN(smart_wifi_positioning_get)
{
    cJSON* subdata;
    u8_t enable,auto_report;
    u16_t scan_interval;

    subdata = cJSON_CreateObject();

    cJSON_AddStringToObject(subdata, "cmd", CmdSetTable[offset].pname);

    // 读取配置
    PPItemRead(PP_WIFI_POS_ENABLE, (u8_t*)&enable, PPItemSize(PP_WIFI_POS_ENABLE));
    PPItemRead(PP_WIFI_POS_SCAN_INTERVAL, (u8_t*)&scan_interval, PPItemSize(PP_WIFI_POS_SCAN_INTERVAL));
    PPItemRead(PP_WIFI_POS_AUTO_REPORT, (u8_t*)&auto_report, PPItemSize(PP_WIFI_POS_AUTO_REPORT));

    cJSON_AddStringToObject(subdata, "enable", enable ? "on" : "off");
    cJSON_AddNumberToObject(subdata, "scan_interval", scan_interval);
    cJSON_AddStringToObject(subdata, "auto_report", auto_report ? "on" : "off");

    return smart_response_send("/dev/get", subdata, psmsg, SMART_OK, to, arg);

}
/*
*******************************************************************************************
*wifi 定位服务管理
*******************************************************************************************
*/
static rt_timer_t wifi_pos_timer = RT_NULL;
static u8_t wifi_pos_service_running = 0;


/*
*******************************************************************************************
*Function    :  wifi_positioning_timer_callback
*Description :  wifi定位定时器回调函数
*Input       :  
*Output      :
*Return      :
*Others      :  27-05-2025 cjt add
*******************************************************************************************
*/
static void wifi_positioning_timer_callback(void *parameter)
{

    if(wifi_pos_service_running){
        esp_wifi_positioning_scan();
    }
}
/*
*******************************************************************************************
*Function    :  wifi_positioning_service_start
*Description :  启动wifi定位服务
*Input       :
*Output      :
*Return      : 0 - 成功 -1 - 失败
*Others      :  27-05-2025 cjt add
*******************************************************************************************
*/
int wifi_positioning_service_start(u16_t scan_interval)
{
    if(wifi_pos_timer == RT_NULL){
        wifi_pos_timer = rt_timer_create("wifi_pos", wifi_positioning_timer_callback, RT_NULL, scan_interval * RT_TICK_PER_SECOND, RT_TIMER_FLAG_PERIODIC);
        
        if(wifi_pos_timer == RT_NULL){
            LOG_E("Failed to create WiFi positioning timer");
            return -1;
        }
    }

    // 更新定时器周期
    rt_timer_control(wifi_pos_timer, RT_TIMER_CTRL_SET_TIME, (void *)(scan_interval * RT_TICK_PER_SECOND));
    wifi_pos_service_running = 1;
    rt_timer_start(wifi_pos_timer);
    LOG_D("WiFi positioning service started with interval %d seconds", scan_interval);
    return 0;
}
/*
***********************************************************************************************************
*Function    :  wifi_positioning_service_stop
*Description :  停止wifi定位服务
*Input       :
*Output      :
*Return      : 
*Others      :  27-05-2025 cjt add
***********************************************************************************************************
*/
void wifi_positioning_service_stop(void)
{

    wifi_pos_service_running = 0;
    if(wifi_pos_timer != RT_NULL){
        rt_timer_stop(wifi_pos_timer);
    }
    LOG_D("WiFi positioning service stopped");
}
/*
**********************************************************************************************************
*Function    :  wifi_positioning_service_init
*Description :  初始化wifi定位服务
*Input       :
*Output      :
*Return      : 
*Others      : 系统启动时调用  27-05-2025 cjt add
**********************************************************************************************************
*/
int wifi_positioning_service_init(void)
{
    u8_t enable;
    u16_t scan_interval;

    // 读取配置
    PPItemRead(PP_WIFI_POS_ENABLE, (u8_t*)&enable, PPItemSize(PP_WIFI_POS_ENABLE));
    PPItemRead(PP_WIFI_POS_SCAN_INTERVAL, (u8_t*)&scan_interval, PPItemSize(PP_WIFI_POS_SCAN_INTERVAL));

    // 如果启用，启动定位服务
    if(enable && scan_interval >0){
       return wifi_positioning_service_start(scan_interval);   
    }
    return 0;
}