/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   proc_v4.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdarrell <cdarrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/18 17:46:33 by cdarrell          #+#    #+#             */
/*   Updated: 2023/07/23 18:50:42 by cdarrell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"
#include <stdio.h>
#include <errno.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>		// inet_ntop
#include <string.h>

static void	tv_sub(struct timeval *out, struct timeval *in)
{
	out->tv_usec -= in->tv_usec;
	if (out->tv_usec < 0)
	{
		--out->tv_sec;
		out->tv_usec += 1000000;
	}
	out->tv_sec -= in->tv_sec;
}

static void	print_v4(struct icmp *icmp, struct ip *ip, double rtt, int icmplen)
{
	char				str[128];
	struct sockaddr_in	*tmp_addr;
	struct timeval		end;

	if (g_ping.flag.quiet)
		return ;
	if (g_ping.flag.timestamps)
	{
		gettimeofday(&end, NULL);
		printf("[%f] ", end.tv_sec + end.tv_usec / 1000000.0);
	}
	tmp_addr = (struct sockaddr_in *) g_ping.sarecv;
	if (inet_ntop(AF_INET, &tmp_addr->sin_addr, str, sizeof(str)) == NULL)
		printf("ERROR! inet_ntop");
	printf("%d bytes from ", icmplen);
	if (!g_ping.flag.no_dns_name)
		printf("%s (%s): icmp_seq=%u ttl=%d time=%.2f ms \n", \
		g_ping.destination_dns, str, icmp->icmp_seq, ip->ip_ttl, rtt);
	else
		printf("%s: icmp_seq=%u ttl=%d time=%.2f ms \n", \
		str, icmp->icmp_seq, ip->ip_ttl, rtt);
}

static void	update_stats(struct icmp *icmp, struct ip *ip, \
							struct timeval *tvrecv, int icmplen)
{
	struct timeval		*tvsend;
	double				rtt;
	double				tmp;

	g_ping.count_recv++;
	tvsend = (struct timeval *)icmp->icmp_data;
	tv_sub(tvrecv, tvsend);
	rtt = tvrecv->tv_sec * 1000.0 + tvrecv->tv_usec / 1000.0;
	if (rtt < g_ping.rtt_min || g_ping.rtt_min == 0)
		g_ping.rtt_min = rtt;
	if (rtt > g_ping.rtt_max)
		g_ping.rtt_max = rtt;
	g_ping.rtt_sum += rtt;
	tmp = g_ping.rtt_sum / g_ping.count_recv - rtt;
	if (tmp < 0)
		tmp *= -1;
	g_ping.rtt_msum += tmp;
	print_v4(icmp, ip, rtt, icmplen);
}

static void	print_verbose(struct icmp *icmp, int icmplen)
{
	char				str_ip[128];
	char				str_dns[128];
	struct timeval		end;

	(void)icmplen;

	
	g_ping.errors++;
	if (g_ping.flag.quiet)
		return ;
	if (g_ping.flag.timestamps)
	{
		gettimeofday(&end, NULL);
		printf("[%f] ", end.tv_sec + end.tv_usec / 1000000.0);
	}

	int	gai_err = 0;
	if (inet_ntop(AF_INET, &((struct sockaddr_in *)g_ping.sarecv)->sin_addr, str_ip, sizeof(str_ip)) == NULL)
	{
		printf("ERROR! inet_ntop in print_verbose\n");
		return ;
	}
	if (getnameinfo(g_ping.sarecv, g_ping.salen, str_dns, sizeof(str_dns), NULL, 0, 0) != 0)
	{
		printf("ERROR! getnameinfo in print_verbose %s : %s\n", str_dns, (char *)gai_strerror(gai_err));
		return ;
	}

	printf("From ");
	if (!g_ping.flag.no_dns_name)
		printf("%s (%s)", str_dns, str_ip);
	else
		printf("%s", str_ip);
	printf(" icmp_seq=%d type=%d code=%d", *(uint16_t*)((char*)&(icmp->icmp_seq) + 28), icmp->icmp_type, icmp->icmp_code);
	printf(" - %s\n", (icmp->icmp_type == 11 && icmp->icmp_code == 0) ? "ICMP_TIME_EXCEEDED" : " look error table https://en.wikipedia.org/wiki/Internet_Control_Message_Protocol");

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
	else if (g_ping.flag.verbose)
	{
		if (*(uint16_t*)((char*)&(icmp->icmp_id) + 28) != g_ping.pid)
			return ;
		g_ping.count_recv++;
		print_verbose(icmp, icmplen);
	}

}
