/*******************************************************************************
 *  The BYTE UNIX Benchmarks - Release 3
 *          Module: spawn.c   SID: 3.3 5/15/91 19:30:20
 *
 *******************************************************************************
 * Bug reports, patches, comments, suggestions should be sent to:
 *
 *	Ben Smith, Rick Grehan or Tom Yagerat BYTE Magazine
 *	ben@bytepb.byte.com   rick_g@bytepb.byte.com   tyager@bytepb.byte.com
 *
 *******************************************************************************
 *  Modification Log:
 *  $Header: spawn.c,v 3.4 87/06/22 14:32:48 kjmcdonell Beta $
 *  August 29, 1990 - Modified timing routines (ty)
 *  October 22, 1997 - code cleanup to remove ANSI C compiler warnings
 *                     Andy Kahn <kahn@zk3.dec.com>
 *
 ******************************************************************************/
char SCCSid[] = "@(#) @(#)spawn.c:3.3 -- 5/15/91 19:30:20";
/*
 *  Process creation
 *
 */

//#include <stdio.h>
//#include <stdlib.h>
//#include <unistd.h>
//#include <sys/wait.h>
#include "types.h"
#include "user.h"

unsigned long iter;

void report()
{
	printf(2,"COUNT|%lu|1|lps\n", iter);
	exit();
}

int main(argc, argv)
int	argc;
char	*argv[];
{
	int	slave;
	int	status;

	iter = 0;

	while (iter <= 10l) {
		if ((slave = fork()) == 0) {
			/* slave .. boring */
#if debug
			printf(1,"fork OK\n");
#endif
			/* kill it right away */
			exit();
		} else if (slave < 0) {
			/* woops ... */
			printf(2,"Fork failed at iteration %lu\n", iter);
			//perror("Reason");
			exit();
		} else {
			/* master */
			wait();
			status = 0;
		}
		if (status != 0) {
			printf(2,"Bad wait status: 0x%x\n", status);
			exit();
		}
		iter++;
#if debug
		printf(1,"Child %d done.\n", slave);
#endif
	}
	exit();
}
