

    本工程是使用不同语言测试HTTPS传输，已测试通过
    
    已测试过的环境有： 
    客户端，ios(oc)，加载ca的证书
    服务端 beego(golang)，nginx
    
    证书生成方式： openssl，从 ../tcp/keys_gen_by_openssl/ 直接复制过来测试.
    
    待测试的环境有:java
    
    注意，要把openssl生成的pem格式用OSX的钥匙串访问导出成cer格式后再拖到 XCode中使用，否则运行时AFN请求会崩溃。
    服务器是 beego+nginx，服务端启动后，先在浏览器中访问 https://192.168.1.110/v1/user 看能否正常返回数据，如果正常再用 app来测试。
    
    参考资源:
    iOS中AFNetworking HTTPS的使用
    http://www.jianshu.com/p/20d5fb4cd76d
    iOS 开发笔记-AFNetWorking https SSL认证
    http://www.cnblogs.com/jys509/p/5001566.html
    
    nginx 部署
    http://beego.me/docs/deploy/nginx.md
    nginx 反向代理https http
    http://54im.com/linux/nginx-http-https.html

