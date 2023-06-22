/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdarrell <cdarrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/18 13:09:58 by cdarrell          #+#    #+#             */
/*   Updated: 2023/06/23 00:11:06 by cdarrell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PING_H
# define FT_PING_H

# include "../libft/include/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdbool.h>
# include <signal.h>
# include <netdb.h>
# include <netinet/in_systm.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <netinet/ip.h>
# include <netinet/ip_icmp.h>
# include <time.h>
# include <sys/time.h>
# include <error.h>
# include <errno.h>

# define BUFSIZE 1500

typedef struct s_ping
{
	char				*destination;
	char				*destination_ip;
	bool				help;
	bool				verbose;
	pid_t				pid;
	int					datalen;
	struct sockaddr_in	sa;
	struct sockaddr		*sasend;
	struct sockaddr_storage		addr;
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


	struct sockaddr_in	ssarecv;
}t_ping;

extern t_ping	g_ping;

void			ft_exit(char *str_err);
void			ft_exit_add_info(char *str_info, \
									char *str_err);
void			init_ping(void);
void			parse(int argc, \
						char **argv);
void			create_socket(void);
void			tv_sub(struct timeval *out, \
						struct timeval *in);
void			sig_alrm(int signo);
void			readloop(void);
void			proc_v4(char *ptr, \
						ssize_t len, \
						struct timeval *tvrecv);
void			send_v4(void);

#endif
