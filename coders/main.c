#include "include/codexion.h"

int	main(int argc, const char *argv[])
{
	t_sim	sim;

	if (init_simulation(&sim, argc, argv))
		return (-1);
	printf("All works well!\n");
	destroy_simulation(&sim);
	return (0);
}
