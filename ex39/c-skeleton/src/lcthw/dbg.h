/**
 * @file dbg.h
 * @brief debug marco
 * @author peter liu <peter.liu.work@gmail.com.tw>
 * @version 0.0
 * @date 2016-04-04
 */

#ifndef _dbg_h_
#define _dbg_h_

#include <stdio.h>
#include <errno.h>
#include <string.h>

#ifdef NDEBUG
#define debug(M, ...)
#else
#define debug(M, ...) fprintf(stderr, "[BEBUG] %s:%d:%s " M "\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__)	/*!< output debug message */
#endif /* NDEBUG */

#define clean_errno() (errno == 0 ? "None" : strerror(errno))		/**< get errno string */

#define Log_err(M, ...) fprintf(stderr, "[ERROR] (%s:%d:%s: errno-> %s) " M "\n", __FILE__, __LINE__, __func__, clean_errno(), ##__VA_ARGS__)	/**< log error message */
#define Log_warn(M, ...) fprintf(stderr, "[WARN]  (%s:%d:%s: errno-> %s) " M "\n", __FILE__, __LINE__, __func__, clean_errno(), ##__VA_ARGS__)	/**< log warn message */
#define Log_info(M, ...) fprintf(stderr, "[INFO]  (%s:%d:%s) " M "\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__)	/**< log info message */

#define check(A, M, ...) if (!(A)) { Log_err(M, ##__VA_ARGS__); errno = 0; goto error; }	/**< assert and error handing at error: label */
#define sentinel(M, ...) { Log_err(M, ##__VA_ARGS__); errno = 0; goto error; }				/**< log sentinel */
#define check_mem(A) check(A, "Out of memory")												/**< memory checker */
#define check_debug(A, M, ...) if (!(A)) { debug(M, ##__VA_ARGS__); errno = 0; goto error; }	/**< debug checher */

#endif /* dbg_h_*/
