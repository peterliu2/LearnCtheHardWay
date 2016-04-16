/**
 * @file commands.h
 * @brief commands
 * @author peter liu <peter.liu.work@gmail.com.tw>
 * @version 0.0
 * @date 2016-04-13
 */

#ifndef _command_h_
#define _command_h_

#include <apr_pools.h>

#define DEPENDS_PATH	"/tmp/DEPENDS"				/**< depends directory */
#define TAR_GZ_SRC		"/tmp/pkg-src.tar.gz"		/**< *.tar.gz package source directory */
#define TAR_BZ2_SRC		"/tmp/pkg-src.tar.bz2"		/**< *.tar.bz2 package source directory */
#define BUILD_DIR		"/tmp/pkg-build"			/**< package build directory */
#define GIT_PAT			"*.git"						/**< *.git url pattern */
#define DEPEND_PAT		"*DEPENDS"					/**< *.depends url pattern */
#define TAR_GZ_PAT		"*.tar.gz"					/**< *.tar.gz url pattern */
#define TAR_BZ2_PAT		"*.tar.bz2"					/**< *.tar.bz2 url pattern */
#define CONFIG_SCRIPT	"/tmp/pkg-build/configure"	/**< package configure script file location  */

/**
 * @brief commands
 */
/* ----------------------------------------------------------------------------*/
enum CommandType {
	COMMAND_NONE,
	COMMAND_INSTALL,
	COMMAND_LIST,
	COMMAND_FETCH,
	COMMAND_INIT,
	COMMAMD_BUILD,
	COMMAMD_DEPS
};

int Command_fetch(apr_pool_t *p, const char *url, int fetch_only);
int Command_install(apr_pool_t *p, const char *url, const char *configure_opts, const char *make_opts, const char *install_opts);
int Command_depends(apr_pool_t *p, const char *path);
int Command_build(apr_pool_t *p, const char *url, const char *configure_opts, const char *make_opts, const char *install_opts);

#endif /* _command_h_ */
