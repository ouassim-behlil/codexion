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
	if (init_dongles(sim) != 0)
	{
		destroy_coders(sim);
		return (-1);
	}
	pthread_mutex_init(&sim->log_lock, NULL);
	return (0);
}

void	destroy_simulation(t_sim *sim)
{
	if (!sim)
		return ;
	destroy_coders(sim);
	destroy_dongles(sim);
	pthread_mutex_destroy(&sim->log_lock);
}

void	start_simulation(t_sim *sim)
{
	int		i;

	sim->start_ms = get_time_ms();
	i = 0;
	while (i < sim->n_coders)
	{
		sim->coders[i]->last_compile_start_ms = sim->start_ms;
		pthread_create(&sim->coders[i]->thread, NULL, coder_routine, sim->coders[i]);
		i ++;	
	}
	pthread_create(&sim->monitor, NULL, monitor_routine, sim);
	i = 0;
	while (i < sim->n_coders)
	{
		pthread_join(sim->coders[i]->thread, NULL);
		i ++;
	}
	pthread_join(sim->monitor, NULL);
}