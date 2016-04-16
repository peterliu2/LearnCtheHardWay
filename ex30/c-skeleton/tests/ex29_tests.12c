#include <stdio.h>
#include "../../../common/include/dbg.h"
#include <dlfcn.h>
#include <string.h>

typedef int (*lib_function) (const char *data);
typedef int (*lib_function_with_len) (const char *data, const int len);

int main(int argc, const char *argv[])
{
	int rc = 0;
	//check(argc == 4, "USAGE: ex29 libex29.so function data");

	const char *lib_file = "build/libex29.so";
	const char *func_to_run[] = {"print_a_message", "uppercase", "lowercase", "print_a_message", "uppercase", "lowercase" /* "fail_on_purpose" */ };
	const char *data = "peter";

	unsigned int i = 0;
	for (i = 0; i< sizeof(func_to_run)/sizeof(func_to_run[0]); i++) {
		void *lib = dlopen(lib_file, RTLD_LAZY);
		check(lib != NULL, "Failed to open the library %s: %s", lib_file, dlerror());

		/* clean any existing error */
		(void)dlerror();
		void *func = dlsym(lib, func_to_run[i]);
		char * errorString = dlerror();
		check(errorString == NULL, "Don't find %s function in the library %s: %s", func_to_run[i], lib_file, errorString);

		if (memcmp(func_to_run[i], "uppercase", strlen("uppercase") + 1) == 0 || memcmp(func_to_run[i], "lowercase", strlen("lowercase") + 1) == 0) 
			rc = ((lib_function_with_len) func)(data, strlen(data));
		else
			rc = ((lib_function)func)(data);

		check(rc == 0, "Function %s return %d for data: %s", func_to_run[i], rc, data);
	
		rc = dlclose(lib);
		check(rc == 0, "Failed to close share library %s: %s", lib_file, dlerror());
	}
	return 0;

error:
	//if (lib) (void)dlclose(lib);
	return 1;
}
