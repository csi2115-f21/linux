/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _NF_DEFRAG_IPV6_H
#define _NF_DEFRAG_IPV6_H

#include <linux/skbuff.h>
#include <linux/types.h>

int nf_defrag_ipv6_enable(struct net *);

int nf_ct_frag6_init(void);
void nf_ct_frag6_cleanup(void);
int nf_ct_frag6_gather(struct net *net, struct sk_buff *skb, u32 user);

struct inet_frags_ctl;

<<<<<<< HEAD
=======
struct nft_ct_frag6_pernet {
	struct ctl_table_header *nf_frag_frags_hdr;
	struct fqdir	*fqdir;
	unsigned int users;
};

>>>>>>> parent of 515dcc2e0217... Merge tag 'dma-mapping-5.15-2' of git://git.infradead.org/users/hch/dma-mapping
#endif /* _NF_DEFRAG_IPV6_H */
