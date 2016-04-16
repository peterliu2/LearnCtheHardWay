/**
 * @file devpkg.c
 * @brief Packages manage main function
 * @author peter liu <peter.liu.work@gmail.com.tw>
 * @version 0.0
 * @date 2016-04-13
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <apr_general.h>
#include <apr_getopt.h>
#include <apr_strings.h>
#include <apr_lib.h>

#include "../../common/include/dbg.h"
#include "db.h"
#include "commands.h"

#define CLI_CMD_ERROR_USAGE_STRING "Invail command given.\n\tUsage: %s -I url | -L | -c url | -m url | -i url | -S | - F url | -B url", argv[0] /**< command line error message: devpkg usage  */

/**
 * @brief command name string
 * for debug only
 */
/* ----------------------------------------------------------------------------*/
const char const *Command_type_names[] = {
	"COMMAND_NONE",
	"COMMAND_INSTALL",
	"COMMAND_LIST",
	"COMMAND_FETCH",
	"COMMAND_INIT",
	"COMMAMD_BUILD",
	"COMMAMD_DEPS"
};

/**
 * @brief print man arguments
 *
 * @param argc argument counts to print
 * @param argv[] list of argument strings
 */
/* ----------------------------------------------------------------------------*/
static void print_man_args(const int argc, const char const *argv[])
{
	int i = 0;

	for (i = 0; i < argc; i++)
		Log_info("man arg %d: %s", i, argv[i]);
}

/**
 * @brief main function
 *
 * @param argc
 * @param argv[]
 *
 * @return 
 */
/* ----------------------------------------------------------------------------*/
int main(int argc, const char const *argv[])
{
	/* check input arguemnt */
	check(argc > 1, CLI_CMD_ERROR_USAGE_STRING);
	
	apr_pool_t *p = NULL;
	apr_getopt_t *opt;
	apr_status_t rv;
	int rc;

	//print_man_args(argc, argv);

	/* initialize APR library */
	rv = apr_initialize();
	check(rv == APR_SUCCESS, "Failed to initialize APR library");

	/* register exit callback function to terminate APR library */
	rc = atexit(apr_terminate);
	check(rc == 0, "Failed to register apt_terminate at program exit");

	/* create pool as root pool */
	rv = apr_pool_create(&p, NULL);
	check(rv == APR_SUCCESS, "Failed to create root pool");

	char ch = '\0';
	const char *optarg = NULL;
	const char *config_opts = NULL;
	const char *install_opts = NULL;
	const char *make_opts = NULL;
	const char *url = NULL;
	enum CommandType request = COMMAND_NONE;

	/* initialize argument parse by apr_getopt() */
	rv = apr_getopt_init(&opt, p, argc, argv);
	check(rv == APR_SUCCESS, "Failed to initialize the APR get opt.");

	/* parse the options */
	int opts_count = 0;		/* record has parsed main argument counts */

	while (apr_getopt(opt, "I:Lc:m:i:d:SF:B:", &ch, &optarg) == APR_SUCCESS) {
		opts_count++;
		switch (ch) {
			case 'I':
				request = COMMAND_INSTALL;
				url = optarg;
				opts_count++;
				break;

			case 'L':
				request = COMMAND_LIST;
				break;

			case 'c':
				config_opts = optarg;
				opts_count++;
				break;

			case 'm':
				make_opts = optarg;
				opts_count++;
				break;

			case 'i':
				install_opts = optarg;
				opts_count++;
				break;

			case 'S':
				request = COMMAND_INIT;
				break;

			case 'F':
				request = COMMAND_FETCH;
				url = optarg;
				opts_count++;
				break;

			case 'B':
				request = COMMAMD_BUILD;
				url = optarg;
				opts_count++;
				break;

			case 'd':
				request = COMMAMD_DEPS;
				url = optarg;
				opts_count++;
				break;

			default:
				sentinel("I can't run to here");
		}
	}

	check((opts_count + 1) == argc, CLI_CMD_ERROR_USAGE_STRING);
	//Log_info("request command: %s, argument counts: %d, opts_count: %d", Command_type_names[request], argc, opts_count);

	switch (request) {
		case COMMAND_INSTALL:
			check(url, "You must at lease give a URL.");
			rc = Command_install(p, url, config_opts, make_opts, install_opts);
			check(rc == 0, "Failed to install: %s --config_opts=%s --make-opts=%s --install-opts=%s", url, config_opts, make_opts, install_opts);
			break;

		case COMMAND_LIST:
			rc = DB_list();
			check(rc == 0, "Failed to list database.");
			break;

		case COMMAND_FETCH:
			check(url != NULL, "You must give a URL.");
			rc = Command_fetch(p, url, 1);
			check(rc != -1, "Failed to fetch only url: %s", url);
			Log_info("Downloaded to %s and in /tmp/", BUILD_DIR);
			break;

		case COMMAMD_BUILD:
			check(url, "You must at least give a URL.");
			rc = Command_build(p, url, config_opts, make_opts, install_opts);
			check(rc == 0, "Failed to build: %s --config_opts=%s --make-opts=%s --install-opts=%s", url, config_opts, make_opts, install_opts);
			break;

		case COMMAND_INIT:
			rc = DB_init();
			check(rc == 0, "Failed to make the database.");
			break;

		case COMMAMD_DEPS:
			check(url, "You must give a path for *DEPENDS file");
			rc = Command_depends(p, url);
			check(rc == 0, "Failed to install all depends, url: %s", url);
			break;

		default:
			sentinel(CLI_CMD_ERROR_USAGE_STRING);
	}

	apr_pool_destroy(p);
	return 0;

error:
	apr_pool_destroy(p);
	return 1;
}
