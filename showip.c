#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
    // Structure to hold address information
    struct addrinfo hints, *res, *p;
    int status;
    char ipstr[INET6_ADDRSTRLEN];

    // Check if hostname argument is provided
    if (argc != 2)
    {
        fprintf(stderr, "usage: showip hostname\n");
        return 1;
    }

    // Clearing hints structure
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;     // Allow IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // Use TCP

    // Retrieve address information corresponding to hostname
    if ((status = getaddrinfo(argv[1], NULL, &hints, &res)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return 2;
    }

    // Display IP addresses for the provided hostname
    printf("IP addresses for %s:\n\n", argv[1]);

    // Iterate through the linked list of address structures
    for (p = res; p != NULL; p = p->ai_next)
    {
        void *addr;
        char *ipver;

        // Determine IP version and cast address accordingly
        if (p->ai_family == AF_INET) // IPv4
        {
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
            addr = &(ipv4->sin_addr);
            ipver = "IPv4";
        }
        else // IPv6
        {
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
            addr = &(ipv6->sin6_addr);
            ipver = "IPv6";
        }

        // Convert binary IP address to presentation format
        inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
        printf(" %s: %s\n", ipver, ipstr);
    }

    freeaddrinfo(res); // Free dynamically allocated memory

    return 0;
}