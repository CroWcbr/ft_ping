/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_int_param.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdarrell <cdarrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/12 22:14:48 by cdarrell          #+#    #+#             */
/*   Updated: 2023/01/07 02:22:05 by cdarrell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

static void	ft_exit_parse_add_param(char flag, char *str_err)
{
	(void)flag;
	printf("ping: invalid argument: '%s'\n", str_err);
	exit(1);
}

static size_t	find_digit(char *dig, int *j, int jj, char flag)
{
	size_t	tmp;

	tmp = 0;
	while (dig[*j])
	{
		if (!ft_isdigit(dig[*j]))
		{
			ft_exit_parse_add_param(flag, dig + jj + 1);
		}
		tmp = tmp * 10 + dig[*j] - '0';
		(*j)++;
	}
	return (tmp);
}

size_t	find_int(char *opt, char **argv, int *i, int *j)
{
	int		jj;
	size_t	tmp;

	tmp = 0;
	jj = (*j)++;
	if (opt[*j])
		tmp = find_digit(opt, j, jj, opt[jj]);
	else
	{
		(*i)++;
		*j = 0;
		tmp = find_digit(argv[*i], j, -1, opt[jj]);
	}
	(*j)--;
	return (tmp);
}
