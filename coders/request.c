#include "include/codexion.h"

static void	push_request(t_request *req, t_dongle *dongle, t_policy sched)
{
	if (dongle->held == 1)
	{
		dongle->requests[1] = req;
		return ;
	}
	if (dongle->requests[0] == NULL)
	{
		dongle->requests[0] = req;
		return ;
	}
	if (sched == POLICY_FIFO)
		dongle->requests[1] = req;
	else
	{
		if (req->deadline_ms < dongle->requests[0]->deadline_ms)
		{
			dongle->requests[1] = dongle->requests[0];
			dongle->requests[0] = req;
		}
		else
			dongle->requests[1] = req;
	}
}

static int	init_request(t_request **req, t_coder *coder)
{
	*req = malloc(sizeof(t_request));
	if (!*req)
		return (-1);
	(*req)->coder_id = coder->id;
	(*req)->arrival_ms = get_time_ms();
	(*req)->deadline_ms = coder->last_compile_start_ms + coder->sim->time_to_burnout;
	return (0);
}

static int	waiting_loop(t_dongle *dongle, t_coder *coder)
{
	t_timespec		ts;

	while (dongle->requests[0]->coder_id != coder->id
		|| dongle->held == 1
		|| get_time_ms() < dongle->not_available_until_ms
	)
	{
		if (is_stopped(coder->sim) == 1)
		{
			pthread_mutex_unlock(&dongle->lock);
			return (-1);
		}
		if (dongle->requests[0]->coder_id != coder->id
			|| dongle->held == 1
		)
			pthread_cond_wait(&dongle->cv, &dongle->lock);
		if (dongle->held == 0 && dongle->requests[0]->coder_id == coder->id)
		{
			ts = ms_to_timespec(dongle->not_available_until_ms);
			pthread_cond_timedwait(&dongle->cv, &dongle->lock, &ts);
		}
	}
	return (0);
}

int	request_dongle(t_coder *coder, int dongle_id)
{
	t_request		*req;
	t_dongle		*dongle;

	if (init_request(&req, coder) != 0)
		return (-1);
	dongle = coder->sim->dongles[dongle_id - 1];
	pthread_mutex_lock(&dongle->lock);
	push_request(req, dongle, coder->sim->scheduler);
	if (waiting_loop(dongle, coder) != 0)
		return (-1);
	dongle->held = 1;
	pthread_mutex_unlock(&dongle->lock);
	return (0);
}
