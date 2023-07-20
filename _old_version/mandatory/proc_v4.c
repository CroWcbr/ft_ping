/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   proc_v4.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdarrell <cdarrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/18 17:46:33 by cdarrell          #+#    #+#             */
/*   Updated: 2022/08/18 20:10:11 by cdarrell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

static void	update_stats(struct icmp *icmp, struct ip *ip, \
							struct timeval *tvrecv, int icmplen)
{
	struct timeval		*tvsend;
	char				str[128];
	double				rtt;
	double				tmp;
	struct sockaddr_in	*tmp_addr;

	g_ping.count_recv++;
	tvsend = (struct timeval *)icmp->icmp_data;
	tv_sub(tvrecv, tvsend);
	rtt = tvrecv->tv_sec * 1000.0 + tvrecv->tv_usec / 1000.0;
	tmp_addr = (struct sockaddr_in *) g_ping.sarecv;
	if (inet_ntop(AF_INET, &tmp_addr->sin_addr, str, sizeof(str)) == NULL)
		printf("ERROR! inet_ntop");
	printf("%d bytes from %s (%s): icmp_seq=%u ttl=%d time=%.1f ms \n", \
	icmplen, g_ping.destination, str, icmp->icmp_seq, ip->ip_ttl, rtt);
	if (rtt < g_ping.rtt_min || g_ping.rtt_min == 0)
		g_ping.rtt_min = rtt;
	if (rtt > g_ping.rtt_max)
		g_ping.rtt_max = rtt;
	g_ping.rtt_sum += rtt;
	tmp = g_ping.rtt_sum / g_ping.count_recv - rtt;
	if (tmp < 0)
		tmp *= -1;
	g_ping.rtt_msum += tmp;
}

static void	print_verbose(struct icmp *icmp, int icmplen)
{
	char				str[128];
	struct sockaddr_in	*tmp_addr;

	tmp_addr = (struct sockaddr_in *) g_ping.sarecv;
	if (inet_ntop(AF_INET, &tmp_addr->sin_addr, str, sizeof(str)) == NULL)
		printf("ERROR! inet_ntop");
	printf(" %d bytes from %s: type = %d, code = %d\n", \
		icmplen, str, icmp->icmp_type, icmp->icmp_code);
}

void	proc_v4(char *ptr, ssize_t len, struct timeval *tvrecv)
{
	int				hlen1;
	int				icmplen;
	struct ip		*ip;
	struct icmp		*icmp;

	ip = (struct ip *)ptr;
	hlen1 = ip->ip_hl << 2;
	if (ip->ip_p != IPPROTO_ICMP)
		return ;
	icmp = (struct icmp *)(ptr + hlen1);
	icmplen = len - hlen1;
	if (icmplen < 8 || icmp->icmp_type == ICMP_ECHO)
		return ;
	if (icmp->icmp_type == ICMP_ECHOREPLY)
	{
		if (icmp->icmp_id != g_ping.pid)
			return ;
		if (icmplen < 16)
			return ;
		update_stats(icmp, ip, tvrecv, icmplen);
	}
	else if (g_ping.verbose)
		print_verbose(icmp, icmplen);
}
