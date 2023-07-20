/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readloop.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdarrell <cdarrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/18 13:31:10 by cdarrell          #+#    #+#             */
/*   Updated: 2022/08/19 23:32:07 by cdarrell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

void	readloop(ssize_t n)
{
	char			recvbuf[BUFSIZE];
	char			controlbuf[BUFSIZE];
	struct msghdr	msg;
	struct iovec	iov;
	struct timeval	tval;

	iov.iov_base = recvbuf;
	iov.iov_len = sizeof(recvbuf);
	msg.msg_name = g_ping.sarecv;
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;
	msg.msg_control = controlbuf;
	while (1)
	{
		msg.msg_namelen = g_ping.salen;
		msg.msg_controllen = sizeof(controlbuf);
		n = recvmsg(g_ping.sockfd, &msg, 0);
		if (n < 0 && errno == EINTR)
			continue ;
		if (n < 0)
			printf("recvmsg error");
		gettimeofday(&tval, NULL);
		g_ping.fproc(recvbuf, n, &msg, &tval);
	}
}
