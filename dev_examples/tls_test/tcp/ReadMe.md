

    本工程是使用不同语言测试TLS传输，已测试通过
    
    测试时至少(不全)应该测试下面的情况:
    1) 测试APP中使用一个正常的自签名CA证书的情况
    2) 测试一下APP中使用一个伪造的CA证书的情况
    3) 测试一下客户端连接时收到了伪造的服务器证书。
    这种情况需要服务器端发送一个被另一个伪CA签名的证书的情况
    
    已测试过的环境有： ios(oc),c,golang
    证书生成方式： openssl,golang代码
    待测试的环境有:java,python
    其中，ios(oc)测试了上述三种情况，c/golang只测试了情况1)。
    
    参考资源:
    openssl证书生产过程
    http://blog.csdn.net/fly2010love/article/details/46415307
    http://blog.chinaunix.net/uid-20539097-id-64403.html
    http://blog.chinaunix.net/uid-20539097-id-64403.html
    
    openssl编程之客户端
    http://blog.csdn.net/fly2010love/article/details/46458805
    
    openssl编程之服务端
    http://blog.csdn.net/fly2010love/article/details/46458963
    
    golang版本
    https://github.com/disintegration/go-tls-client-server
    
    oc版本
    GCDAsyncSocket的官方示例
    https://github.com/robbiehanson/CocoaAsyncSocket/tree/master/Examples/GCD/SimpleHTTPClient
    AFNetworking的源码中关于 TLS 的部分，如
    https://github.com/AFNetworking/AFNetworking/blob/master/AFNetworking/AFSecurityPolicy.m

