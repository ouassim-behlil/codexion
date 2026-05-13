#include "include/codexion.h"

static int	compile(t_coder *coder)
{
	int		first;
	int		second;
	long	now;

	coder->attempt_start_ms = get_time_ms();
	first = coder->left_dongle_id;
	second = coder->right_dongle_id;
	if (coder->left_dongle_id > coder->right_dongle_id)
	{
		first = coder->right_dongle_id;
		second = coder->left_dongle_id;
	}
	if (request_dongle(coder, first) != 0)
		return (-1);
	if (request_dongle(coder, second) != 0)
		return (-1);
	log_msg(coder, "takes dongle", first);
	log_msg(coder, "takes dongle", second);
	log_msg(coder, "is compiling", 0);
	now = get_time_ms();
	pthread_mutex_lock(&coder->lock);
	coder->last_compile_start_ms = now;
	pthread_mutex_unlock(&coder->lock);
	sleep_until_ms(now + coder->sim->time_to_compile, coder->sim);
	release_dongles(coder);
	return (0);
}

static void	debug(t_coder *coder)
{
	log_msg(coder, "is debugging", 0);
	sleep_until_ms(get_time_ms() + coder->sim->time_to_debug, coder->sim);
}

static void	refactor(t_coder *coder)
{
	log_msg(coder, "is refactoring", 0);
	sleep_until_ms(get_time_ms() + coder->sim->time_to_refactor, coder->sim);
	pthread_mutex_lock(&coder->lock);
	coder->compiles_done += 1;
	pthread_mutex_unlock(&coder->lock);
}

void	*coder_routine(void *arg)
{
	t_coder		*coder;
	int			done;

	coder = (t_coder *)arg;
	pthread_mutex_lock(&coder->lock);
	done = coder->compiles_done;
	pthread_mutex_unlock(&coder->lock);
	while (done < coder->sim->compiles_required)
	{
		if (compile(coder) != 0)
			return (NULL);
		debug(coder);
		refactor(coder);
		pthread_mutex_lock(&coder->lock);
		done = coder->compiles_done;
		pthread_mutex_unlock(&coder->lock);
	}
	return (NULL);
}