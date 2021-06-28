#include "skb.h"

struct sk_buff* alloc_skb(struct net_device* netdev){

	struct sk_buff* skb = malloc(sizeof(struct sk_buff));
	skb->netdev = netdev;

	return skb;

}

void free_skb(struct sk_buff* skb){

	free(skb);
}

