//
//  tcpclient.m
//  client
//
//  Created by mxw on 16/7/11.
//  Copyright © 2016年 mtzijin. All rights reserved.
//

#import "tcpclient.h"
#import "GCDAsyncSocket.h"

#import <AssertMacros.h> // __Require_noErr_Quiet

@interface TcpClient()

@property (strong, nonatomic) GCDAsyncSocket * socket;
@property (nonatomic, strong, nullable) NSSet <NSData *> *pinnedCertificates;

@end

@implementation TcpClient

-(instancetype)init{
    self =[super init];
    if (self) {
        [self setupTLS];
        self.socket = [[GCDAsyncSocket alloc] initWithDelegate:self delegateQueue:dispatch_get_main_queue()];
        [self connect];
        
    }
    return self;
}

-(void)setupTLS{
    // 1) 测试APP中使用一个正常的自签名CA证书的情况
    NSString * selfSignCACertFile =@"tlstestapp-root";
    
    // 2) 测试一下APP中使用一个伪造的CA证书的情况
//    NSString * selfSignCACertFile =@"tlstestapp-root_fake_ca_for_test";
    
    // 3) 测试一下客户端连接时收到了伪造的服务器证书。
    //    这种情况需要服务器端发送一个被另一个伪CA签名的证书的情况
    
    NSString *cerPath = [[NSBundle mainBundle] pathForResource:selfSignCACertFile ofType:@"cer"];
    NSData * certData =[NSData dataWithContentsOfFile:cerPath];
    NSSet * certSet = [[NSSet alloc] initWithObjects:certData, nil];
    
    self.pinnedCertificates =certSet;
}

static BOOL AFServerTrustIsValid(SecTrustRef serverTrust) {
    BOOL isValid = NO;
    SecTrustResultType result;
    __Require_noErr_Quiet(SecTrustEvaluate(serverTrust, &result), _out);
    
    isValid = (result == kSecTrustResultUnspecified || result == kSecTrustResultProceed);
    
_out:
    return isValid;
}

static NSArray * AFCertificateTrustChainForServerTrust(SecTrustRef serverTrust) {
    CFIndex certificateCount = SecTrustGetCertificateCount(serverTrust);
    NSMutableArray *trustChain = [NSMutableArray arrayWithCapacity:(NSUInteger)certificateCount];
    
    for (CFIndex i = 0; i < certificateCount; i++) {
        SecCertificateRef certificate = SecTrustGetCertificateAtIndex(serverTrust, i);
        [trustChain addObject:(__bridge_transfer NSData *)SecCertificateCopyData(certificate)];
    }
    
    return [NSArray arrayWithArray:trustChain];
}

- (BOOL)evaluateServerTrust:(SecTrustRef)serverTrust
                  forDomain:(NSString *)domain
{
    NSMutableArray *policies = [NSMutableArray array];
    [policies addObject:(__bridge_transfer id)SecPolicyCreateBasicX509()];
    
    SecTrustSetPolicies(serverTrust, (__bridge CFArrayRef)policies);
    
    NSMutableArray *pinnedCertificates = [NSMutableArray array];
    for (NSData *certificateData in self.pinnedCertificates) {
        [pinnedCertificates addObject:(__bridge_transfer id)SecCertificateCreateWithData(NULL, (__bridge CFDataRef)certificateData)];
    }
    SecTrustSetAnchorCertificates(serverTrust, (__bridge CFArrayRef)pinnedCertificates);
    
    if (!AFServerTrustIsValid(serverTrust)) {
        return NO;
    }
    
    // obtain the chain after being validated, which *should* contain the pinned certificate in the last position (if it's the Root CA)
    NSArray *serverCertificates = AFCertificateTrustChainForServerTrust(serverTrust);
    
    for (NSData *trustChainCertificate in [serverCertificates reverseObjectEnumerator]) {
        if ([self.pinnedCertificates containsObject:trustChainCertificate]) {
            return YES;
        }
    }
    
    return NO;
}


-(void) connect{
    NSError *err = nil;
    if (![self.socket connectToHost:@"192.168.1.102" onPort:14444 withTimeout:4 error:&err]) // Asynchronous!
    {
        // If there was an error, it's likely something like "already connected" or "no delegate set"
        NSLog(@"I goofed: %@", err);
    }
    
    NSDictionary *options = @{
                              GCDAsyncSocketManuallyEvaluateTrust : @(YES),
                              GCDAsyncSocketSSLPeerName : @"tlstestapp-server"
                              };
    
    NSLog(@"Requesting StartTLS with options:\n%@", options);
    [self.socket startTLS:options];
}


- (void)socket:(GCDAsyncSocket *)sock didReceiveTrust:(SecTrustRef)trust
completionHandler:(void (^)(BOOL shouldTrustPeer))completionHandler
{
    NSLog(@"didReceiveTrust");
    
    dispatch_queue_t bgQueue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);
    dispatch_async(bgQueue, ^{
        
        BOOL ret =[self evaluateServerTrust:trust forDomain:nil];
        completionHandler(ret);
    });
}

- (void)socketDidSecure:(GCDAsyncSocket *)sock
{
    // This method will be called if USE_SECURE_CONNECTION is set
    
    NSLog(@"socketDidSecure:");
    NSData *d =[@"This is from client!" dataUsingEncoding:NSUTF8StringEncoding];
    [self.socket writeData:d withTimeout:-1 tag:0];
    [self.socket readDataWithTimeout:-1 tag:0];
}

- (void)socket:(GCDAsyncSocket *)sender didConnectToHost:(NSString *)host port:(UInt16)port
{
    NSLog(@"didConnectToHost");
    
}


- (void)socket:(GCDAsyncSocket *)sender didReadData:(NSData *)data withTag:(long)tag
{
    NSString * s =[[NSString alloc]initWithData:data encoding:NSUTF8StringEncoding];
    NSLog(@"didReadData %lu:%@", (unsigned long)[data length], s);
}


- (void)socketDidDisconnect:(GCDAsyncSocket *)sock withError:(NSError *)err
{
    // Since we requested HTTP/1.0, we expect the server to close the connection as soon as it has sent the response.
    
    NSLog(@"socketDidDisconnect:withError: \"%@\"", err);
}



@end
