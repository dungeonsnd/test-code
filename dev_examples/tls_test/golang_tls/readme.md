### Go TLS Client-Server test
*Use both server and client authentication*

Generate root ca certificate:
```
$ go run gencert.go -type root
2014/10/28 00:19:53 certificate: root_cert.pem
2014/10/28 00:19:53 private key: root_key.pem
2014/10/28 00:19:53 common name: testapp-root
```

Generate server certificate signed by root certificate:
```
$ go run gencert.go -type server
2014/10/28 00:20:00 loading root_cert.pem and root_key.pem
2014/10/28 00:20:00 certificate: server_cert.pem
2014/10/28 00:20:00 private key: server_key.pem
2014/10/28 00:20:00 common name: testapp-server
```

Generate client certificate signed by root certificate:
```
$ go run gencert.go -type client -client-id=1
2014/10/28 00:20:10 loading root_cert.pem and root_key.pem
2014/10/28 00:20:10 certificate: client_1_cert.pem
2014/10/28 00:20:10 private key: client_1_key.pem
2014/10/28 00:20:10 common name: testapp-client-1
```

Server output:
```
$ go run server.go
2014/10/28 00:21:51 server started
2014/10/28 00:21:59 handle connection from client-id: 1
2014/10/28 00:21:59 client:1: read 13 bytes
2014/10/28 00:21:59 client:1: echo: hello-goodbye
2014/10/28 00:21:59 client:1: wrote 13 bytes
2014/10/28 00:21:59 client:1: connection closed
```

Client output:
```
$ go run client.go
2014/10/28 00:21:59 sending: hello-goodbye
2014/10/28 00:21:59 wrote 13 bytes
2014/10/28 00:21:59 read 13 bytes
2014/10/28 00:21:59 server reply: hello-goodbye
```