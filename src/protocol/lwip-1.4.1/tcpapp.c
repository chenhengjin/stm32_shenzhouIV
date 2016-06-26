/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 */


#include "lwip/opt.h"

#if LWIP_NETCONN

#include "lwip/sys.h"
#include "lwip/api.h"
#include "tcpapp.h"

#define TCPECHO_THREAD_PRIO  ( tskIDLE_PRIORITY + 3 )

struct netconn *tcpAppConn, *newtcpAppConn;

/*-----------------------------------------------------------------------------------*/
static void tcpapp_thread(void *arg)
{
	//struct netconn *conn, *newconn;
	err_t err;

	LWIP_UNUSED_ARG(arg);

	/* Create a new connection identifier. */
	tcpAppConn = netconn_new(NETCONN_TCP);

	if (tcpAppConn!=NULL)
	{  
		/* Bind connection to well known port number 7. */
		err = netconn_bind(tcpAppConn, NULL, 2000);

		if (err == ERR_OK)
		{
			/* Tell connection to go into listening mode. */
			netconn_listen(tcpAppConn);

			while (1) 
			{
				/* Grab new connection. */
				err = netconn_accept(tcpAppConn, &newtcpAppConn);

				/* Process the new connection. */
				if (newtcpAppConn) 
				{
					struct netbuf *buf;
					void *data;
					u16_t len;

					while ((netconn_recv(newtcpAppConn, &buf)) == ERR_OK) 
					{
						do 
						{
							netbuf_data(buf, &data, &len);
							//netconn_write(newtcpAppConn, data, len, NETCONN_COPY);
						} 
						while (netbuf_next(buf) >= 0);

						netbuf_delete(buf);
					}

					/* Close connection and discard connection identifier. */
					netconn_close(newtcpAppConn);
					netconn_delete(newtcpAppConn);
				}
			}
		}
		else
		{
			printf(" can not bind TCP netconn");
		}
	}
	else
	{
		printf("can not create TCP netconn");
	}
}
/*-----------------------------------------------------------------------------------*/

void tcpecho_init(void)
{
  sys_thread_new("tcpapp_thread", tcpapp_thread, NULL, DEFAULT_THREAD_STACKSIZE, TCPECHO_THREAD_PRIO);
}
/*-----------------------------------------------------------------------------------*/

#endif /* LWIP_NETCONN */
