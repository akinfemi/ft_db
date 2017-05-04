#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

void error(char *msg)
{
	perror(msg);
	exit(0);
}

int main(int argc, char *argv[])
{
	int			sockfd;
	int			portno;
	int			n;
	struct sockaddr_in	serv_addr;
	struct hostent 		*server;
	char			buffer[1024 * 4];

	if (argc == 2)
		portno = atoi(argv[1]);
	else
		portno = 12345;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		error("ERROR opening socket");
	server = gethostbyname("localhost");
	if (server == NULL)
	{
		fprintf(stderr,"ERROR, no such host\n");
		exit(0);
	}
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(portno);
	if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
		error("ERROR connecting");
	while (1)
	{
		printf("> ");
		bzero(buffer, 1024 * 4);
		fgets(buffer, 256, stdin);
		if (strncmp(buffer, "exit", 5) == 0)
			exit (1);
		n = write(sockfd, buffer, strlen(buffer));
		if (n < 0)
			error("ERROR writing to socket");
		bzero(buffer, 256);
		n = read(sockfd, buffer, 1024 * 4);
		if (n < 0)
			error("ERROR reading from socket");
		printf("%s\n",buffer);
	}
	return (0);
}
