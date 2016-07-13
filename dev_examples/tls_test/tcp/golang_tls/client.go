package main

import (
	"crypto/tls"
	"crypto/x509"
	"io/ioutil"
	"log"
)

const (
	appname = "tlstestapp"
	NeedVerifyClient = true
)

func main() {
	// load root certificate to verify server certificate
	rootPEM, err := ioutil.ReadFile("../keys_gen_by_openssl/root_cert.pem")
	if err != nil {
		log.Fatalf("failed to read root certificate: %s", err)
	}
	roots := x509.NewCertPool()
	ok := roots.AppendCertsFromPEM([]byte(rootPEM))
	if !ok {
		panic("failed to parse root certificate")
	}

	var config tls.Config
	if NeedVerifyClient {
		
		// load client certificate
		cert, err := tls.LoadX509KeyPair("../keys_gen_by_openssl/client_1000_cert.pem", "../keys_gen_by_openssl/client_1000_key.pem")
		if err != nil {
			log.Fatalf("failed to load client tls certificate: %s", err)
		}

		config = tls.Config{
			RootCAs:      roots,
			ServerName:   appname + "-server",
			Certificates: []tls.Certificate{cert},
		}
		
	} else {
		config = tls.Config{
			RootCAs:      roots,
			ServerName:   appname + "-server",
		}

	}


	conn, err := tls.Dial("tcp", "127.0.0.1:14444", &config)
	if err != nil {
		log.Fatalf("error: dial: %s", err)
	}
	defer conn.Close()

	message := "hello, this is from client."
	log.Printf("sending: %s", message)

	n, err := conn.Write([]byte(message))
	if err != nil {
		log.Fatalf("error: write: %s", err)
	}
	log.Printf("wrote %d bytes", n)

	buf := make([]byte, 1024)
	n, err = conn.Read(buf)
	if err != nil {
		log.Fatalf("error: read: %s", err)
	}
	log.Printf("read %d bytes", n)

	log.Printf("server reply: %s", string(buf[:n]))
}
