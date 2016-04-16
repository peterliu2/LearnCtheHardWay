/**
 * @file db.c
 * @brief simple data base
 * @author peter liu <peter.liu.work@gmail.com.tw>
 * @version 0.0
 * @date 2016-04-10
 */

#include <unistd.h>
#include <apr_errno.h>
#include <apr_file_io.h>

#include "db.h"
#include "bstrlib.h"
#include "../../common/include/dbg.h"

static FILE *DB_open(const char *path, const char *mode)
{
	return fopen(path, mode);
}

static int DB_close(FILE *db)
{
	return fclose(db);
}

/**
 * @brief database load
 *
 * Load database contents into bstring
 *
 * @return 
 *		bstring contains database contents, otherwise \b NULL 
 */
/* ----------------------------------------------------------------------------*/
static bstring DB_load(void)
{
	FILE *db = NULL;
	bstring data = NULL;

	db = DB_open(DB_FILE, "r");
	check(db, "Failed to open database: %s", DB_FILE);

	data = bread((bNread)fread, db);
	check(data, "Failed to read from db file: %s", DB_FILE);

	int rc = DB_close(db);
	check(rc == 0, "Failed to close database %s", DB_FILE);
	
	/* if database file close fail, also return NULL*/
	if (rc != 0) data = NULL;

	return data;

error:
	if (db)	DB_close(db);
	if (data) bdestroy(data);
	return NULL;
}

/**
 * @brief database update
 *
 * @param url url to update
 *
 * @return 
 *	 \b 0: database update success
 * \n\b -1: database update fail
 */
/* ----------------------------------------------------------------------------*/
int DB_update(const char *url)
{
	check(url, "input url MUST be not NULL");

	int rc = DB_find(url);

	if (rc == 1)
		Log_info("Already record as installed: %s", url);
	else if (rc == 0) {
		bstring line = bfromcstr(url);
		check_mem(line);

		rc = bconchar(line, '\n');
		check(rc == BSTR_OK, "Failed to bconchar()");

		FILE *db = DB_open(DB_FILE, "a+");
		check(db, "Failed to open DB file: %s", DB_FILE);

		rc = fwrite(line->data, blength(line), 1, db);
		check(rc == 1, "Failed to append to the db.");

		rc = DB_close(db);
		check(rc == 0, "Failed to close: %s", DB_FILE);
		return 0;
error:
		if (db)	(void)DB_close(db);
	}
	return -1;
}

/**
 * @brief find database to input url
 *
 * @param url usl to find
 *
 * @return 
 *   \b 0: url can't find in database
 * \n\b 1: url find in data base
 * \n\b -1: error - url is NULL 
 * \n\b -2: bstring allocate memory failed
 * \n\b -3: error - datebase can't load
 */
/* ----------------------------------------------------------------------------*/
int DB_find(const char *url)
{	
	int res = -1;
	check(url, "input url is NULL");
	
	bstring data = NULL;

	bstring line = bfromcstr(url);	
	res = -2;
	check_mem(line);	
	
	res = -3;
	data = DB_load();	
	check(data, "Failed to load: %s", DB_FILE);

	if (binstr(data, 0, line) == BSTR_ERR) {
		res = 0;
	} else {
		res = 1;
	}

error:	/* fall throught */
	if (data) bdestroy(data);
	if (line) bdestroy(line);

	return res;
}

/**
 * @brief database initialize
 *
 * @return
 *   \b 0: database initialize successfully
 * \n\b -1: Otherwies
 */
/* ----------------------------------------------------------------------------*/
int DB_init(void)
{
	apr_pool_t *p = NULL;
	//apr_pool_initialize();
	apr_pool_create(&p, NULL);
	check(p != NULL, "Failed to create APR pool.");

	if (access(DB_DIR, W_OK | X_OK) == -1) {
		apr_status_t rc = apr_dir_make_recursive(DB_DIR,
						    APR_FPROT_UREAD | APR_FPROT_UWRITE | APR_FPROT_UEXECUTE
						  | APR_FPROT_GREAD | APR_FPROT_GWRITE | APR_FPROT_GEXECUTE, p);
		check(rc == APR_SUCCESS, "Failed to make database directory: %s", DB_DIR);
	}

	if (access(DB_FILE, W_OK) == -1) {
		FILE *db = DB_open(DB_FILE, "w");
		check(db, "Cannot open database: %s", DB_FILE);
		int rc = DB_close(db);
		check(rc == 0, "Failed to colse database: %s", DB_FILE);
	}

	apr_pool_destroy(p);
	return 0;

error:
	apr_pool_destroy(p);
	return -1;
}

/**
 * @brief database list it's contents
 *
 * @return 
 *   \b 0: List success
 * \n\b -1: Otherwise
 */
/* ----------------------------------------------------------------------------*/
int DB_list(void)
{
	bstring data = DB_load();
	check(data, "Failed to load database: %s", DB_FILE);

	printf("%s", bdata(data));
	bdestroy(data);
	return 0;

error:
	return -1;
}
