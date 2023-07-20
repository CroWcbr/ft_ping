/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_socket.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdarrell <cdarrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/18 19:50:16 by cdarrell          #+#    #+#             */
/*   Updated: 2023/06/23 15:23:00 by cdarrell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

static char	*inet_ntop_ipv4_ipv6(struct sockaddr *sa)
{
	char	str[128];

	if (sa->sa_family == AF_INET)
	{
		if (inet_ntop(sa->sa_family, \
				&((struct sockaddr_in *) sa)->sin_addr, str, \
				sizeof(str)) == NULL)
			ft_exit("ERROR! AF_INET inet_ntop in inet_ntop_ipv4_ipv6");
	}
	else if (sa->sa_family == AF_INET6)
	{
		if (inet_ntop(sa->sa_family, \
				&((struct sockaddr_in6 *)sa)->sin6_addr, \
				str, sizeof(str)) == NULL)
			ft_exit("ERROR! AF_INET6 inet_ntop in inet_ntop_ipv4_ipv6");
	}
	else
		ft_exit("ERROR! unknown address family in inet_ntop_ipv4_ipv6");
	return (ft_strdup(str));
}

static void	set_setsockopt(void)
{
	int					on;
	struct icmp6_filter	myfilt;
	int					size;

	if (g_ping.ipv6 && g_ping.verbose == false)
	{
		ICMP6_FILTER_SETBLOCKALL(&myfilt);
		ICMP6_FILTER_SETPASS(ICMP6_ECHO_REPLY, &myfilt);
		if (setsockopt(g_ping.sockfd, IPPROTO_ICMPV6, \
				ICMP6_FILTER, &myfilt, sizeof(myfilt)) < 0)
			ft_exit("ft_ping: setsockopt error: Could not set filter");
	}
	on = 1;
	if (g_ping.ipv6 && setsockopt(g_ping.sockfd, IPPROTO_IPV6, \
				IPV6_RECVHOPLIMIT, &on, sizeof(on)) < 0)
		ft_exit("ft_ping: setsockopt error: Could not set receive hop limit");
	else if (g_ping.ipv6 && setsockopt(g_ping.sockfd, IPPROTO_IPV6, \
				IPV6_UNICAST_HOPS, &on, sizeof(on)) < 0)
		ft_exit("ft_ping: setsockopt error: Could not set hop limit");
	setuid(getuid());
	size = 60 * 1024;
	if (setsockopt(g_ping.sockfd, SOL_SOCKET, \
					SO_RCVBUF, &size, sizeof(size)) < 0)
		ft_exit("ft_ping: setsockopt error: Could not set receive buffer");
}

static void	update_init_v4_v6_func(void)
{
	if (g_ping.ipv4)
	{
		g_ping.fproc = proc_v4;
		g_ping.fsend = send_v4;
		g_ping.icmpproto = IPPROTO_ICMP;
	}
	else
	{
		g_ping.fproc = proc_v6;
		g_ping.fsend = send_v6;
		g_ping.icmpproto = IPPROTO_ICMPV6;
	}
}

static struct addrinfo	*check_destination(void)
{
	struct addrinfo		*ai;
	struct addrinfo		hints;
	int					gai_err;

	ft_bzero(&hints, sizeof(struct addrinfo));
	hints.ai_flags = AI_CANONNAME;
	if (g_ping.ipv4)
		hints.ai_family = AF_INET;
	else if (g_ping.ipv6)
		hints.ai_family = AF_INET6;
	hints.ai_socktype = SOCK_RAW;
	gai_err = getaddrinfo(g_ping.destination, NULL, &hints, &ai);
	if (gai_err)
		ft_exit_add_info(g_ping.destination, (char *)gai_strerror(gai_err));
	if (!g_ping.ipv4 && !g_ping.ipv6)
	{
		if (ai->ai_family == AF_INET)
			g_ping.ipv4 = true;
		else if (ai->ai_family == AF_INET6)
			g_ping.ipv6 = true;
		else
			ft_exit("ft_ping: unknown address family");
	}
	return (ai);
}

void	create_socket(void)
{
	struct addrinfo		*ai;

	ai = check_destination();
	g_ping.ai = ai;
	update_init_v4_v6_func();
	g_ping.destination_ip = inet_ntop_ipv4_ipv6(g_ping.ai->ai_addr);
	if (!g_ping.destination_ip)
		ft_exit("ERROR! ft_strdup in check_destination");
	g_ping.sasend = ai->ai_addr;
	g_ping.sarecv = ft_calloc(1, ai->ai_addrlen);
	if (!g_ping.sarecv)
		ft_exit("ERROR! ft_calloc in check_destination");
	g_ping.salen = ai->ai_addrlen;
	g_ping.sockfd = socket(ai->ai_family, \
							ai->ai_socktype, g_ping.icmpproto);
	if (g_ping.sockfd < 0)
		ft_exit("ft_ping: socket error");
	set_setsockopt();
}
