/*
 * GRE Tunnel in userspace, netmap accelerated
 * (c) 2020 Joseph Hofmann <jh@combahton.net>
 */

/* constants */
#define IP4_HDRLEN 20
#define GRE_HDRLEN 8 //nearly as tiny as udp

/* args for tunnel thread */
struct args {
	char *ethif;
	int queue;
};

/* app config */
struct appconf {

};
