/*
 * GRE Tunnel in userspace, netmap accelerated
 * (c) 2020 Joseph Hofmann <jh@combahton.net>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <poll.h>
#define NETMAP_WITH_LIBS
#include <net/netmap_user.h>
#include <net/ethernet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ether.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <libconfig.h>
#include <time.h>
#include <pthread.h>
#include "defs.h"
#include "config.h"
#include "lib/tunnel.h"

int main(int argc, char* argv[]) {

	/* Load app config */
	init_config();

	int tunnel_t_num=1;

	/* Threads for multiple tunnels */
	void *tunnel_t(void *t_args) {
		packet_handler(t_args->ethif, t_args->queue);
	}

	pthread_t tunnel_t[tunnel_t_num];
  	if(pthread_create(&tunnel_t[tunnel_t_num], NULL, tunnel_t, (void *)&args)) {
    		fprintf(stderr, "Error creating one of the tunnel threads\n");
    		exit(1);
  	}


}
