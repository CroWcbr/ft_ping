/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdarrell <cdarrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/18 13:01:21 by cdarrell          #+#    #+#             */
/*   Updated: 2022/08/19 23:34:28 by cdarrell         ###   ########.fr       */
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
	printf("  -c <count>         stop after <count> replies\n");
	printf("  -D                 print timestamps\n");
	printf("  -h                 print help and exit\n");
	printf("  -i <interval>      seconds between sending each packet\n");
	printf("  -n                 no dns name resolution\n");
	printf("  -q                 quiet output\n");
	printf("  -v                 verbose output\n");
	printf("  -V                 print version and exit\n");
	printf("  -w <deadline>      reply wait <deadline> in seconds\n");
	printf("\n");
	printf("IPv4 options:\n");
	printf("  -4                 use IPv4\n");
	printf("\n");
	printf("IPv6 options:\n");
	printf("  -6                 use IPv6\n");
	printf("\n");
}

static void	ft_exit_parse(char char_err)
{
	printf("ping: invalid option -- '%c'\n", char_err);
	print_help();
	exit(1);
}

static void	ft_exit_help(void)
{
	print_help();
	exit (0);
}

static void	parse_options(char **argv, int *i)
{
	int		j;
	char	*opt;

	j = 1;
	while (argv[*i][j])
	{
		opt = argv[*i];
		if (opt[j] == 'h')
			ft_exit_help();
		else if (opt[j] == 'v')
			g_ping.verbose = true;
		else if (opt[j] == 'c' || opt[j] == 'D' || opt[j] == 'i' || \
				opt[j] == 'n' || opt[j] == 'q' || opt[j] == 'V' || \
				opt[j] == 'w' || opt[j] == '4' || opt[j] == '6')
			parse_options_bonus(opt, argv, i, &j);
		else
			ft_exit_parse(opt[j]);
		j++;
	}
}

void	parse(int argc, char **argv)
{
	int		i;

	i = 1;
	while (i < argc)
	{
		if (argv[i][0] == '-' && argv[i][1] == '\0')
			ft_exit_parse(argv[i][0]);
		else if (argv[i][0] == '-')
			parse_options(argv, &i);
		else if (g_ping.destination == NULL)
			g_ping.destination = argv[i];
		else
			ft_exit("ft_ping: usage error: Multiple destination in cmd");
		i++;
	}
	if (g_ping.destination == NULL)
		ft_exit("ft_ping: usage error: Destination address required");
}
