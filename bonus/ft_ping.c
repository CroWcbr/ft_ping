/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdarrell <cdarrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/18 17:37:24 by cdarrell          #+#    #+#             */
/*   Updated: 2022/06/24 20:26:46 by cdarrell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

void	proc_v4(char *ptr, ssize_t len, struct timeval *tvrecv)
{
}

void	send_v4(void)
{
}

void	proc_v6(char *ptr, ssize_t len, struct timeval *tvrecv)
{
}

void	send_v6(void)
{
}

int	main(int argc, char **argv)
{
	(void)argc;
	if (getuid() != 0)
		ft_exit("ft_ping: usage error: You must be root to run this program");
	init_ping();
	parse(argc, argv);
	create_socket();
	printf ("dest : %s\n", g_ping.destination_ip);
	printf ("ipv4 : %d\n", g_ping.ipv4);
	printf ("ipv6 : %d\n", g_ping.ipv6);
	return (0);
}
