/**
 * @file shell.h
 * @brief shell header
 * @author peter liu <peter.liu.work@gmail.com.tw>
 * @version 0.0
 * @date 2016-04-10
 */

#ifndef _shell_h_
#define _shell_h_

#define MAX_COMMAND_ARGS 100

#include <apr_thread_proc.h>

typedef struct Shell {
	const char *dir;
	const char *exe;

	apr_procattr_t *attr;
	apr_proc_t proc;
	apr_exit_why_e exit_why;
	int exit_code;

	const char *args[MAX_COMMAND_ARGS];
	int need_replace_args_count;
} Shell;

int Shell_run(apr_pool_t *p, Shell *cmd);
int Shell_exec(Shell cmd, ...);

extern Shell CLEANUP_SH;
extern Shell GIT_SH;
extern Shell TAR_GZ_SH;
extern Shell TAR_BZ2_SH;
extern Shell CURL_SH;
extern Shell CONFIGURE_SH;
extern Shell MAKE_SH;
extern Shell INSTALL_SH;

#endif /* _shell_h_ */
