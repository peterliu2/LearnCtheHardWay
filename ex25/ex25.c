#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "../common/include/dbg.h"

#define MAX_DATA 100

int read_string(char **out_string, int max_buffer)
{
	*out_string = calloc(1, max_buffer + 1);
	check_mem(*out_string);

	char *result = fgets(*out_string, max_buffer, stdin);
	check(result != NULL, "Input error.");

	(*out_string)[strlen(*out_string) - 1] = '\0';

	return 0;

error:
	free(*out_string);
	*out_string = NULL;
	return -1;
}

int read_int(int *out_int)
{
	char *input = NULL;

	int rc = read_string(&input, MAX_DATA);
	check(rc == 0, "Fauled to read number.");

	*out_int = atoi(input);

	free(input);
	return 0;

error:
	free(input);
	return -1;
}

int read_scan(const char *fmt, ...)
{
	int i = 0;
	int rc = 0;
	int *out_int = NULL;
	char *out_char = NULL;
	char **out_string = NULL;
	int max_buffer = 0;

	va_list argp;
	va_start(argp, fmt);

	for (i = 0; fmt[i] != '\0'; i++) {
		if (fmt[i] == '%') {
			i++;
			switch (fmt[i]) {
			case '\0':
				sentinel("Invauld format, you end with \\0.");
				break;

			case 'd':
				out_int = va_arg(argp, int *);
				rc = read_int(out_int);
				check(rc == 0, "Failed to read int.");
				break;

			case 'c':
				out_char = va_arg(argp, char *);
				*out_char = fgetc(stdin);
				while (fgetc(stdin) != '\n') ;	//**< eat ENTER */
				break;

			case 's':
				max_buffer = va_arg(argp, int);
				out_string = va_arg(argp, char **);
				rc = read_string(out_string, max_buffer);
				check(rc == 0, "Failed to read string");
				break;

			default:
				sentinel("Invalid format");
			}
		} else {
			fgetc(stdin);
		}

		check(!feof(stdin) && !ferror(stdin), "Input error.");
	}

	va_end(argp);
	return 0;

error:
	va_end(argp);
	return -1;
}

int write_printf(const char *fmt, ...)
{
	int i = 0;
	const char *write_string;
	int write_int;
	char *s = NULL;
	int rc = 0;

	va_list argp;
	va_start(argp, fmt);

	for (i = 0; fmt[i] != '\0'; i++) {
		if (fmt[i] == '%') {
			i++;
			switch (fmt[i]) {
			case 's':
				write_string = va_arg(argp, const char *);
				fputs(write_string, stdout);
				rc += strlen(write_string);
				break;

			case 'd':
				write_int = va_arg(argp, int);
				s = calloc(1, MAX_DATA + 1);
				check_mem(s);
				rc +=
				    snprintf(s, MAX_DATA + 1, "%d", write_int);
				fputs(s, stdout);
				free(s);
				break;

			case 'c':
				write_int = va_arg(argp, int);
				fputc(write_int, stdout);
				rc++;
				break;

			default:
				sentinel("Invalid format.");

			}
		} else {
			fputc(fmt[i], stdout);
			rc++;
		}
	}

	va_end(argp);
	return rc;

error:
	free(s);
	va_end(argp);
	return -1;
}

int main(int argc, char *argv[])
{
	char *first_name = NULL;
	char initial = ' ';
	char *last_name = NULL;
	int age = 0;

	write_printf("What is your first name? ");
	int rc = read_scan("%s", MAX_DATA, &first_name);
	check(rc == 0, "Failed to read first name.");

	write_printf("What is your initial? ");
	rc = read_scan("%c", &initial);
	check(rc == 0, "Fauled initial.");

	write_printf("What is your last name? ");
	rc = read_scan("%s", MAX_DATA, &last_name);
	check(rc == 0, "Failed to read last name.");

	write_printf("How old are you? ");
	rc = read_scan("%d", &age);
	check(rc == 0, "Failed to read age.");

	write_printf("------ RESULR ------\n");
	write_printf("First name: %s\n", first_name);
	write_printf("Last name: %s\n", last_name);
	write_printf("Initial: %c\n", initial);
	write_printf("Age: %d\n", age);

	free(first_name);
	free(last_name);
	return 0;

error:
	return 1;

}
