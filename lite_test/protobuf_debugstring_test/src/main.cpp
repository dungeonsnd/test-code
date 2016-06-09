
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>

#include "message.pb.h"

int main(int argc, char* argv[]) {
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    // serialize
    std::string msgtype ="com.test.message.userinfo";
    unsigned userid =12345;
    std::string username ="zhangsan";
    printf("序列化之前的数据: \n------------------------------------\n");
    printf("msgtype : %s \n",msgtype.c_str());
    printf("userid : %u \n",userid);
    printf("username : %s \n",username.c_str());
    
    message::t ft;
    ft.set_msgtype(msgtype);
    
    message::userinfo* userinfo = ft.add_ui();
    userinfo->set_userid(userid);
    userinfo->set_username(username);
    
    std::string output;
    ft.SerializeToString(&output);
    
    printf("\n序列化之后数据: \n------------------------------------\n");
    for(size_t i=0;i<output.size();i++)
        printf("%X",output[i]);
    
    
    // deserialize
    printf("\n\n反序列化之后数据: \n------------------------------------\n");
    message::t ft1;
    ft1.ParseFromArray(output.c_str(),output.size());
    printf("msgtype : %s \n",ft1.msgtype().c_str());
    const message::userinfo & ui = ft1.ui(0);
    printf("userid : %u \n",ui.userid());
    printf("username : %s \n",ui.username().c_str());
    
    
    // debug string. 
    printf("\n调试输出数据: \n------------------------------------\n");
    std::string dbgstring =ft1.DebugString();
    printf("%s\n",dbgstring.c_str());
    
    google::protobuf::ShutdownProtobufLibrary();
    return 0;
}
