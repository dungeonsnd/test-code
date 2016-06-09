package main

import (
    "bytes"
    "fmt"
    "strconv"
    "net/http"
)

func main(){
    httpclient := &http.Client{}
    
    con_count :=1
    exit :=make(chan int,con_count)
    
    for i:=0;i<con_count;i++{
        go func() {
            for i:=0;i<10000;i++{
                port :=4151
                topic :="mytopic"
                method :="put"
                endpoint := fmt.Sprintf("http://127.0.0.1:%d/%s?topic=%s", port, method, topic)
                body :=[]byte("msg "+strconv.Itoa(i))
                req, err := http.NewRequest("POST", endpoint, bytes.NewBuffer(body))
                resp, err := httpclient.Do(req)
                if err != nil {
                    fmt.Println(err.Error())
                    return
                }
                resp.Body.Close()
            }
            exit <- 1
        }()
    }
    
    for i:=0;i<con_count;i++{
        <-exit
    }    
}


