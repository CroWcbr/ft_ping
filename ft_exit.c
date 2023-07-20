/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdarrell <cdarrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/18 16:04:04 by cdarrell          #+#    #+#             */
/*   Updated: 2022/06/24 20:28:30 by cdarrell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"
#include <stdio.h>
#include <stdlib.h>

void	ft_exit(char *str_err)
{
	printf("%s\n", str_err);
	exit(1);
}

void	ft_exit_add_info(char *str_info, char *str_err)
{
	printf("ft_ping: %s: %s\n", str_info, str_err);
	exit(1);
}
