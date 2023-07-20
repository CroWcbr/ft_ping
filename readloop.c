/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readloop.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdarrell <cdarrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 17:35:07 by cdarrell          #+#    #+#             */
/*   Updated: 2023/07/20 17:55:07 by cdarrell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"
#include <stdio.h>
#include <errno.h>

void	readloop(void)
{
	char			recvbuf[1500];
	char			controlbuf[1500];
	struct msghdr	msg;
	struct iovec	iov;
	size_t			n;
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
