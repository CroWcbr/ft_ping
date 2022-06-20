/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdarrell <cdarrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/18 17:37:24 by cdarrell          #+#    #+#             */
/*   Updated: 2022/06/20 16:43:38 by cdarrell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

void	sig_alrm(int signo)
{
	gettimeofday(&g_ping.time_end, NULL);
	send_v4();
	alarm(1);
}

void	sigint_handler(int signo)
{
	tv_sub(&g_ping.time_end, &g_ping.time_start);
	printf("\n");
	if (g_ping.count_send > 0 || g_ping.count_recv > 0)
		printf("---%s ping statistics ---\n", g_ping.destination);
	if (g_ping.count_send > 0)
		printf("%d packets transmitted, %d received, \
%d%% packet loss, time %.0fms\n", \
				g_ping.count_send, \
				g_ping.count_recv, \
				(g_ping.count_send - g_ping.count_recv) / g_ping.count_send, \
				g_ping.time_end.tv_sec * 1000.0 + \
				g_ping.time_end.tv_usec / 1000.0);
	if (g_ping.count_recv > 0)
		printf ("rtt min/avg/max/mdev = %.3f/%.3f/%.3f/%.3f ms\n", \
			g_ping.rtt_min, g_ping.rtt_sum / g_ping.count_recv, \
			g_ping.rtt_max, g_ping.rtt_msum / g_ping.count_recv);
	freeaddrinfo(g_ping.ai);
	free(g_ping.destination_ip);
	free(g_ping.sarecv);
	exit(1);
}

void	tv_sub(struct timeval *out, struct timeval *in)
{
	out->tv_usec -= in->tv_usec;
	if (out->tv_usec < 0)
	{
		--out->tv_sec;
		out->tv_usec += 1000000;
	}
	out->tv_sec -= in->tv_sec;
}

int	main(int argc, char **argv)
{
	(void)argc;
	if (getuid() != 0)
		ft_exit("ft_ping: usage error: You must be root to run this program");
	init_ping();
	parse(argc, argv);
	create_socket();
	signal(SIGALRM, sig_alrm);
	signal(SIGINT, sigint_handler);
	printf("PING %s (%s) %d(%ld) bytes of data.\n", \
			g_ping.destination, g_ping.destination_ip, g_ping.datalen, \
			g_ping.datalen + sizeof(struct icmp) + sizeof(struct ip));
	gettimeofday(&g_ping.time_start, NULL);
	sig_alrm(SIGALRM);
	readloop();
	freeaddrinfo(g_ping.ai);
	free(g_ping.destination_ip);
	free(g_ping.sarecv);
	return (0);
}
