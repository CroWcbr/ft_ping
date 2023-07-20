/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdarrell <cdarrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 01:49:45 by cdarrell          #+#    #+#             */
/*   Updated: 2023/07/20 18:30:16 by cdarrell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PING_H
# define FT_PING_H

# define DEBUG 1

// # include "./libft/include/libft.h"

#include <stdbool.h>	// for bool
#include <stddef.h>		// for size_t
#include <sys/types.h>	// for pid_t
#include <netdb.h>		// for addrinfo
#include <sys/time.h>	// for timeval
typedef struct s_flag
{
	double	interval;
	size_t	count;
	size_t	deadline;
	size_t	ttl;
	bool	verbose;
	bool	timestamps;
	bool	quiet;
	bool	no_dns_name;
}	t_flag;

typedef struct s_ping
{
	char				*destination;
	char				destination_ip[256];
	char				destination_dns[256];
	pid_t				pid;
	int					datalen;
	struct addrinfo		*ai;
	struct sockaddr		*sasend;
	struct sockaddr		*sarecv;
	socklen_t			salen;
	int					sockfd;

	int					count_send;
	int					count_recv;
	struct timeval		time_start;
	struct timeval		time_end;
	double				rtt_min;
	double				rtt_sum;
	double				rtt_max;
	double				rtt_msum;
	int					errors;

	t_flag				flag;
}	t_ping;

extern t_ping		g_ping;

void	parse(int argc, char **argv);
void	create_socket();
void	send_v4(void);
void	readloop(void);
void	proc_v4(char *ptr, ssize_t len, struct timeval *tvrecv);

void	ft_exit(char *str_err);
void	ft_exit_add_info(char *str_info, char *str_err);

#endif
