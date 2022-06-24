/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdarrell <cdarrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/18 13:09:58 by cdarrell          #+#    #+#             */
/*   Updated: 2022/06/24 20:29:18 by cdarrell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PING_H
# define FT_PING_H

# include "../libft/include/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>

# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include <arpa/inet.h>

# include <netinet/ip6.h>
# include <netinet/icmp6.h>

typedef struct s_ping
{
	char				*destination;
	char				*destination_ip;
	bool				help;
	bool				verbose;
	bool				ipv4;
	bool				ipv6;
	pid_t				pid;
	int					datalen;
	struct addrinfo		*ai;	
	struct sockaddr		*sasend;
	struct sockaddr		*sarecv;
	socklen_t			salen;
	int					icmpproto;
	int					sockfd;
	void				(*fproc)(char*, ssize_t, struct timeval*);
	void				(*fsend)(void);
	int					count_send;
	int					count_recv;
	struct timeval		time_start;
	struct timeval		time_end;
	double				rtt_min;
	double				rtt_sum;
	double				rtt_max;
	double				rtt_msum;
}t_ping;

t_ping			g_ping;

void			ft_exit(char *str_err);
void			ft_exit_add_info(char *str_info, \
									char *str_err);
void			init_ping(void);
void			parse(int argc, \
						char **argv);
void			create_socket(void);

void			send_v4(void);
void			proc_v4(char *ptr, ssize_t len, struct timeval *tvrecv);
void			send_v6(void);
void			proc_v6(char *ptr, ssize_t len, struct timeval *tvrecv);

#endif
