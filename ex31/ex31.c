#include <unistd.h>

int main(int argc, const char *argv[])
{
	int i = 0;

	while (i < 100)
	{
		usleep(3000);
	}

	return 0;
}
