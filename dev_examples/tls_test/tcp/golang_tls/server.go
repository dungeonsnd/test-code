package main

import (
	"crypto/tls"
	"crypto/x509"
	"fmt"
	"io"
	"io/ioutil"
	"log"
	"net"
	"strings"
)

const (
	appname = "tlstestapp"
	NeedVerifyClient = false
)

func main() {
	// load root certificate to verify client certificate	
	rootPEM, err := ioutil.ReadFile("../keys_gen_by_openssl/root_cert.pem")
	if err != nil {
		log.Fatalf("failed to read root certificate: %s", err)
	}
	roots := x509.NewCertPool()
	ok := roots.AppendCertsFromPEM([]byte(rootPEM))
	if !ok {
		log.Fatalf("failed to parse root certificate")
	}

	// load server certificate

	// correct case.
	// cert, err := tls.LoadX509KeyPair("../keys_gen_by_openssl/server_cert.pem", "../keys_gen_by_openssl/server_key.pem")

	// Attack case. Use server cert signed by a fake ca, simulate MITM.
	cert, err := tls.LoadX509KeyPair("../keys_gen_by_openssl/server_cert_signed_by_fake_ca_for_test_mitm.pem", "../keys_gen_by_openssl/server_key.pem")
	
	if err != nil {
		log.Fatalf("failed to load server tls certificate: %s", err)
	}

	var config tls.Config
	if NeedVerifyClient {
		config = tls.Config{
			ClientCAs:    roots,
			ClientAuth:   tls.RequireAndVerifyClientCert,
			Certificates: []tls.Certificate{cert},
		}
		
	} else {
		config = tls.Config{
			ClientCAs:    roots,
			ClientAuth:   tls.NoClientCert, ////
			Certificates: []tls.Certificate{cert},
		}

	}

	listener, err := tls.Listen("tcp", ":14444", &config)
	if err != nil {
		log.Fatalf("error: listen: %s", err)
	}
	log.Print("server started")

	for {
		conn, err := listener.Accept()
		if err != nil {
			log.Fatalf("error: accept: %s", err)
		}
		go handleConn(conn)
	}
}

func handleConn(conn net.Conn) {
	log.Printf("new client, conn=", conn)
	defer conn.Close()

	tlsConn, ok := conn.(*tls.Conn)
	if !ok {
		log.Printf("error: not tls conn")
		return
	}

	err := tlsConn.Handshake()
	if err != nil {
		log.Printf("error: handshake: %s", err)
		return
	}

	clientID, err := getClientID(tlsConn)
	if err != nil {
		log.Printf("warn: cannot get client-id: %s", err) ////
	}

	log.Printf("handle connection from client-id: %s", clientID)

	buf := make([]byte, 1024)
	for {
		n, err := conn.Read(buf)
		if err != nil {
			if err != io.EOF {
				log.Printf("client:%s: error: read: %s", clientID, err)
			}
			break
		}
		log.Printf("client:%s: read %d bytes", clientID, n)

		log.Printf("client:%s: echo: %s", clientID, string(buf[:n]))

		// n, err = conn.Write(buf[:n]) // echo back.
		message := "hi, this is from server."
		n, err = conn.Write([]byte(message))
		if err != nil {
			log.Printf("client:%s: error: write: %s", clientID, err)
			break
		}
		log.Printf("client:%s: wrote %d bytes", clientID, n)
	}
	log.Printf("client:%s: connection closed", clientID)
}

func getClientID(tlsConn *tls.Conn) (string, error) {
	state := tlsConn.ConnectionState()

	if len(state.PeerCertificates) == 0 {
		return "", fmt.Errorf("client certificate not found")
	}

	cert := state.PeerCertificates[0]

	parts := strings.Split(cert.Subject.CommonName, "-")
	if len(parts) != 3 || parts[0] != appname || parts[1] != "client" || len(parts[2]) == 0 {
		return "", fmt.Errorf("bad client common name")
	}

	return parts[2], nil
}
