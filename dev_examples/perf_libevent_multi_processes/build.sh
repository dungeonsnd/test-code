g++ -Wall -Wl,-rpath=libevent/libevent-2.0.21-stable/lib  -g server.cpp -O0 -o server \
-Llibevent/libevent-2.0.21-stable/lib -Ilibevent/libevent-2.0.21-stable/include \
-levent
