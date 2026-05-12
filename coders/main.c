#include "include/codexion.h"

int	main(int argc, const char *argv[])
{
	t_sim	sim;

	if (parse_args(argc, argv, &sim) != 0)
		return (-1);
	return (0);
}
