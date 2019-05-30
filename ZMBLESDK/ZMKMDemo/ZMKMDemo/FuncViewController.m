//
//  FuncViewController.m
//  ZMKMDemo
//
//

#import "FuncViewController.h"

typedef enum {
    Year,
    Month,
    Day,
    Hour,
    Minute,
    Seconds,
} TimeType;

int get_time_type(NSString *str, TimeType tp) {
    int ret = 0;
    if ([str length] > [@"xx-xx-xx " length]) {
        if (tp == Year) {
            ret = [[str substringWithRange:NSMakeRange(0, 2)] intValue];
        }
        if (tp == Month) {
            ret = [[str substringWithRange:NSMakeRange(3, 2)] intValue];
        }
        if (tp == Day) {
            ret = [[str substringWithRange:NSMakeRange(6, 2)] intValue];
        }
        str = [str substringFromIndex:9];
    }
    if (tp == Hour) {
        ret = [[str substringWithRange:NSMakeRange(0, 2)] intValue];
    }
    if (tp == Minute) {
        ret = [[str substringWithRange:NSMakeRange(3, 2)] intValue];
    }
    if (tp == Seconds) {
        ret = [[str substringWithRange:NSMakeRange(6, 2)] intValue];
    }
    return ret;
}

@interface FuncViewController ()

@end

@implementation FuncViewController 
uint8_t Crc8(const void *vptr, int len);

- (void)viewDidLoad {
    [super viewDidLoad];
    self.title = @"锁详情";
    self.startTime.text = [self getTime:0];
    self.endTime.text = [self getTime:24*60*60];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (NSString *)getTime:(NSTimeInterval)interval{
    NSDate * date = [NSDate dateWithTimeIntervalSinceNow:interval];
    NSDateFormatter * format = [[NSDateFormatter alloc]init];
    [format setDateFormat:@"yy-MM-dd HH:mm"];
    return [format stringFromDate:date];
}


- (void)viewWillAppear:(BOOL)animated {
    if (self.theDevice.peripheral.state == CBPeripheralStateConnecting ||
        self.theDevice.peripheral.state == CBPeripheralStateConnected) {
        self.connectBtn.enabled = NO;
        self.disconnectBtn.enabled = YES;
    } else {
        self.connectBtn.enabled = YES;
        self.disconnectBtn.enabled = NO;
    }
}

-(void)viewWillDisappear:(BOOL)animated{
    [[ZMBleManager sharedManager]stopConnect];
}

- (void)viewDidDisappear:(BOOL)animated{
    
}
/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

- (IBAction)doConnect:(id)sender{
    
}

- (IBAction)doDisconnect:(id)sender{
    
}

- (ZMAdminIdType)myAdminId {
    ZMAdminIdType adminIdType;
    NSString *adminId = [_adminIdLabel text];
    ZMBcdTypeFromStr(adminId, &adminIdType, sizeof(adminIdType));
    return adminIdType;
}

- (IBAction)doSetAdminId:(id)sender {
    ZMCmd0x01 *cmd = [ZMCmd0x01 new];
    cmd.adminId = [self myAdminId];
    
    NSLog(@"set adminId %@", ZMAdminIdTypeToStr(cmd.adminId));
        
    int errCode = [_theDevice sendCmd:cmd retBlock1:^ (ZMCmdResponse_1 *rsp,ZMDeviceError error) {
        if (error == ZMDeviceTimeout){
            [SVProgressHUD showInfoWithStatus:@"超时洛"];
            return;
        }
        if (rsp.result != 0) {
            [SVProgressHUD showErrorWithStatus:[NSString stringWithFormat:@"Msg:%d",rsp.result]];
        }else{
            [SVProgressHUD showSuccessWithStatus:@"管理员设置成功"];
        }
    }];
    if (errCode == ZMDeviceNotConnect) {
        [SVProgressHUD showInfoWithStatus:@"设备未连接"];
    }
}
- (IBAction)reconnectDevice:(id)sender {
    if (_theDevice.peripheral.state != CBPeripheralStateConnected) {
         [[ZMBleManager sharedManager]startConnect:_theDevice.peripheral characteristics:nil];
    }
}

- (IBAction)doResetDynamicPasswork:(id)sender {
    ZMCmd0x03 *cmd = [ZMCmd0x03 new];
    cmd.adminId = [self myAdminId];
    
    for (int i = 0; i < 300; i++) {
        ZMDynamicPasswordType pass;
        pass.password[0] = pass.password[1] = pass.password[2] = pass.password[3] = '0'+rand()%9;
        [cmd addPassword:pass];
    }
    
    int errCode = [_theDevice sendCmd:cmd retBlock1:^ (ZMCmdResponse_1 *rsp,ZMDeviceError error) {
        if (error == ZMDeviceTimeout) {
            [SVProgressHUD showInfoWithStatus:@"超时洛"];
            return;
        }
        if (rsp.result != 0) {
            [SVProgressHUD showErrorWithStatus:[NSString stringWithFormat:@"Msg:%d",rsp.result]];
        }else{
            [SVProgressHUD showSuccessWithStatus:@"动态码更新成功"];
        }
    }];
    if (errCode == ZMDeviceNotConnect) {
        [SVProgressHUD showInfoWithStatus:@"设备未连接"];
    }
}

- (IBAction)doQueryStatus:(id)sender{
    ZMCmd0x04 *cmd = [ZMCmd0x04 new];
    cmd.adminId = [self myAdminId];
    
    int errCode = [_theDevice sendCmd:cmd retBlock2:^ (ZMCmdResponse_2 *rsp,ZMDeviceError error) {
        if (error == ZMDeviceTimeout) {
            [SVProgressHUD showInfoWithStatus:@"超时洛"];
            return;
        }
        if (rsp.result != 0) {
            [SVProgressHUD showErrorWithStatus:[NSString stringWithFormat:@"Msg:%d",rsp.result]];
        }else{
            NSMutableString *str = [NSMutableString new];
            ZMPasswordType password = rsp.password;
            [str appendFormat:@"查询成功\n"];
            [str appendFormat:@"管理员密码 %@ \n", ZMBcdTypeToStr(&password, sizeof(password))];
            [str appendFormat:@"电量 %2.2fV \n", rsp.voltage];
            [str appendFormat:@"时间 %02d-%02d-%02d %02d:%02d:%02d\n", rsp.year, rsp.month, rsp.day, rsp.hour, rsp.minute, rsp.seconds];
            [SVProgressHUD showInfoWithStatus:str];
            return;
        }
    }];
    if (errCode == ZMDeviceNotConnect) {
        [SVProgressHUD showInfoWithStatus:@"设备未连接"];
    }
}

- (IBAction)doChangePassword:(id)sender {
    ZMCmd0x05 *cmd = [ZMCmd0x05 new];
    cmd.adminId = [self myAdminId];
    ZMPasswordType pass;
    ZMBcdTypeFromStr(_adminPasswordLabel.text, &pass, sizeof(pass));
    cmd.password = pass;
    
    int errCode = [_theDevice sendCmd:cmd retBlock1:^ (ZMCmdResponse_1 *rsp,ZMDeviceError error) {
        if (error == ZMDeviceTimeout) {
            [SVProgressHUD showInfoWithStatus:@"超时洛"];
            return;
        }
        if (rsp.result != 0) {
            [SVProgressHUD showErrorWithStatus:[NSString stringWithFormat:@"Errcode:%d",rsp.result]];

        }else{
            [SVProgressHUD showSuccessWithStatus:@"管理员密码修改成功"];
        }
    }];
    if (errCode == ZMDeviceNotConnect) {
        [SVProgressHUD showInfoWithStatus:@"设备未连接"];
    }
}

- (IBAction)doSetTime:(id)sender {
    ZMCmd0x06 *cmd = [ZMCmd0x06 new];
    cmd.adminId = [self myAdminId];
    
    if (!_timeLabel.text.length) {
        [SVProgressHUD showInfoWithStatus:@"未选择时间"];
        return;
    }
    cmd.year = [[_timeLabel.text substringWithRange:NSMakeRange(0, 2)] intValue];
    cmd.month = [[_timeLabel.text substringWithRange:NSMakeRange(3, 2)] intValue];
    cmd.day = [[_timeLabel.text substringWithRange:NSMakeRange(6, 2)] intValue];
    cmd.hour = [[_timeLabel.text substringWithRange:NSMakeRange(9, 2)] intValue];
    cmd.minute = [[_timeLabel.text substringWithRange:NSMakeRange(12, 2)] intValue];
    cmd.seconds = [[_timeLabel.text substringWithRange:NSMakeRange(15, 2)] intValue];
    
    int errCode = [_theDevice sendCmd:cmd retBlock1:^ (ZMCmdResponse_1 *rsp,ZMDeviceError error) {
        if (error == ZMDeviceTimeout) {
            [SVProgressHUD showInfoWithStatus:@"超时洛"];
            return;
        }
        if (rsp.result != 0) {
            [SVProgressHUD showErrorWithStatus:[NSString stringWithFormat:@"Msg:%d",rsp.result]];
        }else{
           [SVProgressHUD showSuccessWithStatus:@"时间已校验"];
        }
    }];
    if (errCode == ZMDeviceNotConnect){
        [SVProgressHUD showInfoWithStatus:@"设备未连接"];
    }
}

- (IBAction)doGetNowTime:(id)sender {
    _timeLabel.text = [NSString stringWithFormat:@"%02d-%02d-%02d %02d:%02d:%02d",
                       ZMGetTimeYear(),ZMGetTimeMonth(),ZMGetTimeDay(),
                       ZMGetTimeHour(),ZMGetTimeMinute(),ZMGetTimeSeconds()];
}

- (IBAction)doUnlock1:(id)sender {
    ZMCmd0x02_1 *cmd = [ZMCmd0x02_1 new];
    cmd.adminId = [self myAdminId];
    cmd.type = 1;
    int errCode = [_theDevice sendCmd:cmd retBlock1:^ (ZMCmdResponse_1 *rsp,ZMDeviceError error) {
        if (error == ZMDeviceTimeout) {
            [SVProgressHUD showInfoWithStatus:@"超时洛"];
            return;
        }
        if (rsp.result != 0) {
            [SVProgressHUD showErrorWithStatus:[NSString stringWithFormat:@"Msg:%d",rsp.result]];
        }else{
          [SVProgressHUD showSuccessWithStatus:@"门锁已开"];
        }
    }];
    if (errCode == ZMDeviceNotConnect) {
        [SVProgressHUD showInfoWithStatus:@"设备未连接"];
    }
}
- (IBAction)doUnlock2:(id)sender {
    ZMCmd0x02_2 *cmd = [ZMCmd0x02_2 new];
    cmd.adminId = [self myAdminId];
    cmd.type = 11;
    cmd.startTimeYear = get_time_type(_startTime.text, Year);
    cmd.startTimeMonth = get_time_type(_startTime.text, Month);
    cmd.startTimeDay = get_time_type(_startTime.text, Day);
    cmd.startTimeHour = get_time_type(_startTime.text, Hour);
    cmd.startTimeMinute = get_time_type(_startTime.text, Minute);
    cmd.endTimeYear = get_time_type(_endTime.text, Year);
    cmd.endTimeMonth = get_time_type(_endTime.text, Month);
    cmd.endTimeDay = get_time_type(_endTime.text, Day);
    cmd.endTimeHour = get_time_type(_endTime.text, Hour);
    cmd.endTimeMinute = get_time_type(_startTime.text, Minute);
    
    cmd.start1Hour = get_time_type(_timeStart1.text, Hour);
    cmd.start1Minute = get_time_type(_timeStart1.text, Minute);
    cmd.end1Hour = get_time_type(_timeEnd1.text, Hour);
    cmd.end1Minute = get_time_type(_timeEnd1.text, Minute);

    cmd.start2Hour = get_time_type(_timeStart2.text, Hour);
    cmd.start2Minute = get_time_type(_timeStart2.text, Minute);
    cmd.end2Hour = get_time_type(_timeEnd2.text, Hour);
    cmd.end2Minute = get_time_type(_timeEnd2.text, Minute);

    int errCode = [_theDevice sendCmd:cmd retBlock1:^ (ZMCmdResponse_1 *rsp,ZMDeviceError error) {
        if (error == ZMDeviceTimeout) {
            [SVProgressHUD showInfoWithStatus:@"超时洛"];
            return;
        }
        if (rsp.result != 0) {
            [SVProgressHUD showErrorWithStatus:[NSString stringWithFormat:@"Msg:%d",rsp.result]];
        }else{
            [SVProgressHUD showSuccessWithStatus:@"门锁已开"];
        }
    }];
    if (errCode == ZMDeviceNotConnect) {
        [SVProgressHUD showInfoWithStatus:@"设备未连接"];
    }
}

- (IBAction)doCheckAdmin:(id)sender {
    ZMCmd0x07 *cmd = [ZMCmd0x07 new];
    int errCode = [_theDevice sendCmd:cmd retBlock1:^ (ZMCmdResponse_1 *rsp,ZMDeviceError error) {
        if (error == ZMDeviceTimeout) {
            [SVProgressHUD showInfoWithStatus:@"超时洛"];
            return;
        }
        [SVProgressHUD showInfoWithStatus:[NSString stringWithFormat:@"Msg:%d",rsp.result]];
    }];
    if (errCode == ZMDeviceNotConnect) {
        [SVProgressHUD showInfoWithStatus:@"设备未连接"];
    }
}

- (IBAction)getUnlockRecord:(id)sender{
    ZMCmd0x0B * cmd = [ZMCmd0x0B new];
    cmd.adminId = [self myAdminId];
   int errCode = [_theDevice sendCmd:cmd retBlock3:^(ZMCmdResponse_3 * rsp,ZMDeviceError error){
        if (error == ZMDeviceTimeout) {
            [SVProgressHUD showInfoWithStatus:@"超时洛"];
            return;
        }
    if (rsp.result != 0 && rsp.result != 3) {
        [SVProgressHUD showErrorWithStatus:[NSString stringWithFormat:@"Msg:%d",rsp.result]];
        [SVProgressHUD setForegroundColor:[UIColor blackColor]];
        [SVProgressHUD setBackgroundColor:[UIColor whiteColor]];
    }
    else if (rsp.result == 3){
        [SVProgressHUD showInfoWithStatus:@"暂无记录"];
    } else{
        if (rsp.unlockRecords.count){
         //获取到门锁记录,删除记录
         [self deleteDoorUnlockRecord:_theDevice];
        }
      }
    }];
    if (errCode == ZMDeviceNotConnect) {
        [SVProgressHUD showInfoWithStatus:@"设备未连接"];
    }
}

-(void)deleteDoorUnlockRecord:(ZMDevice *)dev{
    ZMCmd0x0C * cmd = [ZMCmd0x0C new];
    cmd.adminId = [self myAdminId];
   int errCode = [dev sendCmd:cmd retBlock1:^(ZMCmdResponse_1 * rsp,ZMDeviceError error) {
        if (error == ZMDeviceTimeout) {
            [SVProgressHUD showInfoWithStatus:@"超时洛"];
            return;
        }
        if (rsp.result != 0){
            [SVProgressHUD showErrorWithStatus:[NSString stringWithFormat:@"Msg:%d",rsp.result]];
            [SVProgressHUD setForegroundColor:[UIColor blackColor]];
            [SVProgressHUD setBackgroundColor:[UIColor whiteColor]];
        }
    }];
    
    if (errCode == ZMDeviceNotConnect) {
        [SVProgressHUD showInfoWithStatus:@"设备未连接"];
    }
    
}

- (IBAction)addFingerprints:(id)sender{
    ZMCmd0x08 * cmd = [[ZMCmd0x08 alloc]init];
    cmd.adminId = [self myAdminId];
    cmd.year = 24;
    cmd.month = 05;
    cmd.day = 22;
    cmd.hour = 15;
    int errCode = [_theDevice sendCmd:cmd retBlock4:^(ZMCmdResponse_4 * rsp,ZMDeviceError error) {
        if (error == ZMDeviceTimeout) {
            [SVProgressHUD showInfoWithStatus:@"超时洛"];
            return;
        }
        if (rsp.command == 8) {
            if (rsp.result != 0){
                 [SVProgressHUD showErrorWithStatus:[NSString stringWithFormat:@"Msg:%d",rsp.result]];
            }else{
                [SVProgressHUD showInfoWithStatus:@"请按手指采集指纹"];
            }
        }
        if (rsp.command == 9) {
            if (rsp.result != 0){
                [SVProgressHUD showErrorWithStatus:@"指纹采集失败"];
            }
            if (rsp.comd == 1){
                [SVProgressHUD showInfoWithStatus:@"请按手指再次采集指纹"];
            }
            if (rsp.comd == 2){
                [SVProgressHUD showInfoWithStatus:@"请按手指再次采集指纹"];
            }
        }
        if (rsp.command == 10){
            if (rsp.result == 0){
                [SVProgressHUD showSuccessWithStatus:@"指纹注册成功"];

            }else if (rsp.result == 1){
                [SVProgressHUD showErrorWithStatus:@"指纹注册失败"];

            }else if (rsp.result == 2){
                [SVProgressHUD showErrorWithStatus:@"指纹已注册"];
            }
        }
    }];
    if (errCode == ZMDeviceNotConnect) {
        [SVProgressHUD showInfoWithStatus:@"设备未连接"];
    }
}

- (IBAction)getAllFingerprints:(id)sender {
    ZMCMD0x0D * cmd = [[ZMCMD0x0D alloc]init];
    cmd.adminId =  [self myAdminId];
    int errCode = [_theDevice sendCmd:cmd retBlock3:^(ZMCmdResponse_3 * rsp,ZMDeviceError error) {
        if (error == ZMDeviceTimeout) {
            [SVProgressHUD showInfoWithStatus:@"超时洛"];
            return;
        }
        if (rsp.result != 0 && rsp.result != 3) {
            [SVProgressHUD showErrorWithStatus:[NSString stringWithFormat:@"Msg:%d",rsp.result]];
        }else{
            if (rsp.result == 3){
                [SVProgressHUD showInfoWithStatus:@"暂无指纹记录"];
            }
            if (rsp.printIdRecords.count){
                
            }
        }
    }];
    if (errCode == ZMDeviceNotConnect) {
        [SVProgressHUD showInfoWithStatus:@"设备未连接"];
    }
}

- (IBAction)deleteFingerPrint:(id)sender{
    ZMCmd0x0E * cmd = [[ZMCmd0x0E alloc]init];
    cmd.adminId = [self myAdminId];
    cmd.printID = 0; //传0删除全部指纹,如需删除相应指纹id则传对应的数值
    int errCode = [_theDevice sendCmd:cmd retBlock1:^(ZMCmdResponse_1 * rsp,ZMDeviceError error) {
        if (error == ZMDeviceTimeout) {
            [SVProgressHUD showInfoWithStatus:@"超时洛"];
            return;
        }
        if (rsp.result != 0) {
            [SVProgressHUD showErrorWithStatus:[NSString stringWithFormat:@"Msg:%d",rsp.result]];
        }else{
            [SVProgressHUD showSuccessWithStatus:@"指纹删除成功"];
        }
    }];
    if (errCode == ZMDeviceNotConnect) {
        [SVProgressHUD showInfoWithStatus:@"设备未连接"];
    }
}

- (IBAction)modifyFingerPrint:(id)sender {
    ZMCMD0x0F * cmd = [[ZMCMD0x0F alloc]init];
    cmd.adminId = [self myAdminId];
    cmd.printID = 1;//传需要修改的指纹ID
    cmd.year = 20;
    cmd.month = 5;
    cmd.day = 22;
    cmd.hour = 15;
    int errCode = [_theDevice sendCmd:cmd retBlock1:^(ZMCmdResponse_1 * rsp,ZMDeviceError error) {
        if (error == ZMDeviceTimeout) {
            [SVProgressHUD showInfoWithStatus:@"超时洛"];
            return;
        }
        if (rsp.result != 0) {
            [SVProgressHUD showErrorWithStatus:[NSString stringWithFormat:@"Msg:%d",rsp.result]];
        }else{
            [SVProgressHUD showSuccessWithStatus:@"修改成功"];
        }
    }];
    if (errCode == ZMDeviceNotConnect) {
        [SVProgressHUD showInfoWithStatus:@"设备未连接"];
    }
}
- (IBAction)resetLookToFactoryMode:(id)sender {
    UIAlertController *alert = [UIAlertController alertControllerWithTitle:@"您确定要初始化门锁吗?"
                                                                   message:@""
                                                            preferredStyle:UIAlertControllerStyleAlert];
    [alert addAction:[UIAlertAction actionWithTitle:@"取消"
                                              style:UIAlertActionStyleCancel handler:^(UIAlertAction * _Nonnull action) {
                                                  [alert dismissViewControllerAnimated:NO completion:nil];
                                              }]];
    [alert addAction:[UIAlertAction actionWithTitle:@"确定"
                                              style:UIAlertActionStyleDefault handler:^(UIAlertAction * _Nonnull action) {
                                                      [self resetFactoryMode];
                                                  [alert dismissViewControllerAnimated:NO completion:nil];
                                              }]];
    [self presentViewController:alert animated:NO completion:nil];
    
}


-(void)resetFactoryMode{
    ZMCmd0x14 * cmd = [[ZMCmd0x14 alloc]init];
    cmd.adminId = [self myAdminId];
    int errCode = [_theDevice sendCmd:cmd retBlock1:^(ZMCmdResponse_1 * rsp,ZMDeviceError error) {
        if (error == ZMDeviceTimeout) {
            [SVProgressHUD showInfoWithStatus:@"超时洛"];
            return;
        }
        if (rsp.result != 0) {
            [SVProgressHUD showErrorWithStatus:[NSString stringWithFormat:@"Msg:%d",rsp.result]];
        }else{
            [SVProgressHUD showSuccessWithStatus:@"门锁已初始化"];
        }
    }];
    if (errCode == ZMDeviceNotConnect) {
        [SVProgressHUD showInfoWithStatus:@"设备未连接"];
    }
}


- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    [super touchesBegan:touches withEvent:event];
    [self.view endEditing:YES];
}

- (void)onConnected:(ZMDevice *)device {
    [SVProgressHUD dismiss];
    [SVProgressHUD showInfoWithStatus:@"设备连接成功"];
    self.connectBtn.enabled = NO;
    self.disconnectBtn.enabled = YES;
}
- (void)onConnectFailed:(ZMDevice *)device error:(NSError *)error {
    [SVProgressHUD dismiss];
    [SVProgressHUD showInfoWithStatus:@"设备连接失败"];
    self.connectBtn.enabled = YES;
    self.disconnectBtn.enabled = NO;
}
- (void)onDisconneted:(ZMDevice *)device {
    [SVProgressHUD dismiss];
    [SVProgressHUD showInfoWithStatus:@"设备已断开"];
    self.connectBtn.enabled = YES;
    self.disconnectBtn.enabled = NO;
}

@end
