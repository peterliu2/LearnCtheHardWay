/**
 * @file db.h
 * @brief database
 * @author peter liu <peter.liu.work@gmail.com.tw>
 * @version 0.0
 * @date 2016-04-13
 */


#ifndef _db_h_
#define _db_h_

#define DB_FILE "/usr/local/.devpkg/db"		/**< databae file path */
#define DB_DIR "/usr/local/.devpkg"			/**< database directory path */

int DB_init(void);
int DB_list(void);
int DB_update(const char *url);
int DB_find(const char *url);

#endif /* _db_h_ */
