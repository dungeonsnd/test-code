//
//  ViewController.m
//  TestQiniuUploadMem
//
//  Created by jeffery on 16/9/4.
//  Copyright © 2016年 jeffery. All rights reserved.
//

#import "ViewController.h"
#import "QiniuSDK.h"

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.

}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (IBAction)onBtnStartUpload:(id)sender {
    [self testUpload];
}

- (void)testUpload {
    
    // TOKEN在这里赋值。
    NSString * token = @"MY-TEST-TOKEN";
    
    NSError *error = nil;
    NSString * dir = [NSTemporaryDirectory() stringByAppendingString:@"qiniutest"];
    QNFileRecorder *filerec = [QNFileRecorder fileRecorderWithFolder:dir
                                                               error:&error];
    if (error) {
        NSLog(@"recorder error %@", error);
    }
    
    QNUploadManager *upManager = [[QNUploadManager alloc] initWithRecorder:filerec];
    
    QNUpCancellationSignal cancellationSignalBlock =^BOOL(){
        return NO;
    };
    
    QNUploadOption *opt = [[QNUploadOption alloc] initWithMime:@"application/octet-stream"  // @"text/plain"
                                               progressHandler:^(NSString *key, float percent){
                                                   NSLog(@"key=%@, percent=%.5f", key, percent*100);
                                               }
                                                        params:@{ @"x:foo":@"fooval" }
                                                      checkCrc:YES
                                            cancellationSignal:cancellationSignalBlock ];
    
    NSString * inputFile =[[NSBundle mainBundle] pathForResource:@"bigfile" ofType:@"mov"];
    if (!inputFile) {
        NSLog(@"未找到上传的文件，请拖一个大文件并命名为 bigfile.mov到项目中!!!!!!!");
        return;
    }
    [upManager putFile:inputFile
                   key:@"only-test-key-2016-09-04"
                 token:token
              complete: ^(QNResponseInfo *info, NSString *key, NSDictionary *resp) {
                  NSLog(@"putFile complete, info= %@", info);
                  NSLog(@"putFile complete, resp= %@", resp);
              }
                option:opt];
}

@end
