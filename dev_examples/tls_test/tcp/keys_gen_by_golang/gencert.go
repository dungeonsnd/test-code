package main

import (
	"crypto/rand"
	"crypto/rsa"
	"crypto/x509"
	"crypto/x509/pkix"
	"encoding/pem"
	"flag"
	"io/ioutil"
	"log"
	"math/big"
	"os"
	"time"
)

const appname = "tlstestapp"

var (
	certtype = flag.String("type", "", "certificate type: root, server or client")
	clientID = flag.String("client-id", "", "client id")
)

func main() {
	flag.Parse()

	if *certtype != "root" && *certtype != "server" && *certtype != "client" {
		log.Fatalf("bad certificate type")
	}

	if *certtype == "client" && len(*clientID) == 0 {
		log.Fatalf("bad client id (required)")
	}

	priv, err := rsa.GenerateKey(rand.Reader, 2048)
	if err != nil {
		log.Fatalf("failed to generate private key: %s", err)
	}

	serialNumberLimit := new(big.Int).Lsh(big.NewInt(1), 128)
	serialNumber, err := rand.Int(rand.Reader, serialNumberLimit)
	if err != nil {
		log.Fatalf("failed to generate serial number: %s", err)
	}

	notBefore := time.Now()
	notAfter := notBefore.Add(2* 365 * 24 * time.Hour)

	var extKeyUsage []x509.ExtKeyUsage
	var isCA bool
	var commonName string
	var keyUsage x509.KeyUsage

	if *certtype == "root" {
		extKeyUsage = []x509.ExtKeyUsage{x509.ExtKeyUsageClientAuth, x509.ExtKeyUsageServerAuth}
		isCA = true
		commonName = appname + "-root"
		keyUsage = x509.KeyUsageKeyEncipherment | x509.KeyUsageDigitalSignature | x509.KeyUsageCertSign
	} else if *certtype == "server" {
		extKeyUsage = []x509.ExtKeyUsage{x509.ExtKeyUsageServerAuth}
		isCA = false
		commonName = appname + "-server"
		keyUsage = x509.KeyUsageKeyEncipherment | x509.KeyUsageDigitalSignature
	} else if *certtype == "client" {
		extKeyUsage = []x509.ExtKeyUsage{x509.ExtKeyUsageClientAuth}
		isCA = false
		commonName = appname + "-client-" + *clientID
		keyUsage = x509.KeyUsageKeyEncipherment | x509.KeyUsageDigitalSignature
	}

	var rootCert *x509.Certificate
	var rootKey interface{}

	template := x509.Certificate{
		SerialNumber: serialNumber,
		Subject:      pkix.Name{CommonName: commonName},
		NotBefore:    notBefore,
		NotAfter:     notAfter,
		KeyUsage:     keyUsage,
		ExtKeyUsage:  extKeyUsage,
		IsCA:         isCA,
		BasicConstraintsValid: true,
	}

	if *certtype == "root" {
		// root is self-signed certificate
		rootCert = &template
		rootKey = priv
	} else {
		// load root certificate/key to sign client or server certificate
		log.Print("loading root_cert.pem and root_key.pem")

		rootCertData, err := ioutil.ReadFile("root_cert.pem")
		if err != nil {
			log.Fatalf("failed to read root certificate: %s", err)
		}
		rootCertBlock, _ := pem.Decode(rootCertData)
		if rootCertBlock == nil {
			log.Println("failed to decode root certificate pem")
		}
		rootCert, err = x509.ParseCertificate(rootCertBlock.Bytes)
		if err != nil {
			log.Fatalf("failed to parse root certificate: %s", err)
		}

		rootKeyData, err := ioutil.ReadFile("root_key.pem")
		if err != nil {
			log.Fatalf("failed to read root private key: %s", err)
		}
		rootKeyBlock, _ := pem.Decode(rootKeyData)
		if rootKeyBlock == nil {
			log.Println("failed to decode root private key pem")
		}
		rootKey, err = x509.ParsePKCS1PrivateKey(rootKeyBlock.Bytes)
		if err != nil {
			log.Fatalf("failed to parse root private key: %s", err)
		}
	}

	certBytes, err := x509.CreateCertificate(rand.Reader, &template, rootCert, &priv.PublicKey, rootKey)
	if err != nil {
		log.Fatalf("failed to create certificate: %s", err)
	}

	var certfn, keyfn string

	if *certtype == "root" {
		certfn = "root_cert.pem"
		keyfn = "root_key.pem"
	} else if *certtype == "client" {
		certfn = "client_" + *clientID + "_cert.pem"
		keyfn = "client_" + *clientID + "_key.pem"
	} else if *certtype == "server" {
		certfn = "server_cert.pem"
		keyfn = "server_key.pem"
	}

	certOut, err := os.Create(certfn)
	if err != nil {
		log.Fatalf("failed to open %s for writing: %s", certfn, err)
	}
	pem.Encode(certOut, &pem.Block{Type: "CERTIFICATE", Bytes: certBytes})
	certOut.Close()
	log.Println("certificate:", certfn)

	keyOut, err := os.OpenFile(keyfn, os.O_WRONLY|os.O_CREATE|os.O_TRUNC, 0600)
	if err != nil {
		log.Print("failed to open %s for writing:", keyfn, err)
		return
	}
	pem.Encode(keyOut, &pem.Block{Type: "RSA PRIVATE KEY", Bytes: x509.MarshalPKCS1PrivateKey(priv)})
	keyOut.Close()
	log.Println("private key:", keyfn)

	log.Println("common name:", commonName)
}
