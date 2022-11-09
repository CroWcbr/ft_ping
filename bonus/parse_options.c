/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_options.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdarrell <cdarrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/11 20:30:25 by cdarrell          #+#    #+#             */
/*   Updated: 2022/08/18 23:11:10 by cdarrell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

static void	parse_ipv4_v6(char c)
{
	if (g_ping.ipv4 || g_ping.ipv6)
		ft_exit("ft_ping: only one -4 or -6 option may be specified");
	if (c == '4')
		g_ping.ipv4 = true;
	else
		g_ping.ipv6 = true;
}

static void	ft_exit_version(void)
{
	printf("Cdarrell best version\n");
	exit (0);
}

static void	ft_check_count(void)
{
	if (g_ping.count == 0)
	{
		printf("ping: invalid argument: '0': out of \
range: 1 <= value <= 9223372036854775807\n");
		exit (1);
	}
}

static void	ft_check_interval(void)
{
	if (g_ping.interval == 0)
	{
		printf("ping: cannot flood; minimal interval \
allowed for user is 1000ms\n");
		exit (1);
	}
}

void	parse_options_bonus(char *opt, char **argv, int *i, int *j)
{
	if (opt[*j] == 'c')
	{
		g_ping.count = find_int(opt, argv, i, j);
		ft_check_count();
	}
	else if (opt[*j] == 'D')
		g_ping.timestamps = true;
	else if (opt[*j] == 'i')
	{
		g_ping.interval = find_int(opt, argv, i, j);
		ft_check_interval();
	}
	else if (opt[*j] == 'n')
		g_ping.dns_name = false;
	else if (opt[*j] == 'q')
		g_ping.quiet = true;
	else if (opt[*j] == 'V')
		ft_exit_version();
	else if (opt[*j] == 'w')
		g_ping.deadline = find_int(opt, argv, i, j);
	else if (opt[*j] == '4' || opt[*j] == '6')
		parse_ipv4_v6(opt[*j]);
}
