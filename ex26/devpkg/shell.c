/**
 * @file shell.c
 * @brief shell
 * @author peter liu <peter.liu.work@gmail.com.tw>
 * @version 0.0
 * @date 2016-04-10
 */

#include "shell.h"
#include "../../common/include/dbg.h"
#include <stdarg.h>

/**
 * @brief shell execute
 *
 * @param template shell command template
 * @param ... shell command arguments
 *
 * @return 
 *	\b 0: execute finish successfully
 *\n\b -1: error
 */
/* ----------------------------------------------------------------------------*/
int Shell_exec(Shell template, ...)
{
	apr_pool_t *p = NULL;
	int rc = -1;
	apr_status_t rv = APR_SUCCESS;
	va_list argp;
	const char *key = NULL;
	const char *arg = NULL;
	int i = 0;

	rv = apr_pool_create(&p, NULL);
	check(rv == APR_SUCCESS, "Failed to create pool.");

	va_start(argp, template);

	int need_replace_args_count = template.need_replace_args_count;

	for (key = va_arg(argp, const char *); key != NULL; key = va_arg(argp, const char *)) {
		arg = va_arg(argp, const char *);

		for (i = 0; template.args[i] != NULL; i++) {
			if (strcmp(template.args[i], key) == 0) {
				template.args[i] = arg;
				need_replace_args_count--;
				break; /* found it */
			}
		}
	}
	/* check if command required arguments are fill */
	check(need_replace_args_count == 0, "Failed to replace required args: %s.", template.exe);

	rc = Shell_run(p, &template);
	apr_pool_destroy(p);
	va_end(argp);
	return rc;

error:
	if (p) apr_pool_destroy(p);
	va_end(argp);
	return rc;
}

static void print_Shell(Shell *p)
{
	Log_info("dir: %s", p->dir);
	Log_info("exe: %s", p->exe);

	Log_info("exit_code: %d", p->exit_code);

	int i = 0;

	for (i = 0; i < MAX_COMMAND_ARGS; i++) {
		if (p->args[i] == NULL) break;

		Log_info("Arg %d => %s", i, p->args[i]);
	}

	Log_info("need_replace_args_count: %d", p->need_replace_args_count);
}


/**
 * @brief shell run
 *
 * @param p APR pool
 * @param cmd shell command
 *
 * @return
 *	\b 0: run finish successfully
 *\n\b -1: error
 */
/* ----------------------------------------------------------------------------*/
int Shell_run(apr_pool_t *p, Shell *cmd)
{
	apr_procattr_t *attr;
	apr_status_t rv;
	apr_proc_t newproc;
	char errbuf[200];

	//print_Shell(cmd);

	rv = apr_procattr_create(&attr, p);
	check(rv == APR_SUCCESS, "Failed to create proc attr.");

	rv = apr_procattr_io_set(attr, APR_NO_PIPE, APR_NO_PIPE, APR_NO_PIPE);
	check(rv == APR_SUCCESS, "Failed to set IO of command.");

	rv = apr_procattr_dir_set(attr, cmd->dir);
	check(rv == APR_SUCCESS, "Failed to set root to %s.", cmd->dir);

	rv = apr_procattr_cmdtype_set(attr, APR_PROGRAM_PATH);
	check(rv == APR_SUCCESS, "Failed to set cmd type.");

	rv = apr_proc_create(&newproc, cmd->exe, cmd->args, NULL, attr, p);
	check(rv == APR_SUCCESS, "Failed to run command.");

	rv = apr_proc_wait(&newproc, &cmd->exit_code, &cmd->exit_why, APR_WAIT);
	check(rv == APR_CHILD_DONE, "Failed to wait.");

	//check(cmd->exit_code == 0, "%s exit badly.", cmd->exe);
	check(cmd->exit_why == APR_PROC_EXIT, "%s was killed or crashed.", cmd->exe);
	check(cmd->exit_code == 0, "%s exit badly.", cmd->exe);

	return 0;

error:
	apr_strerror(rv, errbuf, sizeof(errbuf));
	Log_err("apr error: %s", errbuf);
	return -1;
}

Shell CLEANUP_SH = {
	.exe = "rm",
	.dir = "/tmp",
	.args = { "rm", "-rf", "/tmp/pkg-build", "/tmp/pkg-src.tar.gz", "/tmp/pkg-src.tar.bz2", "/tmp/DEPENDS", NULL },
	.need_replace_args_count = 0
};

Shell GIT_SH = {
	.exe = "git",
	.dir = "/tmp",
	.args = { "git", "clone", "--progress", "URL", "pkg-build", NULL },
	.need_replace_args_count = 1
};

Shell TAR_BZ2_SH = {
	.exe = "tar",
	.dir = "/tmp/pkg-build",
	.args = { "tar", "-xjf", "FILE", "--strip-components=1", NULL },
	.need_replace_args_count = 1
};

Shell TAR_GZ_SH = {
	.exe = "tar",
	.dir = "/tmp/pkg-build",
	.args = { "tar", "-xzf", "FILE", "--strip-components=1", NULL },
	.need_replace_args_count = 1
};

Shell CURL_SH = {
	.exe = "curl",
	.dir = "/tmp",
	.args = { "curl", "-L", "-o", "TARGET", "URL", NULL },
	.need_replace_args_count = 2
};

Shell CONFIGURE_SH = {
	.exe = "./configure",
	.dir = "/tmp/pkg-build",
	.args = { "configure", "OPTS", NULL},
	.need_replace_args_count = 1
};

Shell MAKE_SH = {
	.exe = "make",
	.dir = "/tmp/pkg-build",
	.args = { "make", "OPTS", NULL },
	.need_replace_args_count  = 1
};

Shell INSTALL_SH = {
	.exe = "sudo",
	.dir = "/tmp/pkg-build",
	.args = { "sudo", "make", "TARGET", NULL },
	.need_replace_args_count = 1
};

