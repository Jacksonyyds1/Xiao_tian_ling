/*
********************************************************************************
********************************************************************************
*/
PPDEF(PP_FLAG             , 1  , "170" )       /* 0XAA 标记位，如果是第一次伟初始化默认启动初始化数据 */
PPDEF(PP_PRD_PSN          , 64 , "00" )        /* 出厂序列号 */
PPDEF(PP_PRD_MODEL        , 16 , "YQ500" )     /* 产品型号 */
PPDEF(PP_PRD_FACTURER     , 20 , "YQ" )        /* 通讯协议 */
PPDEF(PP_PRD_BOARD        , 1  , "0" )         /* 厂家名称 */
                          
PPDEF(PP_RESERVE1         , 10 , "0" )         /* 预留长度  */
PPDEF(PP_RESERVE2         , 25 , "XXXXXXX" )   /* 预留长度 26字节*/
/*
********************************************************************************
********************************************************************************
*/
PPDEF(PP_ESIM_CCID0       , 64  , "0" )        /* 种子卡  */
PPDEF(PP_ESIM_CCID1       , 64  , "0" )        /* ESIM卡1 */
PPDEF(PP_ESIM_CCID2       , 64  , "0" )        /* ESIM卡2 */
                          
PPDEF(PP_BLE_NAME         , 32  , "DG500" ) 
PPDEF(PP_BLE_PIN          , 7   , "123456" ) 
                          
PPDEF(PP_WIFI_NAME        , 16  , "lwp" ) 
PPDEF(PP_WIFI_PWD         , 16  , "88889999" ) 

PPDEF(PP_PUB_TOPICS1      , 32  , "pubtopics1" ) 
PPDEF(PP_PUB_TOPICS2      , 32  , "pubtopics2" ) 
PPDEF(PP_PUB_TOPICS3      , 32  , "pubtopics" ) 

PPDEF(PP_SUB_TOPICS1      , 32  , "subtopics" ) 
PPDEF(PP_SUB_TOPICS2      , 32  , "subtopics" ) 
PPDEF(PP_SUB_TOPICS3      , 32  , "subtopics" ) 

PPDEF(PP_ITTE_MODE        , 1   , "0" ) 
                          
PPDEF(PP_ULOG_LVL         , 36  , "1" ) 
/*
********************************************************************************
********************************************************************************
*/
PPDEF(PP_DEVICE_ID        ,  64  , "4F0024594E501220363736" )              /* 设备编号 */

/* 通用 */                       
PPDEF(PP_MQT_PRO          ,  1   , "0" )                 /* 协议选择 */
PPDEF(PP_MQT_URL          ,  128 , "aicrdfmr9pphp-ats.iot.ap-east-1.amazonaws.com" ) /* IP地址 *//* a1qc8sz6uwatlh-ats.iot.us-east-1.amazonaws.com*/
PPDEF(PP_MQT_PORT         ,  2   , "8883" )              /* 端口号 */
PPDEF(PP_MQT_USERNAME     ,  128 , " " ) /* username */
PPDEF(PP_MQT_PASSWD       ,  64  , " " )        /* passwd */
PPDEF(PP_MQT_CLIENTID     ,  64  , "4F0024594E501220363736" )       /* clientId *//*客户端ID*/
PPDEF(PP_MQT_TICK         ,  2   , "60*6" )         /* heart tick */
/*
********************************************************************************
********************************************************************************
*/
PPDEF(PP_AIRPLANE_MODE    ,  1   , "0")            /* 飞行模式 */
PPDEF(PP_WORK_MODE        ,  1   , "0")            /* 工作模式 */
PPDEF(PP_START_TIME       ,  2   , "0")            /* 开始时间 */
PPDEF(PP_STOP_TIME        ,  2   , "0")            /* 结束时间 */
                                 
                                 
PPDEF(PP_GPS_MODE         ,  1   , "0")            /* GPS运行模式 */
PPDEF(PP_GPS_CYCLE        ,  1   , "0")            /* GPS循环类型 */
PPDEF(PP_GPS_PERIOD       ,  4   , "0")            /* GPS间隔时间 */
                                 
PPDEF(PP_GYR_MODE         ,  1   , "0")            /* GYR模式 */
PPDEF(PP_GYR_PERIOD       ,  4   , "0")            /* GYR间隔时间 */
/*
********************************************************************************
********************************************************************************
*/
PPDEF(PP_ELEFEN_OFFON    ,  1   , "0")            /* 电子围栏 开关 */
PPDEF(PP_ELEFEN_DISTANCE ,  4   , "500")          /* 电子围栏 范围 阈值*/
PPDEF(PP_ELEFEN_LONG     ,  8   , "0" )           /* 电子围栏 经度*/
PPDEF(PP_ELEFEN_LATI     ,  8   , "0" )           /* 电子围栏 维度*/
PPDEF(PP_RESERVE5        ,  50  , " " )           /* 预留长度 5字节*/
/*
********************************************************************************
********************************************************************************
*/
 /*
 **********************************************************************************
 *wifi 定位相关配置参数
 **********************************************************************************
 */
PPDEF(PP_WIFI_POS_ENABLE ,  1   , "0")            /* wifi 定位服务开关 */
PPDEF(PP_WIFI_POS_SCAN_INTERVAL,  2   , "300")          /* wifi 定位扫描间隔时间 */
PPDEF(PP_WIFI_POS_AUTO_REPORT,  1   , "0")          /* wifi 定位自动上报开关  0关闭 1开启*/
PPDEF(PP_WIFI_POS_MIN_RSSI    , 1   , "-90")         /* 最小信号强度阈值 */
PPDEF(PP_WIFI_POS_MAX_APS     , 1   , "20")          /* 最大扫描AP数量 */
PPDEF(PP_WIFI_POS_LOCATION_X  , 8   , "0.0")         /* 当前位置X坐标(米) */
PPDEF(PP_WIFI_POS_LOCATION_Y  , 8   , "0.0")         /* 当前位置Y坐标(米) */
PPDEF(PP_WIFI_POS_BUILDING_ID , 32  , "")            /* 建筑物ID */
PPDEF(PP_WIFI_POS_FLOOR_ID    , 16  , "")            /* 楼层ID */