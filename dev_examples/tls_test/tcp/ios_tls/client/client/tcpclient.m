//
//  tcpclient.m
//  client
//
//  Created by mxw on 16/7/11.
//  Copyright © 2016年 mtzijin. All rights reserved.
//

#import "tcpclient.h"
#import "GCDAsyncSocket.h"

@interface TcpClient()

@property (strong, nonatomic) GCDAsyncSocket * socket;

@end

@implementation TcpClient

-(instancetype)init{
    self =[super init];
    if (self) {
        self.socket = [[GCDAsyncSocket alloc] initWithDelegate:self delegateQueue:dispatch_get_main_queue()];
        [self connect];
        
    }
    return self;
}

-(void) connect{
    NSError *err = nil;
    if (![self.socket connectToHost:@"192.168.1.103" onPort:14444 error:&err]) // Asynchronous!
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
    dispatch_queue_t bgQueue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);
    dispatch_async(bgQueue, ^{
        completionHandler(YES);
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
