#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/skbuff.h>
#include <linux/udp.h>
#include <linux/tcp.h>
#include <linux/ip.h>
#include <linux/inet.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("zerol <zerolfx0@gmail.com>");
MODULE_DESCRIPTION("Record source/destination ip address and port as well as protocol for user-specific ip");


static char *ip;
module_param(ip, charp, S_IRUGO);

static struct nf_hook_ops nfho;

unsigned int hook_func(void *priv, struct sk_buff *skb, const struct nf_hook_state *state) {
  struct tcphdr *tcp_header;
  struct udphdr *udp_header;
  struct iphdr *ip_header;
  struct ethhdr *mac_header;

  ip_header = ip_hdr(skb);
  mac_header = eth_hdr(skb);

  if (ip_header->saddr == in_aton(ip)) {
    printk(KERN_INFO "src_ip: %pI4, dst_ip: %pI4\n", &ip_header->saddr, &ip_header->daddr);
    printk(KERN_INFO "src_mac: %pM, dst_mac: %pM\n", &mac_header->h_source, &mac_header->h_dest);
    switch (ip_header->protocol) {
      case IPPROTO_TCP:
        tcp_header = tcp_hdr(skb);
        printk(KERN_INFO "protocol: TCP\n");
        printk(KERN_INFO "src_port: %u, dst_port: %u\n", tcp_header->source, tcp_header->dest);
        break;
      case IPPROTO_UDP:
        udp_header = udp_hdr(skb);
        printk(KERN_INFO "protocol: UDP\n");
        printk(KERN_INFO "src_port: %u, dst_port: %u\n", udp_header->source, udp_header->dest);
        break;
      default:
        printk(KERN_INFO "protocol: neither TCP nor UDP\n");
    }
  }

  return NF_ACCEPT;
}



static int __init init_main(void) {
  struct net* n;

  nfho.hook     = hook_func;
  nfho.hooknum  = NF_INET_LOCAL_IN;
  nfho.pf       = PF_INET;
  nfho.priority = NF_IP_PRI_FIRST;

  for_each_net(n)
    nf_register_net_hook(n, &nfho);

  return 0;
}


static void __exit cleanup_main(void) {
  struct net* n;

  for_each_net(n)
      nf_unregister_net_hook(n, &nfho);
}

module_init(init_main)
module_exit(cleanup_main)