

本工程是使用不同语言测试TLS传输，已测试通过

已测试过的环境有： c,golang

证书生成方式： openssl,golang代码

待测试的环境有:ios(oc),java,python

其中，ios(oc)没有导入ca的证书即不验证服务器证书时通信成功，但是显然是不安全的，如何加载ca的证书还有待研究。


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
