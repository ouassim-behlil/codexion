#include "include/codexion.h"

long	get_time_ms(void)
{
	t_time_val		tv;

	gettimeofday(&tv, NULL);
	return ((long)(tv.tv_sec * 1000 + tv.tv_usec / 1000));
}
