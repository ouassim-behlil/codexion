#include "include/codexion.h"

int	init_simulation(t_sim *sim, int ac, const char **av)
{
	if (!sim || !av)
		return (-1);
	if (parse_args(ac, av, sim) != 0)
		return (-1);
	sim->n_dongles = sim->n_coders;
	sim->stop = 0;
	if (init_coders(sim) != 0)
		return (-1);
	sim->dongles = init_dongles(sim);
	if (!(sim->dongles))
	{
		destroy_coders(sim->coders);
		return (-1);
	}
	pthread_mutex_init(&sim->log_lock, NULL);
	// we should modify start_ms at the start of simulation
	// sim->start_ms = get_time_ms();
	return (0);
}

void	destroy_simulation(t_sim *sim)
{
	if (!sim)
		return ;
	destroy_coders(sim->coders);
	destroy_dongles(sim->dongles);
	pthread_mutex_destroy(&sim->log_lock);
	free(sim);
}
