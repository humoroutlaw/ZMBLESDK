//
//  ZMProtocolCmd.h
//  ZMKMBleSDK
//
//

#ifndef ZMProtocolCmd_h
#define ZMProtocolCmd_h

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

typedef struct {
    char    password[4];        // 比如 '1234'，无0结尾
} ZMDynamicPasswordType;

typedef struct {
    char    password[6];       // '112233'，无0结尾
} ZMPasswordType;

typedef struct {
    char    password[6];    //管理员账号，无0结尾
    char    serial[4];      //服务器产生的该管理员账号下名下的智能锁编号序号，比如 '1234'，无0结尾
} ZMAdminIdType;

typedef struct {
    char    password[6];    //射频卡ID
} ZMRFCardType;

typedef struct {
    char passcode[12];
} ZMDynPasscode;

// 密码等辅助函数
NSString *ZMBcdTypeToStr(void *src, size_t len);
void ZMBcdTypeFromStr(NSString *src, void *dst, size_t dstlen);

NSString *ZMAdminIdTypeToStr(ZMAdminIdType aid);
ZMAdminIdType ZMAdminIdTypeFromStr(NSString *sid);

NSString *ZMPasswordTypeToStr(ZMPasswordType aid);
ZMPasswordType ZMPasswordTypeFromStr(NSString *sid);

NSString *ZMRFCardToStr(ZMRFCardType aid);
ZMRFCardType ZMRFCardTypeFromStr(NSString *sid);

NSString *ZMPasscodeTypeToStr(ZMDynPasscode aid);
ZMDynPasscode ZMPasscodeFromStr(NSString * sid);


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
    BLUETOOTH_CMD_TYPE_SET_PWD = 0X03,          // 设置动态密码
    BLUETOOTH_CMD_TYPE_QUERY_ADMIN = 0X04,      // 查询管理员密码及电量
    BLUETOOTH_CMD_TYPE_MODIFY_ADMIN_PWD = 0X05, // 修改管理员密码
    BLUETOOTH_CMD_TYPE_SET_TIME = 0X06,         // 设置时间
    BLUETOOTH_CMD_TYPE_QUERY_SET = 0X07,        // 查询锁是否进入设置状态
    BLUETOOTH_CMD_TYPE_SET_FINGERPRINT = 0X08,          //设置指纹
    BLUETOOTH_CMD_TYPE_COLLECT_FINGERPRINT = 0x09,      //指纹采集结果Notify
    BLUETOOTH_CMD_TYPE_SET_FINGERPRINT_RESULT =0x0A,    //指纹设置结果Notify
    BLUETOOTH_CMD_TYPE_QUERY_UNLOCK_RECORD =0x0B,      //查询锁操作记录
    BLUETOOTH_CMD_TYPE_DELETE_UNLOCK_RECORD = 0x0C,    //删除锁操作记录
    BLUETOOTH_CMD_TYPE_QUERY_FINGERPRINTS = 0x0D,      //查询已经设定的指纹
    BLUETOOTH_CMD_TYPE_DELETE_FINGERPRINT = 0x0E,      //删除指纹
    BLUETOOTH_CMD_TYPE_MODIFY_FINGERPRINT = 0x0F,      //修改指纹有限时间
    BLUETOOTH_CMD_TYPE_SET_RFCARD = 0x10,             //设置卡片
    BLUETOOTH_CMD_TYPE_DELETE_RFCARD = 0x11,          //删除卡片
    BLUETOOTH_CMD_TYPE_QUERY_RFCARD = 0x12,           //查询已经设定的卡片
} ZMCmdType;


@interface ZMDoorRecord:NSObject

/**
 type 1 刷卡开门
      2 管理员密码开门
      3 动态密码开门
      4 用户密码开门(动态密码对应)
      5 指纹开门
      6 钥匙开门
      7 遥控开门
      8 用户密码
 */
@property (nonatomic,strong) NSString * type;//操作类型
@property (nonatomic,strong) NSString * addition;//开锁附加信息
@property (nonatomic,strong) NSString * time;//操作时间

@end

@interface ZMPrintRecord:NSObject

@property (nonatomic,strong) NSString * nick;
@property (nonatomic,assign) uint8_t  printId;
@property (nonatomic,strong) NSString * time;
@property (nonatomic,strong) NSString * creatTime;

@end

@interface ZMCardRecord:NSObject

@property (nonatomic,strong) NSString * nick;
@property (nonatomic,strong) NSString * cardId;
@property (nonatomic,strong) NSString * time;

@end



@interface ZMCmdBase : NSObject {
@public
    uint8_t _command;
    CGFloat _timeOut;
}

@property uint8_t command;
@property BOOL noDecrpt;
@property uint8_t timeKey;
  //比如16年10月14号11点 直接使用16进制表示0x16 0x10 0x14 0x11 相加等于4B 则timeKey = 0x4B
@property BOOL getValue;
@property CGFloat timeOut;//通讯超时时间 

- (void)unbox:(NSData *)data;
- (void)receive:(NSArray<NSData *> *)recordList;
- (NSData *)box;

@end

/**
2.1.0 设置管理ID
app->蓝牙锁     COMMAND Type: 0x01

COMMAND:命令类型，值为0x01;
Admin ID:管理员ID.
Time:6Byte 年月日时分秒,修改设备时间.

2.1.1 管理ID设置应答
蓝牙锁->app      COMMAND Type: 0x01

 COMMAND:命令类型，与下发过来的相同;
 Result:返回执行结果-->   0:命令执行成功;
                        1:命令执行失败;
                        2:命令有误;
                        3:未进入设置状态;
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
app->蓝牙锁    COMMAND Type: 0x02

COMMAND：命令类型，值为0x02;
Type：用户类型及控制类型，字节高位代表用户类型，低位代表指的是开锁方式
0x01:管理员正常开锁
0x02:管理员闭锁
0x03:管理员常开式开锁
0x11:用户正常开锁
0x12:用户闭锁开锁
0x13:用户常开式开锁
End Time:电子钥匙有效期结束时间;
Start Time:电子钥匙有效期开始时间;
Admin ID:管理ID;
注:开锁类型为管理员时,只核对管理ID是否正确,不理会有效期限制;开锁类型为用户时,以核对有效期及时间窗来判断是否开锁;
Time star1:一天内的第1个有效时间窗的开始时间默认0000
Time end1:1天内第1个有效时间窗的结束时间默认 FFFF
Time star2:一天内的第2个有效时间窗的开始时间默认0000
Time end2:1天内第2个有效时间窗的结束时间默认 FFFF
时间窗限制表示在该时段内才可以开启，其他时间不能开启,结束时间FFFF表示不限制.
时间窗设置避免时间段重叠.时间参数用BCD码表示,比如12点30分,表示为0x12,0x30.
 
2.2.1 开锁指令应答
蓝牙锁->app    COMMAND Type: 0x02
 COMMAND:命令类型，与下发过来的相同;
 Result:返回执行结果-->    0:命令执行成功;
                         4:管理员ID校验错误;
                         5:电子钥匙过有效期;
                         6:电子钥匙未到启用时间;
                         7:电子钥匙不在开锁时间窗内;
                         12:时间有误需管理员校对时间

*/
@interface ZMCmd0x02_1:ZMCmdBase{
    
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
app->蓝牙锁  COMMAND Type: 0x03

 COMMAND:命令类型,值为0x03;
 Admin ID:5byte.
 PASSWORD:每2Byte为一个动态码,共300个动态码,共600Byte;
 CRC8:对PASSWORD字段的校验;
2.3.1 下载/重置动态码应答
蓝牙锁->app      COMMAND Type: 0x03
 
 COMMAND:命令类型，与下发过来的相同;
 Result:返回执行结果-->    0:命令执行成功;
                         4:管理员ID校验错误;
                         9:动态码接收时部分丢失,需重发;
*/
@interface ZMCmd0x03:ZMCmdBase{
    
}

@property ZMAdminIdType adminId;
- (void)addPassword:(ZMDynamicPasswordType)password;
@end

/**
2.4.0 管理员手机查询状态信息：
app->蓝牙锁      COMMAND Type: 0x04

COMMAND:命令类型,值为0x04;
 Admin ID:5byte.
2.4.1 管理员手机查询应答
蓝牙锁->app      COMMAND Type: 0x04

 COMMAND:命令类型，与下发过来的相同;
 Result1:执行结果->低位 0:命令执行成功;
                      4:管理员ID校验错误；
                  锁状态:高位0:开启  1:锁闭, 没有获取到或没有锁状态F
                  示例:0x10门锁处于开启状态命令执行成功.
 Result2:返回管理员密码;
 Result3:门锁当前电压值.6V----100% 最低电压4.6V,乘10倍表示，如4.6v，则是46(0x2E);APP显示电压百分比或电量图表.（当管理员ID校验错误时返回0）
 Result4:蓝牙锁当前时间值;
*/
@interface ZMCmd0x04:ZMCmdBase{
    
}

@property ZMAdminIdType adminId;

@end

/**
2.5.0 修改管理员密码：
app->蓝牙锁      COMMAND Type: 0x05

 COMMAND:命令类型,值为0x05;
 Admin ID:5byte.
 Admin password:管理员密码10进制6位密码.
2.5.1 管理员密码修改结果
蓝牙锁->app      COMMAND Type: 0x05

COMMAND：命令类型，与下发过来的相同;
 Result：返回执行结果-->    0:命令执行成功;
                          4:管理员ID校验错误
*/
@interface ZMCmd0x05:ZMCmdBase {
    
}

@property ZMAdminIdType adminId;
@property ZMPasswordType password;
@end

/**
2.6.0 时间设置
app->蓝牙锁      COMMAND Type: 0x06

 COMMAND:命令类型,值为0x06;
 Admin ID:5byte.
 Set Time Data:时间数据,年月日时分秒;
2.6.1 时间设置应答
蓝牙锁->app      COMMAND Type: 0x06

COMMAND：命令类型，与下发过来的相同;
 Result：返回执行结果--> 0:命令执行成功;
                       1:命令执行失败:
                       3:命令有误;
                       4:管理员ID校验错误
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
app->蓝牙锁      COMMAND Type: 0x07

COMMAND：命令类型，值为0x07;
2.7.1 返回设置状态
蓝牙锁->app      COMMAND Type: 0x07

COMMAND：命令类型，与下发过来的相同;
 Result：返回执行结果-->   10:已经进入管理设置状态;
                         11:非管理ID设置状态
*/
@interface ZMCmd0x07:ZMCmdBase {
    
}

@end

/**
 添加指纹
 app->蓝牙锁    COMMAND Type:0x08
 Admin ID:5byte
 指纹ID   :1byte
 有效时间  :4byte
 
 蓝牙锁->app     COMMAND Type: 0x08

返回执行结果Result
 Result1
           0:命令执行成功;
           1:命令执行失败;
           2:命令有误;
           4:管理员ID校验错误;
 Result=1 有Result2
          1:指纹已设置满;
          2:时间参数错误;
 
 蓝牙锁->app     COMMAND Type: 0x09
 返回指纹采集结果 Result
       Result1
            0:命令执行成功;
            1:指纹采集失败;
       Result1 = 0,才有Result2,Result2第几次采集
 
 蓝牙锁->app     COMMAND Type: 0x0A
 Result: Result1  0:指纹设置成功;
                  1:指纹设置失败;
               Result1=0时，才有以下数据
     Result2:指纹的编号ID:01—FF（1byte）+有效时间（4byte）

 
 */
@interface ZMCmd0x08:ZMCmdBase {
    
}
@property ZMAdminIdType adminId;
@property uint8_t year;
@property uint8_t month;
@property uint8_t day;
@property uint8_t hour; 

@end


/**
 查询指纹状态
 app->蓝牙锁    COMMAND Type:0x0D
 Admin ID:5byte
 
 蓝牙锁->app    COMMAND Type:0x0D
 
 Result1:返回执行结果—>  0:命令执行成功；
                       1:命令执行失败;
                       2:命令有误;
                       3:当前记录为空;
                       4:管理员ID校验错误
   Result1=0 蓝牙发送以下数据：
    Result2:指纹ID（1byte）+ 时间参数（4byte）
 */

@interface ZMCMD0x0D:ZMCmdBase
{
    
}

@property ZMAdminIdType adminId;

@end


/**
 删除指纹
 app->蓝牙锁    COMMAND Type:0x0E
 Admin ID:5byte
 Print ID: 1byte  0x00 删除全部指纹 0x00 - 0xFF
 
 蓝牙锁->app   COMMAND Type:0x0E

 Result:
 0:命令执行成功;
 1:命令执行失败;
 2:命令有误;
 4:管理员ID校验错误
 */
@interface ZMCmd0x0E:ZMCmdBase {

}
@property ZMAdminIdType adminId;
@property uint8_t printID;

@end

/**
   修改指纹有效时间
   app->蓝牙锁    COMMAND Type:0x0F
   Admin ID:5byte.
   需要修改的指纹编号及时间值:指纹ID（1byte)+时间值（4byte）
  app->蓝牙锁    COMMAND Type:0x0F
    Result 1:
       0:命令执行成功;
       1:命令执行失败;
       2:命令有误;
       4:管理员ID校验错误
   Result 1=1 时才有Result 2:
                   1:无此指纹ID
                   2:时间参数错误
*/
@interface ZMCMD0x0F:ZMCmdBase {

}

@property ZMAdminIdType adminId;
@property uint8_t printID;
@property uint8_t year;
@property uint8_t month;
@property uint8_t day;
@property uint8_t hour; 

@end

/**
 读取开门记录
 app->蓝牙锁    COMMAND Type:0x0B
 Admin ID:5byte
 
 蓝牙锁->app    COMMAND Type:0x0B

 Result1:
      0:命令执行成功;
      1:命令执行失败;
      3:记录为空
      2:命令有误;
      4:管理员ID校验错误
     Result 1=0 时才有Result2:
             Result2:N条开锁记录
 */
@interface ZMCmd0x0B:ZMCmdBase

@property ZMAdminIdType adminId;

@end

/**
 删除开门记录
 app->蓝牙锁    COMMAND Type:0x0C;
 Admin ID:5byte
 
 蓝牙锁->app    COMMAND Type:0x0C
 Result:
 0:命令执行成功;
 1:命令执行失败;
 2:命令有误;
 4:管理员ID校验错误
 */

@interface ZMCmd0x0C:ZMCmdBase

@property ZMAdminIdType adminId;

@end

/**
 增加卡片ID
 app->蓝牙锁    COMMAND Type:0x10
 Admin ID:5byte
 RF卡4byte的卡号+卡片的有效时间(年月日时4byte),若为00000000则该卡片无有效期限制.
 
 蓝牙锁->app    COMMAND Type:0x10
 
 Result1:返回执行结果-->  0:命令执行成功;
                        1:命令执行失败;
                        2:命令有误;
                        4:管理员ID校验错误
 Result1=1 时才有Result2:
                        1:系统已存在此卡号，并改写有效时间（如果系统已存在此卡片，则覆盖原来的记录，包括有效时间记录）
                        2:卡片容量已满
                        3:时间参数错误
 */

@interface ZMCmd0x10:ZMCmdBase

@property ZMAdminIdType adminId;
@property ZMRFCardType cardID;
@property uint8_t year;
@property uint8_t month;
@property uint8_t day;
@property uint8_t hour;
@end

/**
   app->蓝牙锁    COMMAND Type:0x11;
   Admin ID：5byte
   需要删除RF卡4byte的,若为00000000，则删除全部卡片
   蓝牙锁->app    COMMAND Type:0x11
   Result1: 返回执行结果-->
             0:命令执行成功;
             1:命令执行失败;
             2:命令有误;
             4:管理员ID校验错误
   Result1= 1才有以下数据
   Result2:1,无此卡号
*/
@interface ZMCmd0x11:ZMCmdBase

@property ZMAdminIdType adminId;
@property ZMRFCardType cardID;

@end

/**
 查询卡片ID
 app->蓝牙锁    COMMAND Type:0x12
 Admin ID:5byte
 
 蓝牙锁->app    COMMAND Type:0x12
 Result1:返回执行结果--> 0:命令执行成功;
                       1:命令执行失败;
                       2:命令有误;
                       3:当前记录为空;
                       4:管理员ID校验错误
(若Result1没有成功则无Result2):已经登记的RD卡ID(4Byte)+有效时间(4Byte),
 */
@interface ZMCmd0x12:ZMCmdBase

@property ZMAdminIdType adminId;

@end


/**
 app->蓝牙锁    COMMAND Type:0x13;
 Admin ID：5byte
 需要删除临时密码6byte的,若为000000000000，则删除全部临时密码
 
 蓝牙锁->app    COMMAND Type:0x13
 Result1: 返回执行结果-->
           0:命令执行成功;
           1:命令执行失败;
           2:命令有误;
           4:管理员ID校验错误
       Result1= 1才有以下数据
        Result2:1,无此动态码
 */


@interface ZMCmd0x13:ZMCmdBase

@property ZMAdminIdType adminId;
@property ZMDynPasscode passcode;

@end


/**
 初始化门锁
 app->蓝牙锁  COMMAND：命令类型，值为0x14;
 Admin ID：5byte
 
 蓝牙锁->app    COMMAND Type:0x14

 Result1： 返回执行结果-->
 0：命令执行成功；
 1：命令执行失败；
 2：命令有误；
 4：管理员ID校验错误
 */

@interface ZMCmd0x14 : ZMCmdBase

@property ZMAdminIdType adminId;

@end


/*********************************************************
                        应答协议
*********************************************************/

@interface ZMCmdResponse_1:ZMCmdBase {
@public
    uint8_t _result;
}
@property  uint8_t   result;

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

@interface ZMCmdResponse_3:ZMCmdResponse_1{
   
}

-(void)readResult:(NSData *)data;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wobjc-property-no-attribute"

@property  NSMutableArray <ZMDoorRecord *>*  unlockRecords;
@property  NSMutableArray <ZMPrintRecord *>* printIdRecords;
@property  NSMutableArray <ZMCardRecord *>* cardIdRecords;

#pragma clang diagnostic pop

@end


@interface ZMCmdResponse_4:ZMCmdResponse_1
{
    
}

@property uint8_t comd;//返回结果
@property uint8_t printId;//指纹id
@property uint8_t year;
@property uint8_t month;
@property uint8_t day;
@property uint8_t hour;

@end


#endif /* ZMProtocolCmd_h */
