#!/bin/bash

rm *.pem

echo '##### server #####'
echo '##### Common Name please use: tlstestapp-server'
openssl genrsa -out server_key.pem 2048 && openssl req -new -key server_key.pem -out server_cert_orinal.pem -config openssl.cnf -days 365 -utf8
sleep 1

echo '##### client #####'
echo '##### Common Name fetched by server is as clientID, please use tlstestapp-client-1000 for testing. see gocert.go .'
openssl genrsa -out client_1000_key.pem 2048 && openssl req -new -key client_1000_key.pem -out client_1000_cert_orinal.pem -config openssl.cnf -days 365 -utf8
sleep 1

echo '##### CA #####'
echo '##### Common Name please use: tlstestapp-root'
openssl req -new -x509 -keyout root_key.pem -out root_cert.pem -config openssl.cnf -days 3650 -utf8
echo '##### Common Name please use: tlstestapp-root_fake_ca_for_test'
#openssl req -new -x509 -keyout root_key_fake_ca_for_test.pem -out root_cert_fake_ca_for_test.pem -config openssl.cnf -days 3650 -utf8

echo '##### sign certificates #####'
echo '##### mkdir demoCA, see openssl.cnf'
mkdir demoCA
mkdir demoCA/newcerts
touch demoCA/index.txt
echo '0011' > demoCA/serial

echo '##### sign server cert ...'
openssl ca -in server_cert_orinal.pem -out server_cert.pem -cert root_cert.pem -keyfile root_key.pem -config openssl.cnf 
#openssl ca -in server_cert_orinal.pem -out server_cert_signed_by_fake_ca_for_test_mitm.pem -cert root_cert_fake_ca_for_test.pem -keyfile root_key_fake_ca_for_test.pem -config openssl.cnf
#### prevouse command failed to execute for error 'default is an unsupported message digest type'.
#### Note: need to remove demoCA dir and recreate again for next test command.
#openssl ca -in server_cert_orinal.pem -out server_cert_signed_by_fake_ca_for_test_mitm.pem -cert root_cert_fake_ca_for_test.pem -keyfile root_key_fake_ca_for_test.pem 

echo '##### sign client cert ...'
openssl ca -in client_1000_cert_orinal.pem -out client_1000_cert.pem -cert root_cert.pem -keyfile root_key.pem -config openssl.cnf 
