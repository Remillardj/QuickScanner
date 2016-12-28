/*
*
* Author: Jaryd Remillard
* Version: 0.5.2
* 
* QuickScanner is a lightweight port scanner written in C.
* It takes input of the user of the IP address and number of ports to scan
* then prints if a port is opened.
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int check_port(char* ip_addr,int port_number)
{
	struct sockaddr_in target_socket;
	int socket_stream = socket(AF_INET, SOCK_STREAM, 0);

	// validate host IP address
	target_socket.sin_addr.s_addr = inet_addr(ip_addr);
	if (target_socket.sin_addr.s_addr == -1){
		printf("[ERROR]: Cannot create socket streams to host IP -> %s\n", ip_addr);
		exit(-1);
	}

	// connect socket stream to host IP
	target_socket.sin_family = AF_INET;
	target_socket.sin_port = htons(port_number);
	if (connect(socket_stream, (struct sockaddr *)&target_socket, sizeof(target_socket)) < 0) {
		close(socket_stream);
		return 0;
	} else {
		close(socket_stream);
		return 1;
	}
}


int main(int argc, char** argv) {
	int max_port_num;
	int open_port_num = 0;
	float start_time = clock();

	// validate argument counter and assign vectors
	if (argc == 2) {
		max_port_num = 49151; //49152 and above is reserved ephemeral ports
	} else if (argc == 3){
		max_port_num = atoi(argv[2]);
	} else {
		printf("[ERROR]: Please enter a host IP address or it contains too many arguments!");
		exit(-1);
	}

	// validate user max port number input
	if (max_port_num < 0 || max_port_num > 49151){
		printf("[ERROR]: Port index is out of range!");
		exit(-1);
	}

	// scan ports
	printf("Scanning host IP -> %s:0-%d\n", argv[1], max_port_num);
	for (int i = 0; i < max_port_num; i++){
		if (check_port(argv[1], i) > 0){
			printf("%d -> OPEN\n", i);
			open_port_num++;
		}
	}
	
	float cpu_time_used = (clock() - start_time) / CLOCKS_PER_SEC;
	printf("%d ports scanned in %f seconds\n", max_port_num, cpu_time_used);
	printf("%d ports open | %d ports closed\n", open_port_num, max_port_num-open_port_num);

	return 0;
}
