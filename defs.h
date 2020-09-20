/*
 * GRE Tunnel in userspace, netmap accelerated
 * (c) 2020 Joseph Hofmann <jh@combahton.net>
 */

/* args for tunnel thread */
struct args {
	char *ethif;
	int queue;
};

/* app config */
struct appconf {

};
