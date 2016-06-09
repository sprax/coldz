/*****************************************************************************
 MacUnix.h -- for simulating Unix cmdline.    S.R.Lines (spraxlo@ai)    93.6.2
 Usage: #include <MacUnix.h> in sources linked with libMacUnix.a
 *****************************************************************************/

#ifndef _MacUnix_h_
#define _MacUnix_h_

#ifdef  MAC  /* Would prefer #ifndef _unix, if all UNIX cc's defined _unix. */

#error fuck you:
#define MacUnixPROMPT   1
#define MacUnixLOOPMAIN 2

#ifndef MAX_ENVS
#define	MAX_ENVS    64
#endif
#ifndef MAX_ARGS
#define	MAX_ARGS    64
#endif

#define CMD_LEN     512
#ifndef	TRUE
#define	TRUE        1
#define FALSE       0
#endif

        /*___________________GLOBAL_VARIABLES_______________*/
extern  int   MacUnixOptions;    /* prmpMain.c */
extern  char *ProgName;

extern void	MacArg (int *argcp, char ***argvp, char ***envpp);
extern int	Main (         int argc, char *argv[], char *envp[]);
extern int	mainArgsPrompt ( int argc, char *argv[], char *envp[]);
extern int	mainLinePrompt ( int argc, char *argv[], char *envp[], char *command);
extern void	argsPrompt ( int *argcp, char ***argvp, char ***envpp);
extern void	linePrompt (int *argcp, char ***argvp, char ***envpp, char command[CMD_LEN]);
extern int	lineParse (char **argV, char **envP, char *cmdLine);

#endif  /* MAC */

#endif  /* _MacUnix_h_ */

