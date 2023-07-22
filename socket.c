/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdarrell <cdarrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 15:30:19 by cdarrell          #+#    #+#             */
/*   Updated: 2023/07/22 19:19:40 by cdarrell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"
#include "./libft/include/libft.h"
#include <arpa/inet.h>		// inet_ntop
#include <sys/socket.h>	// setsocket
#include <stdio.h>

static void	print_socket()
{
	printf("---------------SOCKET-------------------\n");
	printf("destination_ip    = \t%s\n", g_ping.destination_ip);
	printf("destination_dns    = \t%s\n", g_ping.destination_dns);
	printf("-----------------------------------------\n");
}

void	create_socket()
{
	struct addrinfo		hints;
	ft_bzero(&hints, sizeof(struct addrinfo));
	hints.ai_flags = AI_CANONNAME;
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_RAW;
	hints.ai_protocol = IPPROTO_ICMP;

	int	gai_err;
	if ( (gai_err = getaddrinfo(g_ping.destination, NULL, &hints, &g_ping.ai)) != 0)
	{
		ft_exit_add_info(g_ping.destination, (char *)gai_strerror(gai_err));
	}
	if (g_ping.ai->ai_family != AF_INET)
	{
		ft_exit("ft_ping: unknown address family");
	}
	if (inet_ntop(AF_INET, &((struct sockaddr_in *)g_ping.ai->ai_addr)->sin_addr, g_ping.destination_ip, sizeof(g_ping.destination_ip)) == NULL)
	{
		ft_exit("ERROR! inet_ntop in check_destination");
	}
	if (getnameinfo(g_ping.ai->ai_addr, g_ping.ai->ai_addrlen, g_ping.destination_dns, NI_MAXHOST, NULL, 0, 0) != 0)
	{
		ft_exit_add_info(g_ping.destination_ip, (char *)gai_strerror(gai_err));
	}



	g_ping.sasend = g_ping.ai->ai_addr;
	g_ping.sarecv = ft_calloc(1, g_ping.ai->ai_addrlen);
	if (!g_ping.sarecv)
		ft_exit("ERROR! ft_calloc in create_socket");
	g_ping.salen = g_ping.ai->ai_addrlen;
	g_ping.sockfd = socket(g_ping.ai->ai_family, \
							g_ping.ai->ai_socktype, g_ping.ai->ai_protocol);
	if (g_ping.sockfd < 0)
		ft_exit("ft_ping: socket error");
	int	size = 60 *1024;
	if (setsockopt(g_ping.sockfd, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size)) < 0)
		ft_exit("ft_ping: setsockopt error: Could not set receive buffer");
	if (g_ping.flag.ttl)
	{
		if (setsockopt(g_ping.sockfd, IPPROTO_IP, IP_TTL, &g_ping.flag.ttl, sizeof(g_ping.flag.ttl)) < 0)
			ft_exit("ft_ping: setsockopt error: Could not set ttl");
	}

	if (DEBUG)
		print_socket();
}
