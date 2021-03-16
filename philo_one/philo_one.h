#ifndef PHILO_ONE_H
# define PHILO_ONE_H

# include <stdio.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>
# include <stdint.h>
# include <stdlib.h>
# include <inttypes.h>

# define FORK 0
# define EAT 1
# define SLEEP 2
# define THINK 3
# define DIE 4

typedef struct 		s_params
{
	int				nb_philo;
	int				time_die;
	int				time_eat;
	int				time_sleep;
	int				nb_meal;
	uint64_t		start_time;
	pthread_mutex_t	write_lock;
}					t_params;

typedef struct 		s_philo
{
	int				id;
	uint64_t		time_death;
	int				nb_meal;
	pthread_t		thread;
	t_params		*params;
	pthread_mutex_t	**forks;
}					t_philo;

int			ft_strlen(char const *str);
int			ft_atoi(char const *str);
void		ft_putnbr_fd(uint64_t n, int fd);
uint64_t	get_time(void);

#endif