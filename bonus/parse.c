/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdarrell <cdarrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/18 13:01:21 by cdarrell          #+#    #+#             */
/*   Updated: 2022/06/24 19:48:34 by cdarrell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

static void	print_help(void)
{
	printf("\n");
	printf("Usage\n");
	printf("  ping [options] <destination>\n");
	printf("\n");
	printf("Options:\n");
	printf("  <destination>      dns name or ip address\n");
	printf("  -h                 print help and exit\n");
	printf("  -v                 verbose output\n");
	printf("\n");
	printf("IPv4 options:\n");
	printf("  -4                 use IPv4\n");
	printf("\n");
	printf("IPv6 options:\n");
	printf("  -6                 use IPv6\n");
}

static void	ft_exit_parse(char char_err)
{
	printf("ping: invalid option -- '%c'\n", char_err);
	print_help();
	exit(1);
}

static void	parse_ipv4_v6(char c)
{
	if (g_ping.ipv4 || g_ping.ipv6)
		ft_exit("ft_ping: only one -4 or -6 option may be specified");
	if (c == '4')
		g_ping.ipv4 = true;
	else
		g_ping.ipv6 = true;
}

static void	parse_options(char *opt)
{
	int	j;

	j = 1;
	while (opt[j])
	{
		if (opt[j] == 'v')
			g_ping.verbose = true;
		else if (opt[j] == 'h')
			g_ping.help = true;
		else if (opt[j] == '4' || opt[j] == '6')
			parse_ipv4_v6(opt[j]);
		else
			ft_exit_parse(opt[j]);
		j++;
	}
	if (j == 1)
		ft_exit_parse(opt[0]);
}

void	parse(int argc, char **argv)
{
	int		i;

	i = 1;
	while (i < argc)
	{
		if (argv[i][0] == '-')
			parse_options(argv[i]);
		else if (g_ping.destination == NULL)
			g_ping.destination = argv[i];
		else
			ft_exit("ft_ping: usage error: Multiple destination in cmd");
		i++;
	}
	if (g_ping.help == true)
	{
		print_help();
		exit (0);
	}
	if (g_ping.destination == NULL)
		ft_exit("ft_ping: usage error: Destination address required");
}
