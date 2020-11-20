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
#include <net/netlink.h>
#include <net/sock.h>
#include <net/net_namespace.h>
#include <linux/rtnetlink.h>
#include <linux/skbuff.h>
#include <stdatomic.h>

#define MYGRP 31

MODULE_LICENSE("GPL");
MODULE_AUTHOR("zerol <zerolfx0@gmail.com>");
MODULE_DESCRIPTION("Record source/destination ip address and port as well as protocol for user-specific ip");


static char *ip;

module_param(ip, charp, S_IRUGO);

struct sock *nl_sk = NULL;

static struct nf_hook_ops nfho;


static void send_to_user(const char *buffer) {
    struct sk_buff *skb;
    struct nlmsghdr *nlh;

    int msg_size = strlen(buffer);
    int res;

    pr_info("Creating skb.\n");
    skb = nlmsg_new(NLMSG_ALIGN(msg_size), GFP_KERNEL);
    if (!skb) {
        pr_err("Allocation failure.\n");
        return;
    }

    nlh = nlmsg_put(skb, 0, 0, NLMSG_DONE, msg_size, 0);
    strncpy(nlmsg_data(nlh), buffer, msg_size);

    pr_info("Sending skb.\n");
    res = nlmsg_multicast(nl_sk, skb, 0, MYGRP, GFP_KERNEL);
    if (res < 0)
        pr_info("nlmsg_multicast() error: %d\n", res);
    else
        pr_info("Success.\n");
}


unsigned int hook_func(void *priv, struct sk_buff *skb, const struct nf_hook_state *state) {
    struct tcphdr *tcp_header;
    struct udphdr *udp_header;
    struct iphdr *ip_header;
    struct ethhdr *mac_header;
    static atomic_int counter;
    char output_buffer[200];

    ip_header = ip_hdr(skb);
    mac_header = eth_hdr(skb);

    if (ip_header->saddr == in_aton(ip)) {
        ++counter;
        sprintf(output_buffer, "%03d | src_ip: %pI4, dst_ip: %pI4, src_mac: %pM, dst_mac: %pM",
                counter,
                &ip_header->saddr, &ip_header->daddr,
                &mac_header->h_source, &mac_header->h_dest);



        switch (ip_header->protocol) {
            case IPPROTO_TCP:
                tcp_header = tcp_hdr(skb);
                sprintf(output_buffer + strlen(output_buffer),
                        ", protocol: TCP, src_port: %u, dst_port: %u",
                        tcp_header->source, tcp_header->dest);
                break;
            case IPPROTO_UDP:
                udp_header = udp_hdr(skb);
                sprintf(output_buffer + strlen(output_buffer),
                        ", protocol: UDP, src_port: %u, dst_port: %u",
                        udp_header->source, udp_header->dest);
                break;
            default:
                sprintf(output_buffer + strlen(output_buffer),
                        ", protocol: neither TCP nor UDP");
        }

        send_to_user(output_buffer);
    }

    return NF_ACCEPT;
}


static int __init init_main(void) {
    struct net *n;

    nl_sk = netlink_kernel_create(&init_net, NETLINK_USERSOCK, NULL);
    if (!nl_sk) {
        pr_err("Error creating socket.\n");
    }

    nfho.hook = hook_func;
    nfho.hooknum = NF_INET_LOCAL_IN;
    nfho.pf = PF_INET;
    nfho.priority = NF_IP_PRI_FIRST;

    for_each_net(n)
        nf_register_net_hook(n, &nfho);

    return 0;
}


static void __exit cleanup_main(void) {
    struct net *n;

    for_each_net(n)
        nf_unregister_net_hook(n, &nfho);

    netlink_kernel_release(nl_sk);
}

module_init(init_main)

module_exit(cleanup_main)