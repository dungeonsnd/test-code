package main

import (
    "github.com/bitly/go-nsq"
    "fmt"
    "time"
)

type Handle struct{
    msgchan chan *nsq.Message
    stop bool
}

func (h *Handle) HandleMsg(m *nsq.Message) error {
    if !h.stop{
        h.msgchan <- m    
    }    
    return nil
}

func (h *Handle) Process(){
    h.stop = false
    for{
        select{
            case m := <-h.msgchan:
                fmt.Println(string(m.Body));
            case <-time.After(time.Second):
                if h.stop{
                    close(h.msgchan)
                    return
                }        
        }
    }
}

func (h *Handle) Stop(){
    h.stop = true
}

func main(){
    config := nsq.NewConfig()
    consumer, err := nsq.NewConsumer("mytopic", "mychannel", config)
    if err != nil{
        panic(err)
    }
    h := new(Handle)
    consumer.AddHandler(nsq.HandlerFunc(h.HandleMsg))
    h.msgchan = make(chan *nsq.Message, 1024)
    err = consumer.ConnectToNSQD("127.0.0.1:4150")
    if err != nil{
        panic(err)
    }
    h.Process()
}


