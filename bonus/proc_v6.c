/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   proc_v6.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdarrell <cdarrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/09 21:43:26 by cdarrell          #+#    #+#             */
/*   Updated: 2022/08/19 23:29:06 by cdarrell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

static void	print_v6(struct icmp6_hdr *icmp6, double rtt, int hlim, ssize_t len)
{
	char				str[128];
	struct sockaddr_in6	*tmp_addr;
	struct timeval		end;

	if (g_ping.quiet)
		return ;
	if (g_ping.timestamps)
	{
		gettimeofday(&end, NULL);
		printf("[%f] \n", end.tv_sec + end.tv_usec / 1000000.0);
	}
	tmp_addr = (struct sockaddr_in6 *) g_ping.sarecv;
	if (inet_ntop(AF_INET6, &tmp_addr->sin6_addr, str, sizeof(str)) == NULL)
		printf("ERROR! inet_ntop\n");
	printf("%ld bytes from ", len);
	if (g_ping.dns_name)
		printf("%s (%s): ", g_ping.destination, str);
	else
		printf("%s: , ttl=", str);
	printf("icmp_seq=%u, ttl=", icmp6->icmp6_seq);
	if (hlim == -1)
		printf("???");
	else
		printf("%d", hlim);
	printf(", time=%.3f ms\n", rtt);
}

static int	find_hlim(struct msghdr *msg)
{
	int				hlim;
	struct cmsghdr	*cmsg;

	hlim = -1;
	cmsg = CMSG_FIRSTHDR(msg);
	while (cmsg != NULL)
	{
		if (cmsg->cmsg_level == IPPROTO_IPV6 && \
			cmsg->cmsg_type == IPV6_HOPLIMIT)
		{
			hlim = *(u_int32_t *)CMSG_DATA(cmsg);
			break ;
		}
		cmsg = CMSG_NXTHDR(msg, cmsg);
	}
	return (hlim);
}

static void	update_stats(struct icmp6_hdr *icmp6, \
							struct msghdr *msg, \
							struct timeval *tvrecv, \
							ssize_t len)
{
	struct timeval		*tvsend;
	double				rtt;
	int					hlim;
	double				tmp;

	tvsend = (struct timeval *)(icmp6 + 1);
	tv_sub(tvrecv, tvsend);
	rtt = tvrecv->tv_sec * 1000.0 + tvrecv->tv_usec / 1000.0;
	hlim = find_hlim(msg);
	g_ping.count_recv++;
	if (rtt < g_ping.rtt_min || g_ping.rtt_min == 0)
		g_ping.rtt_min = rtt;
	if (rtt > g_ping.rtt_max)
		g_ping.rtt_max = rtt;
	g_ping.rtt_sum += rtt;
	tmp = g_ping.rtt_sum / g_ping.count_recv - rtt;
	if (tmp < 0)
		tmp *= -1;
	g_ping.rtt_msum += tmp;
	print_v6(icmp6, rtt, hlim, len);
}

static void	print_verbose(struct icmp6_hdr *icmp6, ssize_t len)
{
	char				str[128];
	struct sockaddr_in6	*tmp_addr;

	tmp_addr = (struct sockaddr_in6 *) g_ping.sarecv;
	if (inet_ntop(AF_INET6, &tmp_addr->sin6_addr, str, sizeof(str)) == NULL)
		printf("ERROR! inet_ntop\n");
	printf("%ld bytes from %s: type = %d, code = %d\n", \
			len, str, icmp6->icmp6_type, icmp6->icmp6_code);
	g_ping.errors++;
}

void	proc_v6(char *ptr, \
				ssize_t len, \
				struct msghdr *msg, \
				struct timeval *tvrecv)
{
	struct icmp6_hdr	*icmp6;
	struct cmsghdr		*cmsg;

	icmp6 = (struct icmp6_hdr *) ptr;
	if (len < 8 || icmp6->icmp6_type == ICMP6_ECHO_REQUEST)
		return ;
	if (icmp6->icmp6_type == ICMP6_ECHO_REPLY)
	{
		if (icmp6->icmp6_id != g_ping.pid || len < 16)
			return ;
		update_stats(icmp6, msg, tvrecv, len);
	}
	else if (g_ping.verbose)
		print_verbose(icmp6, len);
}
