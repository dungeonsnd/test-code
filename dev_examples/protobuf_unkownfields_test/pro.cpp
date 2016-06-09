
#include "msgA.pb.h"
#include "msgB.pb.h"

#include <stdlib.h>
#include <stdio.h>
#include <string>

#include <google/protobuf/message.h>
using namespace google::protobuf;

int main(int argc, char * argv[])
{   
    MessageA msgA;
    msgA.set_f1(12345);
    msgA.set_f2("abcdef");
    msgA.set_f3("xyz");
    msgA.set_f4(6789); 
    std::string output;
    msgA.SerializeToString(&output);
    
    MessageB msgB;
    msgB.ParseFromArray(output.c_str(),output.size());
    printf("msgB f1=%d, f2=%s \n", msgB.f1(), msgB.f2().c_str());
    
    std::string output1;
    msgB.SerializeToString(&output1);
    
    MessageA msgA1;
    msgA1.ParseFromArray(output1.c_str(),output1.size());
    printf("msgA1 f1=%d, f2=%s, f3=%s , f4=%d  \n", 
        msgA1.f1(), msgA1.f2().c_str(), msgA1.f3().c_str(), msgA1.f4());
    
    return 0;
}

