//
//  ZMBleManager.h
//  ZMKMBleSDK
//
//

#import <Foundation/Foundation.h>
@class ZMDevice;
extern NSString *const kZMBleConnectDeviceNotification;    // userInfo = {"device":ZMDevice}
extern NSString *const kZMBleDisconnectDeviceNotification; // userInfo = {"device":ZMDevice}

extern NSString *const kZMBlePowerOnNotification;
extern NSString *const kZMBlePowerOffNotification;
typedef enum : uint8_t {
    BLUETOOTH_LOCK_TYPE_NONE = -1,
    BLUETOOTH_LOCK_TYPE_ALL = 0,	//所有
    BLUETOOTH_LOCK_TYPE_LOCK = 1,	//门锁
    BLUETOOTH_LOCK_TYPE_PARKING_LOCK = 2, //车位锁
    BLUETOOTH_LOCK_TYPE_PAD_LOCK = 3,//挂锁
    BLUETOOTH_LOCK_TYPE_DEVICE_LOCK = 4,//设备锁
    BLUETOOTH_LOCK_TYPE_ACCESS_LOCK = 5,//门禁锁
} ZMSearchType;


@interface ZMBleManager : NSObject

@property (nonatomic) NSMutableArray<ZMDevice *> *deviceList;

+ (instancetype)sharedManager;

- (void)startScan:(ZMSearchType)searchType;

- (void)stopScan;

@end
