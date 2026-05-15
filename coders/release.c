#include "include/codexion.h"

static void	release_dongle(t_dongle *dongle, long cooldown)
{
	pthread_mutex_lock(&dongle->lock);
	dongle->held = 0;
	free(dongle->requests[0]);
	dongle->requests[0] = dongle->requests[1];
	dongle->requests[1] = NULL;
	dongle->not_available_until_ms = get_time_ms() + cooldown;
	pthread_cond_broadcast(&dongle->cv);
	pthread_mutex_unlock(&dongle->lock);
}

void	release_dongles(t_coder *coder)
{
	t_dongle	*left;
	t_dongle	*right;

	left = coder->sim->dongles[coder->left_dongle_id - 1];
	right = coder->sim->dongles[coder->right_dongle_id - 1];
	release_dongle(left, coder->sim->dongle_cooldown);
	release_dongle(right, coder->sim->dongle_cooldown);
}