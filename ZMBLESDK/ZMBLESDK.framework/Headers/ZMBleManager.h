//
//  ZMBleManager.h
//  ZMKMBleSDK
//
//

#import <Foundation/Foundation.h>
#import <CoreBluetooth/CoreBluetooth.h>
@class ZMDevice;
extern NSString *const kZMBleConnectDeviceNotification;
// userInfo = {"device":ZMDevice}
extern NSString *const kZMBleDisconnectDeviceNotification;
// userInfo = {"device":ZMDevice}
extern NSString * const kZMBleConnectFailNotificaiton;// userInfo = {"device":ZMDevice}
extern NSString * const kZMBlePowerOnNotification;
extern NSString * const kZMBlePowerOffNotification;

@interface ZMBleManager : NSObject

@property (nonatomic) NSMutableArray<ZMDevice *> *deviceList;

@property (nonatomic,assign) BOOL bleStatus;

+ (instancetype)sharedManager;

/**
   @brief 初始化蓝牙
*/
- (void)initBLE;
/**
   @brief 开启调试日志
*/
- (void)turnOnLog;

/**
   @brief 蓝牙扫描
*/
- (void)startScan;

/**
   @brief 停止扫描
 */
- (void)stopScan;


/**
   @brief 连接设备
   @param peripheral 外设参数
   @param findCharacteristic   回调Block
*/
-(void)startConnect:(CBPeripheral *)peripheral characteristics:(void(^)())findCharacteristic;

/**
   @brief 停止蓝牙连接
*/
-(void)stopBleConnect;

/**
   @brief 停止扫描,停止所有连接
*/
- (void)stopConnect;

/**
   @brief 发现设备
   @param nameArr 蓝牙过滤名称数组
   @param block   发现设备的回调
*/
-(void)discoverService:(NSArray <NSString *>*)nameArr block:(void(^)())block;
/**
   @brief 遍历设备
   @param name 根据名称过滤ZMDevice
*/
-(ZMDevice *)findOneDevice:(NSString *)name;

/**
   @brief 移除所有设备
*/
-(void)removeAllDevice;

@end
