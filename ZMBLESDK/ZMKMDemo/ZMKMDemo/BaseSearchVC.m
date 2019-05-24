//
//  BaseSearchVC.m
//  ZMKMDemo
//
//  Created by humoroutlaw on 2018/9/17.
//  Copyright © 2018年 annidy. All rights reserved.
//

#import "BaseSearchVC.h"
#import "ViewController.h"

@interface BaseSearchVC ()

@property (weak, nonatomic) IBOutlet UITextField *searchText;

@end

@implementation BaseSearchVC

- (void)viewDidLoad {
    [super viewDidLoad];
    self.title = @"添加锁";
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (IBAction)touchAction:(id)sender {
    UIStoryboard * storyboard = [UIStoryboard storyboardWithName:@"Main" bundle:nil];
    ViewController * vc = [storyboard instantiateViewControllerWithIdentifier:@"ViewController"];
    [self.navigationController pushViewController:vc animated:YES];
}

-(void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender
{
    ViewController * vc = segue.destinationViewController;
    if (!self.searchText.text.length) {
        vc.nameArr = @[@"ZMKM",@"TBLK",@"ACTL",@"MWEL"];
    }else{
        if ([self.searchText.text containsString:@","]) {
            vc.nameArr = [self.searchText.text componentsSeparatedByString:@","];
        }else{
            vc.nameArr = @[self.searchText.text];
        }
    }
}

@end
