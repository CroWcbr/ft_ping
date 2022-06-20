/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdarrell <cdarrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/18 13:01:21 by cdarrell          #+#    #+#             */
/*   Updated: 2022/06/18 21:02:04 by cdarrell         ###   ########.fr       */
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
}

static void	ft_exit_parse(char char_err)
{
	printf("ping: invalid option -- '%c'\n", char_err);
	print_help();
	exit(1);
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
			ft_exit("ft_ping: usage error: Multiple destination in cmd\n");
		i++;
	}
	if (g_ping.help == true)
	{
		print_help();
		exit (0);
	}
	if (g_ping.destination == NULL)
		ft_exit("ft_ping: usage error: Destination address required\n");
}
