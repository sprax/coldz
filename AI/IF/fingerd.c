/*
 * Copyright (c) 1983 The Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that the above copyright notice and this paragraph are
 * duplicated in all such forms and that any documentation,
 * advertising materials, and other materials related to such
 * distribution and use acknowledge that the software was developed
 * by the University of California, Berkeley.  The name of the
 * University may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#ifndef lint
char copyright[] =
"@(#) Copyright (c) 1983 The Regents of the University of California.\n\
 All rights reserved.\n";
#endif /* not lint */

#ifndef lint
static char sccsid[] = "@(#)fingerd.c	5.4 (Berkeley) 11/23/88";
#endif /* not lint */

#include <stdio.h>

/* #define sleep(tt) ; */

main(int argc, char **argv)
{
	register FILE *fp;
	register int ch;
	register char *lp;
	int p[2], wt;
#define	ENTRIES	50
	char **ap, *av[ENTRIES + 1], line[1024], *strtok();
static char *fool = "\
Stephen R Lines (spraxlo)\n\
Home: /home/vu/spraxlo\n\
Shell: /usr/local/bin/tcsh\n\
Timeout -- while examining user mail forward file.\n";
static char *gool = "\
Timeout -- while examining user mail forward file.\n";

#define LOGGING 1
#ifdef	LOGGING					/* unused for now */
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/nameser.h>
#include <resolv.h>
int res_query (char *dname, int class, int type, u_char *answer, int anslen);
        FILE *logfile = NULL;
	unsigned long sad = 0;
#if 1
        time_t	tim;
	char 	*iname, *inet_ntoa(struct in_addr in);
        struct	sockaddr_in sin;
        int  	sval = sizeof(sin), qtype, stat;
        if (getpeername(0, &sin, &sval) < 0)
                fatal("getpeername");
        if (! ( logfile = fopen("/usr/etc/fingerd.log","a")))
                fatal(   "fopen( /usr/etc/fingerd.log)");
        sad = sin.sin_addr.s_addr;
        /** fprintf(logfile,"f.p.a: %d.%u.%u\n"
            ,sin.sin_family,sin.sin_port,sad); **/
        tim = time(&tim);
        srand(tim);
        lp = ctime(&tim);
        iname = inet_ntoa(sin.sin_addr);
        fprintf(logfile,"\
--------------------------------------------------------\n%s  (%u)  \t%s"
	,iname,sad,lp);
/******************************************************
        {
#define ANSLEN	1024
        u_char	answ[ANSLEN];
	struct	hostent *he, *gethostbyname(const char *name);
	he = gethostbyname(iname);
        fprintf(logfile,"\
	struct  hostent {\n\
        char    *h_name;        %s\n\
        char    **h_aliases;    %s    %s\n\
        int     h_addrtype;     %d\n\
        int     h_length;       %d\n\
        char    **h_addr_list;  %u    %u\n"
	,he->h_name,he->h_aliases[0],(he->h_aliases[1]?he->h_aliases[1]:"")
	,he->h_addrtype,he->h_length,he->h_addr_list[0]
	,(he->h_addr_list[1]?he->h_addr_list[1]:0));
	stat = res_init();
	ch = res_search(iname,C_IN,qtype=T_A,answ,ANSLEN);
        fprintf(logfile,"stat:%d, %d = res_search(%d):<%.*s>\n"
	,stat,ch,qtype,ANSLEN,answ);
        fprintf(logfile,"------------------------------------------\n");
        }
*******************************************************/
#else
        char ss[128];
        struct sockaddr name;
        int namelen;
        namelen = 8;
        if ((ch=getpeername(0, &name, &namelen)) < 0)
                fatal("getpeername");
        if (! ( logfile = fopen("/usr/etc/fingerd.log","a")))
                fatal(   "fopen( /usr/etc/fingerd.log)");
        fprintf(logfile,"gpn:%d, namelen:%d, sa_family:%u,  sa_data:<%.8d>\n"
               ,ch,namelen,name.sa_family,name.sa_data);
        /** u_short sa_family; char    sa_data[14]; **/
#endif
#endif

	if (!fgets(line, sizeof(line), stdin))
		exit(1);
        /** fprintf(logfile,"line:<%s>\n",line); **/

         
	av[0] = "finger";
	for (lp = line, ap = &av[1];;) {
		*ap = strtok(lp, " \t\r\n");
		if (!*ap)
			break;
		/* RFC742: "/[Ww]" == "-l" */
		if ((*ap)[0] == '/' && ((*ap)[1] == 'W' || (*ap)[1] == 'w'))
			*ap = "-l";
		if (++ap == av + ENTRIES)
			break;
		lp = NULL;
                /** fprintf(logfile,"av++:<%s>\n",*ap); **/
	}

	if (pipe(p) < 0)
		fatal("pipe");

	switch(fork()) {
	case 0:
		(void)close(p[0]);
		if (p[1] != 1) {
			(void)dup2(p[1], 1);
			(void)close(p[1]);
		}
		execv("/usr/ucb/finger", av);
		_exit(1);
	case -1:
		fatal("fork");
	}
	(void)close(p[1]);
	if (!(fp = fdopen(p[0], "r")))
		fatal("fdopen");
        if (!strncmp(iname,"18.85",5)) {
          sleep(9);
          wt = rand() % 3;
          fprintf(logfile,"wt:%d\n",wt);
          if (wt) {
            sleep(wt + 7);
            if (!strcmp(line,"spraxlo") || !strcmp(line,"lines"))
              lp = fool;
            else {
              sleep(wt + 8);
              fprintf(logfile,"line:%s\n",line);
              lp = gool;
            }
            while ((ch = *lp++) != NULL) {
              if (ch == '\n') putchar('\r');
              putchar(ch);
              fputc(ch,logfile);
            }
            exit(1);
          }
        }
	while ((ch = getc(fp)) != EOF) {
		if (ch == '\n') putchar('\r');
		putchar(ch);
		fputc(ch,logfile);
	}
	exit(0);
}

fatal(msg)
	char *msg;
{
	extern int errno;
	char *strerror();

	fprintf(stderr, "fingerd: %s: %s\r\n", msg, strerror(errno));
	exit(1);
}


