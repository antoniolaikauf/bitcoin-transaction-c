#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

/*
AF_INET         tipo di address
SOCK_STREAM     tcp


seed.bitcoin.sipa.be ← il più classico e affidabile
dnsseed.bluematt.me
seed.bitcoin.wiz.biz
dnsseed.emzy.de



*/
int main()
{

    const char *dns_seed = "seed.bitcoin.sipa.be";
    const char *network = "8333"; // mainet

    struct addrinfo hints, *res;

    hints.ai_family = AF_UNSPEC; // IPv4 o IPv6
    hints.ai_socktype = SOCK_STREAM;

    getaddrinfo(dns_seed, network, &hints, &res);

    printf("hello word\n");
    return 0;
}