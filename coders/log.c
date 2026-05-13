#include "include/codexion.h"

void	log_msg(t_coder *coder, const char *msg, int dongle_id)
{
	long	rel_time;

	if (!msg)
		return ;
	rel_time = get_time_ms() - coder->sim->start_ms;
	pthread_mutex_lock(&coder->sim->log_lock);
	if (coder->sim->stop == 1)
	{
		pthread_mutex_unlock(&coder->sim->log_lock);
		return ;
	}
	if (strcmp(msg, "takes dongle") == 0)
		fprintf(stdout, "%ld coder %d %s %d\n", rel_time, coder->id, msg, dongle_id);
	else
		fprintf(stdout, "%ld coder %d %s\n", rel_time, coder->id, msg);
	pthread_mutex_unlock(&coder->sim->log_lock);
}