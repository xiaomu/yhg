/*****************************************************************************
 *  Copyright          :  2012  All Rights Reserved.
 *
 *  Date               :  2012-04-20 12:04:58
 *  Author             :  Dengzhaoqun
 *****************************************************************************/
// test when ip is self's, socket can do normal.
// result: when client's connect ip is itself's, it connect succeed， just as normal.

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

int main()
{
	int server_sockfd, client_sockfd;
	int server_len, client_len;
	struct sockaddr_in server_address;
	struct sockaddr_in client_address;

	server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);
	server_address.sin_port = htons(9734);
	server_len = sizeof(server_address);
	bind(server_sockfd, (struct sockaddr *)&server_address, server_len);

	listen(server_sockfd, 5);

	client_len = sizeof(client_address);

	while(1)
	{
		client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, &client_len);
	
		char buf[10];
		memset(buf, 0, 10);
		read(client_sockfd, buf, 10);
		printf("connect %s\n", buf);
		printf("ip: %s\n", inet_ntoa(client_address.sin_addr));
		close(client_sockfd);
	}
	close(server_sockfd);

	return 0;
}
