//
//  ZMDevice.h
//  ZMKMBleSDK
//
//

#import <Foundation/Foundation.h>
#import "ZMProtocolCmd.h"
@import CoreBluetooth;
@class ZMDevice;

typedef void(^RetCmdBlock)(id rsp);


typedef NS_ENUM(NSUInteger, ZMDeviceError) {
    ZMDeviceNoError = 0,
    ZMDeviceNotConnect,
    ZMDeviceNoCBCharacteristic,
};

@interface ZMDevice : NSObject

/**
 peripheral.statu可以获取到当前连接状态
 */
@property CBPeripheral *peripheral;
@property NSDictionary *advertisementData;
@property NSNumber     *RSSI;


@property CBService         *cmdCBService;
@property CBCharacteristic  *cmdCBCharacteristic;

/**
 以下几个值取自name中的不同位置
 */
@property (nonatomic, readonly) int lockType; /// 锁类型
@property (nonatomic, readonly) int version;  /// 固件版本号
@property (nonatomic, readonly) int deviceId; /// 设备ID

- (void)reset;

/**
 返回设备本地名称
 
 @return name
 */
- (NSString *)name;


- (ZMDeviceError)sendCmd:(ZMCmdBase *)cmd retBlock1:(RetCmdBlock)retblock;
- (ZMDeviceError)sendCmd:(ZMCmdBase *)cmd retBlock2:(RetCmdBlock)retblock;
@end
