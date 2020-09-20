/*
 * GRE Tunnel in userspace, netmap accelerated
 * (c) 2020 Joseph Hofmann <jh@combahton.net>
 */

/* gre packet tx */
int packet_gre_tx() {
	for (t_i = nm_desc_tx->first_tx_ring; t_i <= nm_desc_tx->last_tx_ring; ++t_i) {
    		txring = NETMAP_TXRING(nm_desc_tx->nifp, t_i);
    		if (nm_ring_empty(txring)) {
      			if (t_i == nm_desc_tx->last_tx_ring) {
       	 			pending_txring = NETMAP_TXRING(nm_desc_tx->nifp, nm_desc_tx->first_tx_ring);
        			while(nm_tx_pending(pending_txring)) {
          				ioctl(nm_desc_tx->fd, NIOCTXSYNC, NULL);
        			}
      			}
      			continue;
    		}

    		t_cur = txring->cur;
    		t_buf_i = txring->slot[t_cur].buf_idx;

    		struct iphdr iphdr;
    		iphdr.saddr = ip->daddr;
    		iphdr.daddr = ip->saddr;
    		iphdr.version = 4;
    		iphdr.tos = 0;
    		iphdr.ihl = 5;
    		iphdr.tot_len = htons(sizeof(struct iphdr) + sizeof(struct udphdr) + datalen);
    		iphdr.protocol = 47; //47 = GRE
    		iphdr.id = htons(0);
    		iphdr.ttl = appconf->tx_ip_ttl;
    		//iphdr.check = 0;
    		iphdr.check = checksum ((uint16_t *) &iphdr2, sizeof(struct iphdr));

		memcpyASM(packet, &ethhdr, ETH_HLEN);
    		memcpyASM(packet + ETH_HLEN, &iphdr2, IP4_HDRLEN * sizeof (uint8_t));
    		memcpyASM((packet + ETH_HLEN + IP4_HDRLEN), &grehdr, GRE_HDRLEN * sizeof (uint8_t));
    		memcpyASM(packet + ETH_HLEN + IP4_HDRLEN + GRE_HDRLEN, dataplain, datalen * sizeof (uint8_t));

   		buf2 = NETMAP_BUF(txring, txring->slot[t_cur].buf_idx);
    		nm_pkt_copy(packet, buf2, (ETH_HLEN + IP4_HDRLEN + GRE_HDRLEN + datalen));
    		txring->slot[t_cur].len = (ETH_HLEN + IP4_HDRLEN + GRE_HDRLEN + datalen);
    		txring->slot[t_cur].flags |= NS_BUF_CHANGED;     
    		txring->head = txring->cur = nm_ring_next(txring, t_cur);
		break; //tx finished
	}
}

/* packet handler which deals with gre packets rx/tx */
int packet_handler(char *ethif, int queue){

}
