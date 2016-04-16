/**
 * @file commands.c
 * @brief commands
 * @author peter liu <peter.liu.work@gmail.com.tw>
 * @version 0.0
 * @date 2016-04-13
 */

#include <netdb.h>

#include <apr_uri.h>
#include <apr_fnmatch.h>
#include <unistd.h>

#include "commands.h"
#include "../../common/include/dbg.h"
#include "bstrlib.h"
#include "db.h"
#include "shell.h"

/**
 * @brief install all depends package for path 
 *
 * @param p APR pool
 * @param path depends file path which contains a url per line
 *
 * @return 
 *   \b 0: All dependes are installed successfully
 * \n\b -1: Otherwise
 */ 
/* ----------------------------------------------------------------------------*/
int Command_depends(apr_pool_t *p, const char *path)
{
	FILE *in = NULL;
	bstring line = NULL;

	in = fopen(path, "r");
	check(in != NULL, "Failed to open downloaded depends file: %s.", path);

	int rc = 0;
	int i = 0;
	struct bstrList *args = NULL;
	char *opts[4];

	for (line = bgets((bNgetc)fgetc, in, '\n'); line != NULL; line = bgets((bNgetc)fgetc, in, '\n')) {
		rc = btrimws(line);
		check(rc == BSTR_OK, "Failed to btrimws().");
		
		args = bsplit(line, ' ');
		check(args, "Failed to split command: %s", bdata(line));
		check(args->qty > 0 && args->qty < 5, "command to many arguments: %s", bdata(line));
		
		opts[0] = opts[1] = opts[2] = opts[3] =  NULL;
		for (i = 0; i < args->qty; i++) {
			rc = btrimws(args->entry[i]);
			check(rc == BSTR_OK, "Failed to btrimws(): %s", bdata(args->entry[i]));

			if (strcmp("NULL", bdata(args->entry[i])) != 0)
				opts[i] = bdata(args->entry[i]);
		}
		Log_info("Processing depend: %s c_opts:%s, m_opts:%s, i_opts:%s", opts[0], opts[1], opts[2], opts[3]);
		rc = Command_install(p, opts[0], opts[1], opts[2], opts[3]);
		check(rc == 0, "Failed to install: %s", bdata(line));
	}

	rc = fclose(in);
	check(rc == 0, "Failed to close downloaded depends file: %s.", path);
	//rc = bdestroy(line);
	//check(rc == BSTR_OK, "Failed to bdestory(): %s", bdata(line));
	rc = bstrListDestroy(args);
	check(rc == BSTR_OK, "Failed to bstrListDestroy(): %p", args);
	return 0;

error:
	if (in) fclose(in);
	if (line) bdestroy(line);
	if (args) bstrListDestroy(args);
	return -1;
}

static void print_apr_uri_t(const apr_uri_t const *p)
{
	Log_info("scheme\t\t: %s", p->scheme);
	Log_info("hostinfo\t: %s", p->hostinfo);
	Log_info("user\t\t: %s", p->user);
	Log_info("password\t: %s", p->password);
	Log_info("hostname\t: %s", p->hostname);
	Log_info("port_str\t: %s", p->port_str);
	Log_info("port\t\t: %d", p->port);
	Log_info("path\t\t: %s", p->path);
	Log_info("query\t\t: %s", p->query);
	Log_info("fragment\t: %s", p->fragment);
	//Log_info("hostent.h_name: %s", p->hostent->h_name);
	Log_info("is_initialized\t: %d", p->is_initialized);
	Log_info("dns_looked_up\t: %d", p->dns_looked_up);
	Log_info("dns_resolved\t: %d", p->dns_resolved);
}

/**
 * @brief fetch package from url 
 *
 * @param p APR pool
 * @param url url to fetch
 * @param fetch_only fetch only
 *
 * @return
 *	\b 1: fetch success, need to build & install
 *\n\b 0: fetch success, nothing to do
 *\n\b -1: fetch error
 */
/* ----------------------------------------------------------------------------*/
int Command_fetch(apr_pool_t *p, const char *url, int fetch_only)
{
	apr_uri_t info = { .port = 0 };
	int rc = 0;
	const char *depends_file = NULL;
	char errbuf[100];

	apr_status_t rv = apr_uri_parse(p, url, &info);
	check(rv == APR_SUCCESS, "Failed to pares URL: %s", url);
	//print_apr_uri_t(&info);

	if (apr_fnmatch(GIT_PAT, info.path, 0) == APR_SUCCESS) {
		/* download *.git */
		Log_info("\"%s\" download into /tmp/pkg_build", url);
		rc = Shell_exec(GIT_SH, "URL", url, NULL);
		check(rc == 0, "git failed.");
	} else if (apr_fnmatch(DEPEND_PAT, info.path, 0) == APR_SUCCESS) {
		check(!fetch_only, "No point in fetching a DEPENDS file.");

		if (info.scheme) {
			depends_file = DEPENDS_PATH;
			rc = Shell_exec(CURL_SH, "URL", url, "TARGET", depends_file, NULL);
			check(rc == 0, "Curl failed.");
		} else {
			depends_file = info.path;
		}

		/* recursively process the devpkg list */
		Log_info("Building according to DEPENDS: %s", url);
		rc = Command_depends(p, depends_file);
		check(rc == 0, "Failed to process the DEPENDS: %s.", url);

		/* this indicates that nothing needs to be done */
		return 0;
	} else if (apr_fnmatch(TAR_GZ_PAT, info.path, 0) == APR_SUCCESS) {
		if (info.scheme) {
			/* download *.tar.gz */
			Log_info("Download \"%s\" into \"%s\"", url, TAR_GZ_SRC);
			rc = Shell_exec(CURL_SH, "URL", url, "TARGET", TAR_GZ_SRC, NULL);
			check(rc == 0, "Failed to curl source: %s", url);
		}

		rv = apr_dir_make_recursive(BUILD_DIR, APR_UREAD | APR_UWRITE | APR_UEXECUTE, p);
		check(rv == APR_SUCCESS, "Failed to make directory: %s", BUILD_DIR);

		/* untar to TAR_SH.dir */
		Log_info("untar \"%s\" into \"%s\"", TAR_GZ_SRC, TAR_GZ_SH.dir);
		rc = Shell_exec(TAR_GZ_SH, "FILE", TAR_GZ_SRC, NULL);
		check(rc == 0, "Failed to untar %s", TAR_GZ_SRC);
	} else if (apr_fnmatch(TAR_BZ2_PAT, info.path, 0) == APR_SUCCESS) {
		if (info.scheme) {
			/* download *.tar.bz2 */
			Log_info("\"%s\" download into \"%s\"", url, TAR_BZ2_SRC);
			rc = Shell_exec(CURL_SH, "URL", url, "TARGET", TAR_BZ2_SRC, NULL);
			check(rc == 0, "Failed to curl source: %s", url);
		}

		rv = apr_dir_make_recursive(BUILD_DIR, APR_UREAD | APR_UWRITE | APR_UEXECUTE, p);
		check(rv == APR_SUCCESS, "Fauled to make directory: %s", BUILD_DIR);

		/* untar to TAR_SH.dir */
		Log_info("untar \"%s\" into \"%s\"", TAR_BZ2_SRC, TAR_BZ2_SH.dir);
		rc = Shell_exec(TAR_BZ2_SH, "FILE", TAR_BZ2_SRC, NULL);
		check(rc == 0, "Failed to untar %s", TAR_BZ2_SRC);
	} else {
		sentinel("Don't know how to handle %s", url);
	}

	/* indicates that an build & install needs to actually run */
	return 1;

error:
	apr_strerror(rv, errbuf, sizeof(errbuf));
	Log_err("apr error: %s", errbuf);
	return -1;
}

/**
 * @brief build package from url
 *
 * @param p APR pool
 * @param url url to build
 * @param configure_opts configure options
 * @param make_opts make options
 * @param install_opts install options
 *
 * @return 
 *   \b 0: build, make, install, cleanup, and database update successfully
 * \n\b -1: otherwise
 */
/* ----------------------------------------------------------------------------*/
int Command_build(apr_pool_t *p, const char *url, const char *configure_opts, const char *make_opts, const char *install_opts)
{
	int rc = 0;

	check(url, "URL MUSE be not NULL");
	check(access(BUILD_DIR, X_OK | R_OK | W_OK) == 0, "Build durectory doesn't exit: %s", BUILD_DIR);

	/* actually do install */
	if (access(CONFIG_SCRIPT, X_OK) == 0) {
		Log_info("Has a configure script, running it");
		rc = Shell_exec(CONFIGURE_SH, "OPTS", configure_opts, NULL);
		check(rc == 0, "Failed to configure.");
	}

	rc = Shell_exec(MAKE_SH, "OPTS", make_opts, NULL);
	check(rc == 0, "Failed to build.");

	rc = Shell_exec(INSTALL_SH, "TARGET", install_opts ? install_opts : "install", NULL);
	check(rc == 0, "Failed to install.");

	rc = Shell_exec(CLEANUP_SH, NULL);
	check(rc == 0, "Failed to cleanup after build.");

	rc = DB_update(url);
	check(rc == 0, "Failed to add this packages to the database.");

	return 0;

error:
	return -1;
}

/**
 * @brief Install package command
 *
 * The package come from input parameter : url
 *
 * @param p APR pool
 * @param url package url
 * @param configure_opts configure options
 * @param make_opts make options
 * @param install_opts install options
 *
 * @return 
 *	\b 0: install sucess
 *\n\b -1: install fail
 */
/* ----------------------------------------------------------------------------*/
int Command_install(apr_pool_t *p, const char *url, const char *configure_opts, const char *make_opts, const char *install_opts)
{
	check(p, "input pool is NULL");
	check(url, "input url is NULL");
	
	int rc = 0;

	check(Shell_exec(CLEANUP_SH, NULL) == 0, "Failed to cleanup before install.");

	rc = DB_find(url);
	check(rc != -1, "input URL is NULL");
	check(rc != -2, "bstring allocate failed");
	check(rc != -3, "Error checking the install database.");

	if (rc == 1) {
		Log_info("Package \"%s\" already installed.", url);
		return 0;
	}

	rc = Command_fetch(p, url, 0);

	if (rc == 1) {
		rc = Command_build(p, url, configure_opts, make_opts, install_opts);
		check(rc == 0, "Failed to build: %s", url);
	} else if (rc == 0) {
		/* no install needed */
		Log_info("Depends successfully installed: %s", url);
	} else {
		/* had a error */
		sentinel("Install failed: %s", url);
	}

	Shell_exec(CLEANUP_SH, NULL);
	return 0;

error:
	Shell_exec(CLEANUP_SH, NULL);
	return -1;
}

