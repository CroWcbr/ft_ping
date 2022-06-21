/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readloop.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdarrell <cdarrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/18 13:31:10 by cdarrell          #+#    #+#             */
/*   Updated: 2022/06/21 19:02:20 by cdarrell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

static void	readloop_while(struct msghdr *msg, \
						char *recvbuf, char *controlbuf)
{
	ssize_t			n;
	struct timeval	tval;

	while (1)
	{
		msg->msg_namelen = g_ping.salen;
		msg->msg_controllen = sizeof(controlbuf);
		n = recvmsg(g_ping.sockfd, msg, 0);
		if (n < 0)
		{
			if (errno == EINTR)
				continue ;
			else
				printf("recvmsg error");
		}
		gettimeofday(&tval, NULL);
		proc_v4(recvbuf, n, &tval);
	}
}

void	readloop(void)
{
	char			recvbuf[BUFSIZE];
	char			controlbuf[BUFSIZE];
	struct msghdr	msg;
	struct iovec	iov;

	iov.iov_base = recvbuf;
	iov.iov_len = sizeof(recvbuf);
	msg.msg_name = g_ping.sarecv;
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;
	msg.msg_control = controlbuf;
	readloop_while(&msg, recvbuf, controlbuf);
}
