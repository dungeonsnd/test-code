package main

import (
	"bytes"
	"encoding/binary"
	"fmt"
	"io"
	"net"
)

func Int32ToBytes(n int32) []byte {
	bytesBuf := bytes.NewBuffer([]byte{})
	binary.Write(bytesBuf, binary.BigEndian, n)
	return bytesBuf.Bytes()
}

func BytesToInt32(b []byte) int {
	bytesBuffer := bytes.NewBuffer(b)
	var x int32
	binary.Read(bytesBuffer, binary.BigEndian, &x)
	return int(x)
}

func Readn(c net.Conn, totalLen int) ([]byte, error) {
	left := totalLen
	res := make([]byte, totalLen)
	for {
		n, err := c.Read(res[totalLen-left : totalLen])
		if err != nil {
			return res, err
		}

		if left -= n; left <= 0 {
			break
		}
	}
	return res, nil
}

func Echo(c net.Conn) {
	defer c.Close()
	header := make([]byte, 4)
	var err error
	for {
		// read header
		header, err = Readn(c, 4)
		if err != nil {
			break
		}
		bodyLen := BytesToInt32(header)
		if bodyLen > 104857600 {
			fmt.Printf("bodyLen{%d} > 104857600 \n", bodyLen)
			break
		}

		// read body
		body := make([]byte, bodyLen+4)
		body, err = Readn(c, bodyLen)
		if err != nil {
			break
		}

		bufToWrite := append(Int32ToBytes(int32(len(body))), body...)

		nsent := 0
		nsent, err = c.Write(bufToWrite)
		if err != nil {
			break
		}
		if nsent != len(bufToWrite) {
			fmt.Printf("Waring! nsent{%d}!=len(bufToWrite){%d} \n", nsent, len(bufToWrite))
		}
	}

	if err != nil {
		if err == io.EOF {
			fmt.Printf("The RemoteAddr:%s is closed!\n", c.RemoteAddr().String())
		} else {
			fmt.Printf("error:%s \n", err.Error())
		}
	}
}

func main() {
	fmt.Printf("Server is ready...\n")
	l, err := net.Listen("tcp", ":18080")
	if err != nil {
		fmt.Printf("Failure to listen: %s\n", err.Error())
	}

	for {
		if c, err := l.Accept(); err == nil {
			go Echo(c)
		}
	}
}
