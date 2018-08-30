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
	char *text;
}buf_t;


int main ()
{
	buf_t *buf = NULL;
	buf = (buf_t *)malloc(sizeof(buf_t));
	buf->text = (char *)malloc(sizeof(char)*1024);

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
