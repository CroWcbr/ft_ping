/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdarrell <cdarrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 01:50:38 by cdarrell          #+#    #+#             */
/*   Updated: 2023/07/23 14:22:47 by cdarrell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h> // freeadrrinfo
#include <netinet/ip_icmp.h>

t_ping		g_ping;

static void	init_ping(void)
{
	g_ping.destination = NULL;
	g_ping.destination_ip[0] = '\0';
	g_ping.destination_dns[0] = '\0';
	g_ping.pid = getpid() & 0xffff;;
	g_ping.datalen = 56;
	g_ping.sasend = NULL;
	g_ping.sarecv = NULL;
	g_ping.salen = 0;
	g_ping.sockfd = 0;

	g_ping.count_send = 0;
	g_ping.count_recv = 0;
	g_ping.errors = 0;
	g_ping.rtt_min = 0;
	g_ping.rtt_sum = 0;
	g_ping.rtt_max = 0;
	g_ping.rtt_msum = 0;

	g_ping.flag.verbose = false;
	g_ping.flag.timestamps = false;
	g_ping.flag.quiet = false;
	g_ping.flag.no_dns_name = false;
	g_ping.flag.count = 0;
	g_ping.flag.interval = 1;
	g_ping.flag.deadline = 0;
	g_ping.flag.ttl = 0;
}

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

static void	sigint_handler(int signo)
{
	(void)signo;
	tv_sub(&g_ping.time_end, &g_ping.time_start);
	printf("\n");
	if (g_ping.count_send > 0 || g_ping.count_recv > 0)
		printf("---%s ping statistics ---\n", g_ping.destination);
	if (g_ping.count_send > 0)
	{
		printf("%d packets transmitted, %d received, ", \
		g_ping.count_send, g_ping.count_recv);
		if (g_ping.errors > 0)
			printf("+%d errors, ", g_ping.errors);
		printf("%d%% packet loss, time %.0fms\n", \
		(g_ping.count_send - g_ping.count_recv) * 100 / g_ping.count_send, \
				g_ping.time_end.tv_sec * 1000.0 + \
				g_ping.time_end.tv_usec / 1000.0);
	}
	if (g_ping.count_recv > 0)
		printf ("rtt min/avg/max/mdev = %.3f/%.3f/%.3f/%.3f ms\n", \
			g_ping.rtt_min, g_ping.rtt_sum / g_ping.count_recv, \
			g_ping.rtt_max, g_ping.rtt_msum / g_ping.count_recv);
	freeaddrinfo(g_ping.ai);
	free(g_ping.sarecv);
	exit(1);
}

static void	sig_alrm(int signo)
{
	struct timeval	tmp;

	gettimeofday(&tmp, NULL);
	(void)signo;
	if ((g_ping.flag.count != 0 && g_ping.flag.count <= (size_t)g_ping.count_send) || \
		(g_ping.flag.deadline != 0 && g_ping.flag.deadline * 1000 < \
			(size_t)((tmp.tv_sec - g_ping.time_start.tv_sec) * 1000 + \
			(tmp.tv_usec - g_ping.time_start.tv_usec) / 1000)))
	{
		if (g_ping.count_send != g_ping.count_recv + g_ping.errors)
		{
			alarm(g_ping.flag.interval);
			return ;
		}
		sigint_handler(0);
	}
	gettimeofday(&g_ping.time_end, NULL);
	send_v4();
	alarm(g_ping.flag.interval);
}

static void	sigint_quit(int signo)
{
	(void)signo;
	printf("\r%d/%d packets, ", \
		g_ping.count_send, g_ping.count_recv);
	printf("%d%% loss, ", \
		(g_ping.count_send - g_ping.count_recv) * 100 / g_ping.count_send);
	printf ("min/avg/max/mdev = %.3f/%.3f/%.3f/%.3f ms", \
			g_ping.rtt_min, g_ping.rtt_sum / g_ping.count_recv, \
			g_ping.rtt_max, g_ping.rtt_msum / g_ping.count_recv);
	printf("\n");
}

int	main(int argc, char **argv)
{
	if (getuid() != 0)
		ft_exit("ft_ping: usage error: You must be root to run this program");
	init_ping();
	parse(argc, argv);
	if (g_ping.destination == NULL)
		ft_exit("ft_ping: usage error: Destination address required");
	create_socket();
	setuid(getuid());

	signal(SIGALRM, sig_alrm);
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, sigint_quit);

	printf("PING %s (%s) %d(%ld) bytes of data.\n", \
			g_ping.destination, g_ping.destination_ip, g_ping.datalen, \
			g_ping.datalen + sizeof(struct icmp));
	gettimeofday(&g_ping.time_start, NULL);
	sig_alrm(SIGALRM);
	readloop();

	freeaddrinfo(g_ping.ai);
	free(g_ping.sarecv);
	return(0);
}
