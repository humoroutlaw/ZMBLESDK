//
//  ZMProtocolCmd.h
//  ZMKMBleSDK
//
//

#ifndef ZMProtocolCmd_h
#define ZMProtocolCmd_h

#import <Foundation/Foundation.h>

typedef struct {
    char    password[4];        // 比如 '1234'，无0结尾
} ZMDynamicPasswordType;

typedef struct {
    char    password[6];        // '112233'，无0结尾
} ZMPasswordType;

typedef struct {
    char    password[6];    //管理员账号，无0结尾
    char    serial[4];      //服务器产生的该管理员账号下名下的智能锁编号序号，比如 '1234'，无0结尾
} ZMAdminIdType;

// 密码等辅助函数
NSString *ZMBcdTypeToStr(void *src, size_t len);
void ZMBcdTypeFromStr(NSString *src, void *dst, size_t dstlen);

NSString *ZMAdminIdTypeToStr(ZMAdminIdType aid);
ZMAdminIdType ZMAdminIdTypeFromStr(NSString *sid);

NSString *ZMPasswordTypeToStr(ZMPasswordType aid);
ZMPasswordType ZMPasswordTypeFromStr(NSString *sid);

/// 获取当前时间
uint8_t ZMGetTimeYear();
uint8_t ZMGetTimeMonth();
uint8_t ZMGetTimeDay();
uint8_t ZMGetTimeHour();
uint8_t ZMGetTimeMinute();
uint8_t ZMGetTimeSeconds();

uint8_t ZMBcd2HexValue(uint8_t value);

///> 命令字
typedef enum : uint8_t {
    BLUETOOTH_CMD_TYPE_SET_ADMIN = 0X01,        // 设置管理员ID
    BLUETOOTH_CMD_TYPE_OPER_LOCK = 0X02,        // 锁操作
    BLUETOOTH_CMD_TYPE_SET_PWD = 0X03,      // 设置动态密码
    BLUETOOTH_CMD_TYPE_QUERY_ADMIN = 0X04,      // 查询管理员密码及电量BLUETOOTH_CMD_TYPE_MODIFY_ADMIN_PWD = 0X05;// 修改管理员密码
    BLUETOOTH_CMD_TYPE_SET_TIME = 0X06,     // 设置时间
    BLUETOOTH_CMD_TYPE_QUERY_SET = 0X07,        // 查询锁是否进入设置状态
} ZMCmdType;

///> 返回值常量
typedef enum : uint8_t {
    BLUETOOTH_CMD_RESPONSE_CURRESS = 0x00,          //执行成功
    BLUETOOTH_CMD_RESPONSE_FAILED = 0x01,           //执行失败
    BLUETOOTH_CMD_RESPONSE_ERROR = 0x02,            //执行错误
    BLUETOOTH_CMD_RESPONSE_NOT_SET = 0x03,          //没有进入设置状
    BLUETOOTH_CMD_RESPONSE_CHECK_ERROR = 0x04,          //管理员ID校验错误
    BLUETOOTH_CMD_RESPONSE_KEY_TIMEOUT = 0x05,          //电子钥匙过有效期
    BLUETOOTH_CMD_RESPONSE_KEY_TIME_NOT_TO = 0x06,          //电子钥匙未到启用时间
    BLUETOOTH_CMD_RESPONSE_KEY_NOT_IN_TIME = 0x07,          //电子钥匙不在开锁时间窗内
    BLUETOOTH_CMD_RESPONSE_KEY_LOSE = 0x09,         //动态码接收时部分丢失，需重发
    BLUETOOTH_CMD_RESPONSE_SET = 10,          //已经进入管理设置状态
    BLUETOOTH_CMD_RESPONSE_NOT_ADMIN_SET = 11,            //非管理ID设置状态
    BLUETOOTH_CMD_RESPONSE_TIME_ERROR = 12,     //时间有误需管理员校对时间
    BLUETOOTH_CMD_RESPONSE_OPEN_STATE = 0x00,           //车位锁开启装态
    BLUETOOTH_CMD_RESPONSE_CLOSE_STATE = 0x01,          //车位锁关闭装态
    BLUETOOTH_CMD_RESPONSE_NO_STATE = 0x0F,         //没有车位锁装态
} ZMResponseCode;

@interface ZMCmdBase : NSObject {
@public
    uint8_t _command;
}

@property uint8_t command;

- (void)unbox:(NSData *)data;
- (NSData *)box;


@end

/**
2.1.0 设置管理ID
app蓝牙锁     COMMAND Type: 0x01


COMMAND：命令类型，值为0x01；
Admin ID：管理员ID。
Time：6Byte 年月日时分秒，修改设备时间。

2.1.1 管理ID设置应答
蓝牙锁app      COMMAND Type: 0x01

COMMAND：命令类型，与下发过来的相同；
Result：返回执行结果-->0：命令执行成功；
                     1：命令执行失败
                        2：命令有误
                        3：未进入设置状态；          
*/
@interface ZMCmd0x01:ZMCmdBase {
@public
    
}
@property ZMAdminIdType adminId;
@property uint8_t year;     /// 默认当前时间
@property uint8_t month;    /// 默认当前时间
@property uint8_t day;      /// 默认当前时间
@property uint8_t hour;     /// 默认当前时间
@property uint8_t minute;   /// 默认当前时间
@property uint8_t seconds;  /// 默认当前时间
@end

/**
2.2.0 开锁
app蓝牙锁    COMMAND Type: 0x02

COMMAND：命令类型，值为0x02;
Type：用户类型及控制类型，字节高位代表用户类型，低位代表指的是开锁方式
0x01:管理员正常开锁
0x02:管理员闭锁
0x03:管理员常开式开锁
0x11:用户正常开锁
0x12:用户闭锁开锁
0x13:用户常开式开锁
End Time：电子钥匙有效期结束时间；
star Time：电子钥匙有效期开始时间；
Admin ID：管理ID；
注：开锁类型为管理员时，只核对管理ID是否正确，不理会有效期限制；开锁类型为用户时，以核对有效期及时间窗来判断是否开锁；
Time star1：一天内的第1个有效时间窗的开始时间默认0000
Time end1：1天内第1个有效时间窗的结束时间默认 FFFF
Time star2：一天内的第2个有效时间窗的开始时间默认0000
Time end2：1天内第2个有效时间窗的结束时间默认 FFFF
Time star3：一天内的第3个有效时间窗的开始时间默认0000
Time end3：1天内第3个有效时间窗的结束时间默认 FFFF
时间窗限制表示在该时段内才可以开启，其他时间不能开启，结束时间FFFF表示不限制。
最多可以设置3个时间窗参数，时间窗设置避免时间段重叠。时间参数用BCD码表示，比如12点30分，表示为0x12,0x30。
注：为保证开锁数据包的安全性，开锁数据包使用另外的加/密方式，详见加密说明文档。
2.2.1 开锁指令应答
蓝牙锁app    COMMAND Type: 0x02

COMMAND：命令类型，与下发过来的相同；
Result：返回执行结果-->    0：命令执行成功；
                        4：管理员ID校验错误；
                        5：电子钥匙过有效期；
                        6：电子钥匙未到启用时间；
                        7：电子钥匙不在开锁时间窗内；         
                        12：时间有误需管理员校对时间

*/
@interface ZMCmd0x02_1:ZMCmdBase {
@public
    
}
@property uint8_t type;
@property ZMAdminIdType adminId;

@end

@interface ZMCmd0x02_2:ZMCmdBase{
@public
    
}

@property uint8_t type;
@property ZMAdminIdType adminId;

@property uint8_t endTimeYear;
@property uint8_t endTimeMonth;
@property uint8_t endTimeDay;
@property uint8_t endTimeHour;
@property uint8_t endTimeMinute;

@property uint8_t startTimeYear;
@property uint8_t startTimeMonth;
@property uint8_t startTimeDay;
@property uint8_t startTimeHour;
@property uint8_t startTimeMinute;

@property uint8_t start1Hour;           // 默认0x00
@property uint8_t start1Minute;         // 默认0x00
@property uint8_t end1Hour;             // 默认0xFF
@property uint8_t end1Minute;           // 默认0xFF

@property uint8_t start2Hour;           // 默认0x00
@property uint8_t start2Minute;         // 默认0x00
@property uint8_t end2Hour;             // 默认0xFF
@property uint8_t end2Minute;           // 默认0xFF

- (void)setEndTime:(NSString *)time;    // "2017-03-27 23:04";
- (void)setEndTimeWindow:(NSString *)window; //"23:59,23:59";
- (void)setStartTime:(NSString *)time;    // "2017-03-27 23:04";
- (void)setStartTimeWindow:(NSString *)window; //"23:59,23:59";
@end

/**
重置/下载动态码操作
app蓝牙锁      COMMAND Type: 0x03

COMMAND：命令类型，值为0x03;
Admin ID：5byte。
PASSWORD：每2Byte为一个动态码，共300个动态码，共600Byte；
CRC8：对PASSWORD字段的校验；
2.3.1 下载/重置动态码应答
蓝牙锁app      COMMAND Type: 0x03


COMMAND：命令类型，与下发过来的相同;
Result：返回执行结果-->    0：命令执行成功；
                        4：管理员ID校验错误；
                        9：动态码接收时部分丢失，需重发；
*/
@interface ZMCmd0x03:ZMCmdBase {
    
}

@property ZMAdminIdType adminId;
- (void)addPassword:(ZMDynamicPasswordType)password;
@end

/**
2.4.0 管理员手机查询状态信息：
app蓝牙锁      COMMAND Type: 0x04

COMMAND：命令类型，值为0x04;
Admin ID：5byte。
2.4.1 管理员手机查询应答
蓝牙锁app      COMMAND Type: 0x04

COMMAND：命令类型，与下发过来的相同;
Result1：执行结果->0：命令执行成功；4：管理员ID校验错误；
Result2：返回管理员密码；
Result3：门锁当前电压值。6V----100% 最低电压4.6V，乘10倍表示，如4.6v，则是46(0x2E)；APP显示电压百分比或电量图表。（当管理员ID校验错误时返回0）
Result4：蓝牙锁当前时间值；
*/
@interface ZMCmd0x04:ZMCmdBase {
    
}

@property ZMAdminIdType adminId;
@end

/**
2.5.0 修改管理员密码：
app蓝牙锁      COMMAND Type: 0x05

COMMAND：命令类型，值为0x05;
Admin ID：5byte。
Admin password：管理员密码10进制6位密码。
2.5.1 管理员密码修改结果
蓝牙锁app      COMMAND Type: 0x05

COMMAND：命令类型，与下发过来的相同;
Result：返回执行结果-->    0：命令执行成功；
                        4：管理员ID校验错误
*/
@interface ZMCmd0x05:ZMCmdBase {
    
}

@property ZMAdminIdType adminId;
@property ZMPasswordType password;
@end

/**
2.6.0 时间设置
app蓝牙锁      COMMAND Type: 0x06

COMMAND：命令类型，值为0x06;
Admin ID：5byte。
Set Time Data：时间数据，年月日时分秒；
2.6.1 时间设置应答
蓝牙锁app      COMMAND Type: 0x06

COMMAND：命令类型，与下发过来的相同;
Result：返回执行结果--> 0：命令执行成功；
                        1：命令执行失败；                       
                        3：命令有误； 
                        4：管理员ID校验错误
*/
@interface ZMCmd0x06:ZMCmdBase {
    
}

@property ZMAdminIdType adminId;
@property uint8_t year;     /// 默认当前时间
@property uint8_t month;    /// 默认当前时间
@property uint8_t day;      /// 默认当前时间
@property uint8_t hour;     /// 默认当前时间
@property uint8_t minute;   /// 默认当前时间
@property uint8_t seconds;  /// 默认当前时间
@end

/**
2.7.0 查询蓝牙锁是否进入管理ID设置状态：
app蓝牙锁      COMMAND Type: 0x07

COMMAND：命令类型，值为0x07;
2.7.1 返回设置状态
蓝牙锁app      COMMAND Type: 0x07

COMMAND：命令类型，与下发过来的相同;
Result：返回执行结果-->    10：已经进入管理设置状态；
                        11：非管理ID设置状态
*/
@interface ZMCmd0x07:ZMCmdBase {
    
}

@end

/*********************************************************
                        应答协议
*********************************************************/

@interface ZMCmdResponse_1:ZMCmdBase {
@public
    uint8_t _result;
}
@property  uint8_t   result;

- (NSString *)resultString;
@end

/// 管理员查询应答协议
@interface ZMCmdResponse_2:ZMCmdResponse_1 {
    
}
@property    ZMPasswordType password;
@property    float   voltage; // 单位 V
@property    uint8_t year;
@property    uint8_t month;
@property    uint8_t day;
@property    uint8_t hour;
@property    uint8_t minute;
@property    uint8_t seconds;
@end

#endif /* ZMProtocolCmd_h */
