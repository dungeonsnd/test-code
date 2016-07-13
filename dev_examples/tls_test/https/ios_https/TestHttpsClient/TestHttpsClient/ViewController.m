//
//  ViewController.m
//  TestHttpsClient
//
//  Created by mxw on 16/7/13.
//  Copyright © 2016年 mtzijin. All rights reserved.
//

#import "ViewController.h"
#import "AFNetworking.h"

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    
    NSString *urlString = @"https://192.168.1.110/v1/user";
//    NSString *cerPath = [[NSBundle mainBundle] pathForResource:@"server_cert" ofType:@"pem"];
    NSString *cerPath = [[NSBundle mainBundle] pathForResource:@"tlstestapp-root" ofType:@"cer"];
    NSData * certData =[NSData dataWithContentsOfFile:cerPath];
    NSSet * certSet = [[NSSet alloc] initWithObjects:certData, nil];
    AFSecurityPolicy *securityPolicy = [AFSecurityPolicy policyWithPinningMode:AFSSLPinningModeCertificate];
    // 是否允许,NO-- 不允许无效的证书
    [securityPolicy setAllowInvalidCertificates:YES];
    securityPolicy.validatesDomainName = NO;
    // 设置证书
    [securityPolicy setPinnedCertificates:certSet];
    
    AFHTTPSessionManager *manager = [AFHTTPSessionManager manager];
    [manager setSecurityPolicy:securityPolicy];
    manager.responseSerializer = [AFHTTPResponseSerializer serializer];
    // request
    [manager GET:urlString parameters:nil progress:^(NSProgress * progress){
    } success:^(NSURLSessionDataTask *task, id responseObject) {
        NSLog(@"success");
        NSDictionary *d = [NSJSONSerialization JSONObjectWithData:responseObject options:NSJSONReadingMutableContainers error:nil];
        NSLog(@"OK === responseObject=%@",d);
    } failure:^(NSURLSessionDataTask *task, NSError *error) {
        NSLog(@"error ==%@",error.description);
    }];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
