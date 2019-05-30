//
//  ViewController.m
//  ZMKMDemo
//
//

#import "ViewController.h"
#import "FuncViewController.h"

@interface ViewController ()

@property (nonatomic,strong) NSTimer * scanTimer;

@end

@implementation ViewController {
    ZMBleManager *_deviceManager;
}

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
     self.title = @"添加锁";
    
    _deviceManager = [ZMBleManager sharedManager];
   
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(foundDevice:)
                                                 name:kZMBleConnectDeviceNotification
                                               object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(foundDevice:)
                                                 name:kZMBleDisconnectDeviceNotification
                                               object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(powerOn:)
                                                 name:kZMBlePowerOnNotification
                                               object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(powerOff:)
                                                 name:kZMBlePowerOffNotification
                                               object:nil];
    
}

-(void)viewWillAppear:(BOOL)animated{
    if (self.scanTimer) {
        return;
    }
    if (![ZMBleManager sharedManager].bleStatus) {
        [SVProgressHUD dismiss];
        [SVProgressHUD showInfoWithStatus:@"请开启蓝牙"];
    }
    self.scanTimer = [NSTimer scheduledTimerWithTimeInterval:2.f target:self selector:@selector(scanDev) userInfo:nil repeats:YES];
}

-(void)viewWillDisappear:(BOOL)animated{
    if ([self.scanTimer isValid]) {
        [self.scanTimer invalidate];
        self.scanTimer = nil;
    }
}

-(void)scanDev{
    [_deviceManager startScan];
    [_deviceManager discoverService:self.nameArr block:nil];
    [_tableView reloadData];
}
- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)foundDevice:(NSNotification *)noti{
    
}

- (void)powerOn:(NSNotification *)noti {
    [SVProgressHUD showInfoWithStatus:@"设备打开成功，开始扫描设备"];
}

- (void)powerOff:(NSNotification *)noti {
    [SVProgressHUD showInfoWithStatus:@"手机蓝牙关闭，请打开"];
    [_deviceManager stopScan];
}

- (void)viewDidAppear:(BOOL)animated {
    [super viewDidAppear:animated];
    [_tableView reloadData];
}

#pragma mark -table委托 table delegate

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section{
    return _deviceManager.deviceList.count;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath{
    
    UITableViewCell *cell = [self.tableView dequeueReusableCellWithIdentifier:@"cell"];
    ZMDevice *dev = [[_deviceManager deviceList] objectAtIndex:indexPath.row];
    
    if (!cell) {
        cell = [[UITableViewCell alloc]initWithStyle:UITableViewCellStyleSubtitle reuseIdentifier:@"cell"];
        cell.accessoryType = UITableViewCellAccessoryDisclosureIndicator;
    }
    cell.selectionStyle = UITableViewCellSelectionStyleNone;
    cell.textLabel.text = [dev name];
    return cell;
}

-(void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath{
    //停止扫描
    [_deviceManager stopScan];
    [SVProgressHUD showWithStatus:@"连接中"];
    ZMDevice * dev = _deviceManager.deviceList[indexPath.row];
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(3.0 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
        [SVProgressHUD dismiss];
    });
    [_deviceManager startConnect:dev.peripheral characteristics:^{
        [SVProgressHUD dismiss];
        [self.tableView deselectRowAtIndexPath:indexPath animated:YES];
        [self performSegueWithIdentifier:@"segueDevice" sender:dev];
    }];
}

- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender
{
    
    FuncViewController *vcToPushTo = segue.destinationViewController;
    vcToPushTo.theDevice = sender;
}
@end
