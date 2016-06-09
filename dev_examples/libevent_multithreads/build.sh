g++ -Wall -Wl,-rpath=libevent/libevent-2.0.21-stable/lib/:cppfastdevelop/cppfoundation/bin/  \
-g server.cpp io_cb.cpp thread_worker.cpp  -O0 -o server  \
-Llibevent/libevent-2.0.21-stable/lib -Lcppfastdevelop/cppfoundation/bin \
-Ilibevent/libevent-2.0.21-stable/include -Icppfastdevelop/cppfoundation/include  \
-levent -lcfclass_d -lcfstub_d
