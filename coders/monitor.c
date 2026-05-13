#include "include/codexion.h"

int	is_stopped(t_sim *sim)
{
	pthread_mutex_lock(&sim->log_lock);
		if (sim->stop == 1)
		{
			pthread_mutex_unlock(&sim->log_lock);
			return (1);
		}
	pthread_mutex_unlock(&sim->log_lock);
	return (0);
}

void	*monitor_routine(void *arg)
{
	int			i;
	int			n;
	t_coder		*coder;
	t_sim		*sim;

	sim = (t_sim *)arg;
	while (1)
	{
		if (is_stopped(sim) != 0)
			return (NULL);
		i = 0;
		n = 0;
		while (i < sim->n_coders)
		{
			coder = sim->coders[i];
			pthread_mutex_lock(&coder->lock);
			if (coder->compiles_done < sim->compiles_required
				&& elapsed_ms(coder->last_compile_start_ms) >= sim->time_to_burnout)
			{
				log_msg(coder, "Burnout");
				pthread_mutex_unlock(&coder->lock);
				pthread_mutex_lock(&sim->log_lock);
				sim->stop = 1;
				pthread_mutex_unlock(&sim->log_lock);
				return (NULL);
			}
			else
				pthread_mutex_unlock(&coder->lock);
			pthread_mutex_lock(&coder->lock);
			n += coder->compiles_done;
			pthread_mutex_unlock(&coder->lock);
			i++;
		}
		if (n == sim->n_coders * sim->compiles_required)
			return (NULL);
		usleep(1000);
	}
}