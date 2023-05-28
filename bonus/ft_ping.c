/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdarrell <cdarrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/18 17:37:24 by cdarrell          #+#    #+#             */
/*   Updated: 2023/05/28 02:05:35 by cdarrell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

t_ping		g_ping;

void	sigint_handler(int signo)
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
	free(g_ping.destination_ip);
	free(g_ping.sarecv);
	exit(1);
}

void	sig_alrm(int signo)
{
	struct timeval	tmp;

	gettimeofday(&tmp, NULL);
	(void)signo;
	if ((g_ping.count != 0 && g_ping.count <= (size_t)g_ping.count_send) || \
		(g_ping.deadline != 0 && g_ping.deadline * 1000 < \
			(size_t)((tmp.tv_sec - g_ping.time_start.tv_sec) * 1000 + \
			(tmp.tv_usec - g_ping.time_start.tv_usec) / 1000)))
	{
		if (g_ping.count_send != g_ping.count_recv + g_ping.errors)
		{
			alarm(g_ping.interval);
			return ;
		}
		sigint_handler(0);
	}
	gettimeofday(&g_ping.time_end, NULL);
	g_ping.fsend();
	alarm(g_ping.interval);
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

void	sigint_quit(int signo)
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
	(void)argc;
	if (getuid() != 0)
		ft_exit("ft_ping: usage error: You must be root to run this program");
	init_ping();
	gettimeofday(&g_ping.time_start, NULL);
	parse(argc, argv);
	create_socket();
	signal(SIGALRM, sig_alrm);
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, sigint_quit);
	if (g_ping.ipv6)
		printf("PING %s (%s) %d bytes of data.\n", \
			g_ping.destination, g_ping.destination_ip, g_ping.datalen);
	else
		printf("PING %s (%s) %d(%ld) bytes of data.\n", \
			g_ping.destination, g_ping.destination_ip, g_ping.datalen, \
			g_ping.datalen + sizeof(struct icmp));
	sig_alrm(SIGALRM);
	readloop(0);
	return (0);
}
