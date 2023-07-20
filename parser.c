/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdarrell <cdarrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 01:52:16 by cdarrell          #+#    #+#             */
/*   Updated: 2023/07/20 18:56:00 by cdarrell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./libft/include/libft.h"
#include "ft_ping.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

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
	printf("  -t <ttl>           define time to live\n");
	printf("  -v                 verbose output\n");
	printf("  -V                 print version and exit\n");
	printf("  -w <deadline>      reply wait <deadline> in seconds\n");
	printf("\n");
}

static void	ft_exit_parser(char char_err)
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

static void	ft_exit_version(void)
{
	printf("Cdarrell best version!\n");
	exit (0);
}

static void	ft_exit_error(char *first_arg, ...)
{
	va_list	args;
	char	*arg;

	va_start(args, first_arg);
	arg = first_arg;
	while (*arg)
	{
		write(1, arg, ft_strlen(arg));
		arg = va_arg(args, char *);
	}
	va_end(args);
	ft_putstr("\n");
	exit(1);
}

static size_t	find_int(char *opt, char flag)
{
	for (int i = 0; i < ft_strlen(opt); ++i)
	{
		if (!ft_isdigit(opt[i]))
		{
			ft_exit_error("ft_ping: invalid argument: \'", opt, "\'", "\0");
		}
	}

	char*	max_numeric = "9223372036854775807";
	if (ft_strlen(opt) >= ft_strlen(max_numeric))
	{
		for (int i = 0; i < ft_strlen(opt); ++i)
		{
			if (opt[i] > max_numeric[i])
			{
				ft_exit_error("ft_ping: invalid argument: \'", opt, "\': Numerical result out of range", "\0");
			}
		}
	}

	size_t	tmp = 0;
	for (int i = 0; i < ft_strlen(opt); ++i)
	{
		tmp = tmp * 10 + opt[i] - '0';
	}

	if (flag == 'c' && tmp == 0)
	{
			ft_exit_error("ft_ping: invalid argument: \'", opt, "\': out of range: 1 <= value <= 9223372036854775807", "\0");
	}
	else if (flag == 't' && tmp > 255)
	{
		ft_exit_error("ft_ping: invalid argument: \'", opt, "\': out of range: 0 <= value <= 255", "\0");
	}
	else if (flag == 'w' && tmp > 2147483647)
	{
		ft_exit_error("ft_ping: invalid argument: \'", opt, "\': out of range: 0 <= value <= 2147483647", "\0");
	}

	return (tmp);
}

static double	find_double(char *opt, char flag)
{
	for (int i = 0, point = 0; i < ft_strlen(opt); ++i)
	{
		if (opt[i] == '.')
		{
			point++;
		}
		else if (!ft_isdigit(opt[i]) && point < 2)
		{
			ft_exit_error("ft_ping: invalid argument: \'", opt, "\'", "\0");
		}
	}

	double	tmp = 0;
	double	decimal = 0.1;
	for (int i = 0, point = 0; i < ft_strlen(opt); ++i)
	{
		if(opt[i] == '.')
			point++;
		else if (point == 0)
		{
			tmp = tmp * 10 + opt[i] - '0';
		}
		else
		{
			tmp = tmp + (opt[i] - '0') * decimal;
			decimal *= 0.1;
		}
	}

	if (flag == 'f' && tmp < 0.002)
	{
			ft_exit_error("ft_ping: invalid argument: \'", opt, "\': minimal interval allowed for user is 2ms", "\0");
	}
	else if (flag == 'f' && tmp > 100)
	{
			ft_exit_error("ft_ping: invalid argument: \'", opt, "\': maximum interval allowed for user is 100s", "\0");
	}

	return (tmp);
}

static void	print_parser()
{
	printf("---------------PARSRER-------------------\n");
	printf("interval    = \t%s\n", g_ping.destination);
	printf("count       = \t%ld\n", g_ping.flag.count);
	printf("deadline    = \t%ld\n", g_ping.flag.deadline);
	printf("ttl         = \t%ld\n", g_ping.flag.ttl);
	printf("interval    = \t%.3f\n", g_ping.flag.interval);
	printf("verbose     = \t%d\n", g_ping.flag.verbose);
	printf("timestamps  = \t%d\n", g_ping.flag.timestamps);
	printf("quiet       = \t%d\n", g_ping.flag.quiet);
	printf("no_dns_name = \t%d\n", g_ping.flag.no_dns_name);
	printf("-----------------------------------------\n");
}

void	parse(int argc, char **argv)
{
	int	i = 1;

	while (i < argc)
	{
		if (argv[i][0] == '-' && argv[i][1] != '\0')
		{
			int j = 1;

			while (j < ft_strlen(argv[i]))
			{
				char	flag = argv[i][j];
				if (flag == 'h')
					ft_exit_help();
				else if (flag == 'V')
					ft_exit_version();
				else if (flag == 'v' && ++j)
				{
					g_ping.flag.verbose = true;
					continue;
				}
				else if (flag == 'D' && ++j)
				{
					g_ping.flag.timestamps = true;
					continue;
				}
				else if (flag == 'q' && ++j)
				{
					g_ping.flag.quiet = true;
					continue;
				}
				else if (flag == 'n' && ++j)
				{
					g_ping.flag.no_dns_name = true;
					continue;
				}
				else if (flag == 'c' && (argv[i][2] || argv[i + 1]))
					g_ping.flag.count = find_int(argv[i][j + 1] ? argv[i] + j + 1 : argv[++i], flag);
				else if (flag == 'i' && (argv[i][2] || argv[i + 1]))
					g_ping.flag.interval = find_double(argv[i][j + 1] ? argv[i] + j + 1 : argv[++i], flag);
				else if (flag == 't' && (argv[i][2] || argv[i + 1]))
					g_ping.flag.ttl = find_int(argv[i][j + 1] ? argv[i] + j + 1 : argv[++i], flag);
				else if (flag == 'w' && (argv[i][2] || argv[i + 1]))
					g_ping.flag.deadline = find_int(argv[i][j + 1] ? argv[i] + j + 1 : argv[++i], flag);
				else
					ft_exit_parser(flag);
				break;
			}
		}
		else
			g_ping.destination = argv[i];
		i++;
	}
	if (DEBUG)
		print_parser();
}
