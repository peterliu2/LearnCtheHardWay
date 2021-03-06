#include <stdio.h>
#include "../common/include/dbg.h"

#define MAX_DATA 100

enum EyeColor {
	BLUE_EYES,
	GREEN_EYES,
	BROWN_EYES,
	BLACK_EYES,
	OTHER_EYES
};

const char *EYE_COLOR_NAMES[] = {
	"Bule",
	"Green",
	"Brown",
	"Black",
	"Others"
};

struct Person {
	int age;
	char first_name[MAX_DATA];
	char last_nmae[MAX_DATA];
	enum EyeColor eyes;
	float income;
};

int main(int argc, char *argv[])
{
	struct Person you = {.age = 0, .first_name = 'N', .last_nmae = 'N', .eyes = OTHER_EYES, .income = 0.0 };
	int i = 0;
	char *in = NULL;

	printf("What is your first name? ");
	in = fgets(you.first_name, MAX_DATA - 1, stdin);
	check(in != NULL, "Failed to read first name.");

	printf("What is your last name? ");
	in = fgets(you.last_nmae, MAX_DATA - 1, stdin);
	check(in != NULL, "Failed to read last name.");

	printf("How old are you? ");
	int rc = fscanf(stdin, "%d", &you.age);

	check(rc == 1 && you.age > 0, "You have to enter a positive number.");

	printf("What color are you eyes?\n");
	for (i = 0; i <= OTHER_EYES; i++)
		printf("%d) %s\n", i + 1, EYE_COLOR_NAMES[i]);
	printf("> ");

	int eyes = -1;

	rc = fscanf(stdin, "%d", &eyes);
	check(rc == 1 , "You have enter a number.");

	you.eyes = eyes - 1;
	check(you.eyes >= 0 && you.eyes <= OTHER_EYES, "Do it right, this is not an option.");

	printf("How much you make a hour? ");
	rc = fscanf(stdin, "%f", &you.income);
	check(rc == 1, "Enter a float point number.");

	printf("------ RESULT --------\n");
	printf("First name\t: %s", you.first_name);
	printf("Last name\t: %s", you.last_nmae);
	printf("Age\t\t: %d\n", you.age);
	printf("Eyes\t\t: %s\n", EYE_COLOR_NAMES[you.eyes]);
	printf("Income\t\t: %f\n", you.income);

	return 0;

error:
	return 1;
}
