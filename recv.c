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
	//buf->text = (char *)malloc(sizeof(char)*1024);

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
