//
//  ZMDevice.h
//  ZMKMBleSDK
//
//

#import <Foundation/Foundation.h>
#import "ZMProtocolCmd.h"
@import CoreBluetooth;
@class ZMDevice;

typedef NS_ENUM(NSUInteger, ZMDeviceError) {
    ZMDeviceNoError = 0,
    ZMDeviceNotConnect,
    ZMDeviceNoCBCharacteristic,
    ZMDeviceTimeout,
};
typedef void(^RetCmdBlock)(id rsp,ZMDeviceError error);

@interface ZMDevice : NSObject

@property CBPeripheral *peripheral;
@property CBService         *cmdCBService;
@property CBCharacteristic  *cmdCBCharacteristic;


- (void)reset;

/**
 @brief  返回设备本地名称
 @return name
 */
- (NSString *)name;

/**
  @brief  蓝牙基本操作
  @return ZMDeviceError
  @param cmd 发送的指令
  @param retblock 回调
*/
- (ZMDeviceError)sendCmd:(ZMCmdBase *)cmd retBlock1:(RetCmdBlock)retblock;

/**
 @brief  获取设备电量
 @return ZMDeviceError
 @param cmd 发送的指令
 @param retblock 回调
 */
- (ZMDeviceError)sendCmd:(ZMCmdBase *)cmd retBlock2:(RetCmdBlock)retblock;
/**
 @brief  获取开锁记录,指纹记录,卡片记录
 @return ZMDeviceError
 @param  cmd 发送的指令
 @param  retblock 回调
 */
- (ZMDeviceError)sendCmd:(ZMCmdBase *)cmd retBlock3:(RetCmdBlock)retblock;
/**
 @brief  设置指纹
 @return ZMDeviceError
 @param  cmd 发送的指令
 @param  retblock 回调
 */
- (ZMDeviceError)sendCmd:(ZMCmdBase *)cmd retBlock4:(RetCmdBlock)retblock;

@end
