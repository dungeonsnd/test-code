//
//  ViewController.m
//  client
//
//  Created by mxw on 16/7/11.
//  Copyright © 2016年 mtzijin. All rights reserved.
//

#import "ViewController.h"
#import "tcpclient.h"

@interface ViewController ()

@property (strong, nonatomic) TcpClient * tcpClient;
@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    self.tcpClient =[[TcpClient alloc]init];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
