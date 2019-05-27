//
//  FuncViewController.h
//  ZMKMDemo
//
//

#import <UIKit/UIKit.h>


@interface FuncViewController : UIViewController

@property ZMDevice *theDevice;

@property (nonatomic, weak) IBOutlet UIButton *connectBtn;
@property (nonatomic, weak) IBOutlet UIButton *disconnectBtn;

@property (nonatomic, weak) IBOutlet UITextField  *adminIdLabel;
@property (nonatomic, weak) IBOutlet UITextField  *adminPasswordLabel;
@property (nonatomic, weak) IBOutlet UITextField  *timeLabel;

@property (nonatomic, weak) IBOutlet UITextField  *startTime;
@property (nonatomic, weak) IBOutlet UITextField  *endTime;

@property (nonatomic, weak) IBOutlet UITextField  *timeStart1;
@property (nonatomic, weak) IBOutlet UITextField  *timeEnd1;

@property (nonatomic, weak) IBOutlet UITextField  *timeStart2;
@property (nonatomic, weak) IBOutlet UITextField  *timeEnd2;

@property (nonatomic, weak) IBOutlet UITextField  *timeStart3;
@property (nonatomic, weak) IBOutlet UITextField  *timeEnd3;

- (IBAction)doConnect:(id)sender;
- (IBAction)doDisconnect:(id)sender;

- (IBAction)doSetAdminId:(id)sender;
- (IBAction)doResetDynamicPasswork:(id)sender;
- (IBAction)doQueryStatus:(id)sender;
- (IBAction)doSetTime:(id)sender;
- (IBAction)doGetNowTime:(id)sender;
- (IBAction)doUnlock1:(id)sender;
- (IBAction)doUnlock2:(id)sender;
- (IBAction)doCheckAdmin:(id)sender;
@end
