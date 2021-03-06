/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_ping.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdarrell <cdarrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/18 13:08:05 by cdarrell          #+#    #+#             */
/*   Updated: 2022/06/24 20:28:04 by cdarrell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

void	init_ping(void)
{
	g_ping.destination = NULL;
	g_ping.destination_ip = NULL;
	g_ping.help = false;
	g_ping.verbose = false;
	g_ping.ipv4 = false;
	g_ping.ipv6 = false;
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
	g_ping.rtt_min = 0;
	g_ping.rtt_sum = 0;
	g_ping.rtt_max = 0;
	g_ping.rtt_msum = 0;
}
