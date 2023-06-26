/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send_v4.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdarrell <cdarrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/18 17:40:57 by cdarrell          #+#    #+#             */
/*   Updated: 2023/06/23 12:20:14 by cdarrell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

uint16_t	in_cksum(uint16_t *addr, int len)
{
	int				nleft;
	uint32_t		sum;
	uint16_t		*w;
	uint16_t		answer;

	nleft = len;
	sum = 0;
	w = addr;
	answer = 0;
	while (nleft > 1)
	{
		sum += *w++;
		nleft -= 2;
	}
	if (nleft == 1)
	{
		*(unsigned char *)(&answer) = *(unsigned char *)w ;
		sum += answer;
	}
	sum = (sum >> 16) + (sum & 0xffff);
	sum += (sum >> 16);
	answer = ~sum;
	return (answer);
}

void	send_v4(void)
{
	static int		nsent;
	static char		sendbuf[BUFSIZE];
	int				len;
	struct icmp		*icmp;

	icmp = (struct icmp *)sendbuf;
	icmp->icmp_type = ICMP_ECHO;
	icmp->icmp_code = 0;
	icmp->icmp_id = g_ping.pid;
	icmp->icmp_seq = ++nsent;
	memset(icmp->icmp_data, 0xa5, g_ping.datalen);
	gettimeofday((struct timeval *)icmp->icmp_data, NULL);
	len = 8 + g_ping.datalen;
	icmp->icmp_cksum = 0;
	icmp->icmp_cksum = in_cksum((u_short *)icmp, len);
	if (sendto(g_ping.sockfd, sendbuf, len, \
			0, g_ping.sasend, g_ping.salen) != (ssize_t)len)
	{
		printf("ft_ping: sendmsg: %s\n", strerror(errno));
		if (errno == 101 || errno == 99)
			exit(1);
	}
	g_ping.count_send++;
}
