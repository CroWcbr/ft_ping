/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdarrell <cdarrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/18 13:09:58 by cdarrell          #+#    #+#             */
/*   Updated: 2022/08/19 23:32:54 by cdarrell         ###   ########.fr       */
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

# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include <arpa/inet.h>
# include <time.h>
# include <sys/time.h>

# include <netdb.h>
# include <netinet/in_systm.h>
# include <netinet/ip.h>
# include <netinet/ip_icmp.h>
# include <netinet/ip6.h>
# include <netinet/icmp6.h>

# include <error.h>
# include <errno.h>

# define BUFSIZE 1500

typedef struct s_ping
{
	char				*destination;
	char				*destination_ip;
	bool				answer;
	size_t				count;
	bool				timestamps;
	size_t				interval;
	bool				dns_name;
	bool				quiet;
	bool				verbose;
	size_t				deadline;
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
	void				(*fproc)(char*, \
								ssize_t, \
								struct msghdr *, \
								struct timeval*);
	void				(*fsend)(void);
	int					count_send;
	int					count_recv;
	int					errors;
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
void			parse_options_bonus(char *opt, char **argv, int *i, int *j);
size_t			find_int(char *opt, char **argv, int *i, int *j);
double			find_double(char *opt, char **argv, int *i, int *j);

void			create_socket(void);
void			tv_sub(struct timeval *out, \
						struct timeval *in);
void			sig_alrm(int signo);
void			readloop(ssize_t n);

void			send_v4(void);
void			proc_v4(char *ptr, \
						ssize_t len, \
						struct msghdr *msg, \
						struct timeval *tvrecv);
void			send_v6(void);
void			proc_v6(char *ptr, \
						ssize_t len, \
						struct msghdr *msg, \
						struct timeval *tvrecv);

#endif
