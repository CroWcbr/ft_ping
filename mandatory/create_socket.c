/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_socket.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdarrell <cdarrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/18 19:50:16 by cdarrell          #+#    #+#             */
/*   Updated: 2022/06/18 22:00:35 by cdarrell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

static struct addrinfo	*check_destination(void)
{
	struct addrinfo		*ai;
	struct addrinfo		hints;
	int					gai_err;
	char				str[128];

	ft_bzero(&hints, sizeof(struct addrinfo));
	hints.ai_flags = AI_CANONNAME;
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_RAW;
	hints.ai_protocol = IPPROTO_ICMP;
	gai_err = getaddrinfo(g_ping.destination, NULL, &hints, &ai);
	if (gai_err)
		ft_exit_add_info(g_ping.destination, (char *)gai_strerror(gai_err));
	if (ai->ai_family != AF_INET)
		ft_exit("ft_ping: unknown address family");
	if (inet_ntop(AF_INET, \
			&((struct sockaddr_in *)ai->ai_addr)->sin_addr, \
			str, sizeof(str)) == NULL)
		ft_exit("ERROR! inet_ntop in check_destination");
	g_ping.destination_ip = ft_strdup(str);
	if (!g_ping.destination_ip)
		ft_exit("ERROR! ft_strdup in check_destination");
	return (ai);
}

void	create_socket(void)
{
	struct addrinfo	*ai;
	int				size;

	ai = check_destination();
	g_ping.ai = ai;
	g_ping.sasend = ai->ai_addr;
	g_ping.sarecv = ft_calloc(1, ai->ai_addrlen);
	if (!g_ping.sarecv)
		ft_exit("ERROR! ft_calloc in check_destination");
	g_ping.salen = ai->ai_addrlen;
	g_ping.sockfd = socket(ai->ai_family, \
							ai->ai_socktype, ai->ai_protocol);
	if (g_ping.sockfd < 0)
		ft_exit("ft_ping: socket error");
	size = 60 * 1024;
	if (setsockopt(g_ping.sockfd, SOL_SOCKET, \
					SO_RCVBUF, &size, sizeof(size)) < 0)
		ft_exit("ft_ping: setsockopt error: Could not set receive buffer");
}
