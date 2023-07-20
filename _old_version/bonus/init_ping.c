/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_ping.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdarrell <cdarrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/18 13:08:05 by cdarrell          #+#    #+#             */
/*   Updated: 2022/08/19 23:20:23 by cdarrell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

static void	init_ping_options(void)
{
	g_ping.count = 0;
	g_ping.timestamps = false;
	g_ping.interval = 1;
	g_ping.dns_name = true;
	g_ping.quiet = false;
	g_ping.verbose = false;
	g_ping.deadline = 0;
	g_ping.ipv4 = false;
	g_ping.ipv6 = false;
}

void	init_ping(void)
{
	g_ping.destination = NULL;
	g_ping.destination_ip = NULL;
	init_ping_options();
	g_ping.pid = getpid() & 0xffff;
	g_ping.datalen = 56;
	g_ping.ai = NULL;
	g_ping.sasend = NULL;
	g_ping.sarecv = NULL;
	g_ping.salen = 0;
	g_ping.icmpproto = 0;
	g_ping.sockfd = -1;
	g_ping.count_send = 0;
	g_ping.count_recv = 0;
	g_ping.errors = 0;
	g_ping.rtt_min = 0;
	g_ping.rtt_sum = 0;
	g_ping.rtt_max = 0;
	g_ping.rtt_msum = 0;
}
