#include "get_next_line.h"
#include <fcntl.h>
int	main(int argc, char *argv[])
{
	int	opened;

	(void) argc;
	opened = open(argv[1], O_RDONLY);
	if (opened == -1)
	{
		printf("Nope");
		return (1);
	}
	while (1)
	{
		printf("%s\n\n", get_next_line(opened));
	}
}
