1、应用手册

https://github.com/nanomsg/nanomsg

    % mkdir build
    % cd build
    % cmake ..
    % cmake --build .
    % ctest .
    % sudo cmake --build . --target install
    % sudo ldconfig (if on Linux)

2、性能测试

dong@ubuntu:~/nanomsg-1.1.4/build$ ctest .
Test project /home/dong/nanomsg-1.1.4/build
      Start  1: inproc
 1/43 Test  #1: inproc ...........................   Passed    0.51 sec
      Start  2: inproc_shutdown
 2/43 Test  #2: inproc_shutdown ..................   Passed    0.26 sec
      Start  3: ipc
 3/43 Test  #3: ipc ..............................   Passed    0.71 sec
      Start  4: ipc_shutdown
 4/43 Test  #4: ipc_shutdown .....................   Passed    1.07 sec
      Start  5: ipc_stress
 5/43 Test  #5: ipc_stress .......................   Passed    1.52 sec
      Start  6: tcp
 6/43 Test  #6: tcp ..............................   Passed    0.65 sec
      Start  7: tcp_shutdown
 7/43 Test  #7: tcp_shutdown .....................   Passed    2.72 sec
      Start  8: ws
 8/43 Test  #8: ws ...............................   Passed    1.73 sec
      Start  9: pair
 9/43 Test  #9: pair .............................   Passed    0.00 sec
      Start 10: pubsub
10/43 Test #10: pubsub ...........................   Passed    0.12 sec
      Start 11: reqrep
11/43 Test #11: reqrep ...........................   Passed    0.11 sec
      Start 12: pipeline
12/43 Test #12: pipeline .........................   Passed    0.02 sec
      Start 13: survey
13/43 Test #13: survey ...........................   Passed    1.01 sec
      Start 14: bus
14/43 Test #14: bus ..............................   Passed    0.01 sec
      Start 15: async_shutdown
15/43 Test #15: async_shutdown ...................   Passed    2.02 sec
      Start 16: block
16/43 Test #16: block ............................   Passed    0.21 sec
      Start 17: term
17/43 Test #17: term .............................   Passed    0.11 sec
      Start 18: timeo
18/43 Test #18: timeo ............................   Passed    0.21 sec
      Start 19: iovec
19/43 Test #19: iovec ............................   Passed    0.00 sec
      Start 20: msg
20/43 Test #20: msg ..............................   Passed    0.03 sec
      Start 21: prio
21/43 Test #21: prio .............................   Passed    0.11 sec
      Start 22: poll
22/43 Test #22: poll .............................   Passed    0.15 sec
      Start 23: device
23/43 Test #23: device ...........................   Passed    0.21 sec
      Start 24: device4
24/43 Test #24: device4 ..........................   Passed    0.11 sec
      Start 25: device5
25/43 Test #25: device5 ..........................   Passed    0.11 sec
      Start 26: device6
26/43 Test #26: device6 ..........................   Passed    1.01 sec
      Start 27: device7
27/43 Test #27: device7 ..........................   Passed    1.01 sec
      Start 28: emfile
28/43 Test #28: emfile ...........................   Passed    0.05 sec
      Start 29: domain
29/43 Test #29: domain ...........................   Passed    0.00 sec
      Start 30: trie
30/43 Test #30: trie .............................   Passed    0.00 sec
      Start 31: list
31/43 Test #31: list .............................   Passed    0.00 sec
      Start 32: hash
32/43 Test #32: hash .............................   Passed    0.02 sec
      Start 33: stats
33/43 Test #33: stats ............................   Passed    0.51 sec
      Start 34: symbol
34/43 Test #34: symbol ...........................   Passed    0.00 sec
      Start 35: separation
35/43 Test #35: separation .......................   Passed    0.41 sec
      Start 36: zerocopy
36/43 Test #36: zerocopy .........................   Passed    0.00 sec
      Start 37: shutdown
37/43 Test #37: shutdown .........................   Passed    0.01 sec
      Start 38: cmsg
38/43 Test #38: cmsg .............................   Passed    0.01 sec
      Start 39: bug328
39/43 Test #39: bug328 ...........................   Passed    0.41 sec
      Start 40: bug777
40/43 Test #40: bug777 ...........................   Passed    0.00 sec
      Start 41: ws_async_shutdown
41/43 Test #41: ws_async_shutdown ................   Passed    1.08 sec
      Start 42: reqttl
42/43 Test #42: reqttl ...........................   Passed    0.21 sec
      Start 43: surveyttl
43/43 Test #43: surveyttl ........................   Passed    0.21 sec

100% tests passed, 0 tests failed out of 43

Total Test time (real) =  18.70 sec
dong@ubuntu:~/nanomsg-1.1.4/build$

3、demo

recv.c

#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include <assert.h>
#include <nanomsg/nn.h>
#include <nanomsg/reqrep.h>
#include <nanomsg/pubsub.h>
#include <nanomsg/pipeline.h>

const char *url = "ipc:///tmp/pipeline.ipc";

typedef struct{
    int type;
    char text[1024];
}buf_t;

int main ()
{
    buf_t *buf = NULL;
    buf = (buf_t *)malloc(sizeof(buf_t));

    int sock = nn_socket (AF_SP, NN_PULL);
    assert (sock >= 0);
    assert (nn_bind (sock, url) >= 0);
    while (1)
    {
        int bytes = nn_recv (sock, &buf, NN_MSG, 0);
        assert (bytes >= 0);
        printf ("NODE0: RECEIVED %d \"%s\"\n", buf->type, buf->text);
        nn_freemsg (buf);
    }
    return 1;
}


send.c

#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include <assert.h>
#include <nanomsg/nn.h>
#include <nanomsg/reqrep.h>
#include <nanomsg/pubsub.h>
#include <nanomsg/pipeline.h>

const char *url = "ipc:///tmp/pipeline.ipc";

typedef struct{
    int type;
    char text[1024];
}buf_t;


int main ()
{
    buf_t *buf = NULL;
    buf = (buf_t *)malloc(sizeof(buf_t));
    buf->type = 1;
    memset(buf->text,0,1024);
    memcpy(buf->text,"hello,world !",strlen("hello,world !"));

    int sz_msg = sizeof(buf_t);
    int sock = nn_socket (AF_SP, NN_PUSH);
    assert (sock >= 0);
    assert (nn_connect (sock, url) >= 0);
    printf ("NODE1: SENDING %d \"%s\"\n", buf->type,buf->text);
    int bytes = nn_send (sock, buf, sz_msg, 0);
    assert (bytes == sz_msg);
    return nn_shutdown (sock, 0);
}


编译运行

dong@ubuntu:~/nanomsg_demo$ gcc -o recv recv.c -lnanomsg
dong@ubuntu:~/nanomsg_demo$ ./recv
NODE0: RECEIVED 1 "hello,world !"
NODE0: RECEIVED 1 "hello,world !"
NODE0: RECEIVED 1 "hello,world !"

 

dong@ubuntu:~/nanomsg_demo$ gcc -o send send.c -lnanomsg
dong@ubuntu:~/nanomsg_demo$ ./send
NODE1: SENDING 1 "hello,world !"
dong@ubuntu:~/nanomsg_demo$ ./send
NODE1: SENDING 1 "hello,world !"
dong@ubuntu:~/nanomsg_demo$ ./send
NODE1: SENDING 1 "hello,world !"
dong@ubuntu:~/nanomsg_demo$

* 进程内通信（inproc）：url格式为inproc://test
* 进程间同in想（ipc）：url格式为ipc:///tmp/test.ipc
* tcp通信：url格式为tcp://*:5555

 

参考

nanomsg通信库的pubsub及survey
https://yq.aliyun.com/ziliao/829
https://yq.aliyun.com/articles/8694
https://www.oschina.net/code/snippet_1444806_49921
https://nanomsg.org/v0.1/nn_recv.3.html

1)Getting Started with nanomsg
https://blog.csdn.net/zsy19881226/article/details/56486176
2)This is a sample for p2p network based nanomsg
https://github.com/pch957/nanomsg_p2pnode
