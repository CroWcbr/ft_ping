/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send_v6.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdarrell <cdarrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/09 21:43:17 by cdarrell          #+#    #+#             */
/*   Updated: 2023/06/23 12:20:20 by cdarrell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

void	send_v6(void)
{
	static int			nsent;
	static char			sendbuf[BUFSIZE];
	int					len;
	struct icmp6_hdr	*icmp6;

	icmp6 = (struct icmp6_hdr *)sendbuf;
	icmp6->icmp6_type = ICMP6_ECHO_REQUEST;
	icmp6->icmp6_code = 0;
	icmp6->icmp6_id = g_ping.pid;
	icmp6->icmp6_seq = ++nsent;
	memset((icmp6 + 1), 0xa5, g_ping.datalen);
	gettimeofday((struct timeval *)(icmp6 + 1), NULL);
	len = 8 + g_ping.datalen;
	if (sendto(g_ping.sockfd, sendbuf, len, \
			0, g_ping.sasend, g_ping.salen) != (ssize_t)len)
	{
		printf("ft_ping: sendmsg: %s\n", strerror(errno));
		if (errno == 101 || errno == 99)
			exit(1);
	}
	g_ping.count_send++;
}
