#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>
# include <limits.h>

typedef enum 
{
	ERR_OK = 0,
	ERR_INVALID_ARG = 1 << 0,
	ERR_OVERFLOW = 1 << 1,
	ERR_INVALID_NUM_PARAMS = 1 << 2,
	ERR_NULL_PTR = 1 << 3,
	ERR_EMPTY_STR = 1 << 4,
	ERR_BAD_SCHEDULER = 1 << 5,
}	t_error_code;

typedef enum e_policy
{
	POLICY_FIFO = 0,
	POLICY_EDF = 1
}	t_policy;

typedef struct s_sim	t_sim;
typedef struct s_coder	t_coder;
typedef struct s_dongle	t_dongle;
typedef struct timeval	t_time_val;

typedef struct s_request
{
	int			coder_id;
	long		arrival_ms;
	long		deadline_ms;
}	t_request;

struct s_dongle
{
	int					id;
	int					held;
	long				not_available_until_ms;
	t_request			*waiters[2];
	pthread_mutex_t		lock;
	pthread_cond_t		cv;
};

struct s_coder
{
	int					id;
	int					left_dongle_id;
	int					right_dongle_id;
	long				last_compile_start_ms;
	int					compiles_done;
	pthread_mutex_t		lock;
	pthread_t			thread;
	t_sim				*sim;
};

struct s_sim
{
	int					n_coders;
	long				time_to_burnout;
	long				time_to_compile;
	long				time_to_debug;
	long				time_to_refactor;
	int					compiles_required;
	long				dongle_cooldown;
	t_policy			scheduler;
	long				start_ms;
	int					stop;
	int					n_dongles;
	t_coder				**coders;
	t_dongle			**dongles;
	pthread_mutex_t		log_lock;
	pthread_t			monitor;
};

// parser.c
int		parse_args(int ac, const char **av, t_sim *sim);

// error.c
void	print_error(t_error_code err);

// time.c
long	get_time_ms(void);

// coder.c
int		init_coders(t_sim *sim);
void	destroy_coders(t_sim *sim);

// dongle.c
int		init_dongles(t_sim *sim);
void	destroy_dongles(t_sim *sim);

// simulation.c
int		init_simulation(t_sim *sim, int ac, const char **av);
void	destroy_simulation(t_sim *sim);

#endif
