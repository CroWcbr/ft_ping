/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_socket.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdarrell <cdarrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/18 19:50:16 by cdarrell          #+#    #+#             */
/*   Updated: 2023/06/23 15:22:43 by cdarrell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

void	create_socket(void)
{
	int					size;
	char				str[128];

	memset(&g_ping.sa, 0, sizeof(g_ping.sa));
	g_ping.sa.sin_family = AF_INET;
	if (inet_pton(AF_INET, g_ping.destination, &(g_ping.sa.sin_addr)) <= 0)
		ft_exit("ERROR! Invalid destination address");
	if (inet_ntop(AF_INET, &(g_ping.sa.sin_addr), str, 128) == NULL)
		ft_exit("ERROR! inet_ntop in check_destination");
	g_ping.destination_ip = ft_strdup(str);
	if (!g_ping.destination_ip)
		ft_exit("ERROR! ft_strdup in check_destination");
	g_ping.sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (g_ping.sockfd < 0)
		ft_exit("ft_ping: socket error");
	setuid(getuid());
	size = 60 * 1024;
	if (setsockopt(g_ping.sockfd, SOL_SOCKET, \
					SO_RCVBUF, &size, sizeof(size)) < 0)
		ft_exit("ft_ping: setsockopt error: Could not set receive buffer");
	g_ping.sasend = (struct sockaddr *)&g_ping.sa;
	g_ping.salen = sizeof(g_ping.sa);
}
