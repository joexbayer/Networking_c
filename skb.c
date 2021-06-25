#include "skb.h"

struct sk_buff* alloc_skb(){

	struct sk_buff* skb = malloc(sizeof(struct sk_buff));

	return skb;

}

