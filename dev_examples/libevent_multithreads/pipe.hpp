#ifndef _HEADER_FILE_PIPE_HPP_
#define _HEADER_FILE_PIPE_HPP_

#include "include_system.hpp"
#include "include_cpp.hpp"

class Pipe
{
public:
    int fd_r;
    int fd_w;
    Pipe()
    {
        int fds[2];
        int rt =pipe(fds);
        if(-1==rt)
            printf("Failed pipe,errno=%d,%s \n",errno,strerror(errno));
        fd_r =fds[0];
        fd_w =fds[1];
//        printf("Pipe , fd_r=%d,fd_w=%d \n",fd_r,fd_w);
    }
    ~Pipe()
    {
        close(fd_r);
        close(fd_w);
//        printf("~Pipe , fd_r=%d,fd_w=%d \n",fd_r,fd_w);
    }
};
typedef std::tr1::shared_ptr < Pipe > T_spPipe;

#endif // _HEADER_FILE_PIPE_HPP_
