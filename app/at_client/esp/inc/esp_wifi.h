#ifndef __ESP_WIFI_H
#define __ESP_WIFI_H

#include <time.h>
#include "type.h"

/*
********************************************************************************
*wifi 热点信息结构体  27-05-2025
********************************************************************************
*/ 
typedef struct{
    char ssid[33];       // 热点名称
    char bssid[18];      // Mac地址
    int rssi;          // 信号强度
    int channel;       // 信道
    int auth_mode;     // 加密方式
    time_t scan_time;  // 扫描时间戳
} wifi_ap_info_t;
/*
********************************************************************************
*wifi 扫描结果结构体  27-05-2025
********************************************************************************
*/
typedef struct{
    wifi_ap_info_t aps[20]; // 存储最多20个热点信息
    int count;              //实际扫描到的热点数量
    double latitude;         // 当前位置纬度（如果有GPS）
    double longitude;        // 当前位置经度（如果有GPS）
    char device_id[33]; // 设备ID
} wifi_scan_result_t;

// 函数声明
int esp_wifi_position_scan(void);
int esp_cwlap_scan_all(wifi_scan_result_t *result);
int wifi_positioning_report(wifi_scan_result_t *result);
int smart_wifi_positioning_report(wifi_scan_result_t *result);
int wifi_positioning_service_start(u16_t scan_interval);
void wifi_positioning_service_stop(void);
int wifi_positioning_service_init(void);
#endif /*__ESP_WIFI_H */