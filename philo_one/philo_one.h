/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_one.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celeloup <celeloup@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/27 16:12:40 by celeloup          #+#    #+#             */
/*   Updated: 2021/04/27 16:12:42 by celeloup         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_ONE_H
# define PHILO_ONE_H

# include <stdio.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
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

typedef struct		s_params
{
	int				nb_philo;
	int				time_die;
	int				time_eat;
	int				time_sleep;
	int				nb_meal;
	uint64_t		start;
	pthread_mutex_t	*write_lock;
	int				stop;
}					t_params;

typedef struct		s_philo
{
	int				id;
	uint64_t		time_death;
	int				nb_meal;
	pthread_t		*thread;
	t_params		*param;
	pthread_mutex_t	**forks;
}					t_philo;

int					ft_strlen(char const *str);
int					ft_atoi(char const *str);
void				ft_putnbr_fd(uint64_t n, int fd);
uint64_t			get_time(void);

void				*watching(void *args);
int					monitoring(t_philo **philo, int *full, int *i);
void				did_someone_die(t_philo **philosophers, int i,
						int full_philosophers);

void				*philosophizing(void *args);
void				eating(t_philo *self, int left, int right);
void				sleeping(t_philo *self);
int					message(int philosopher, int type, t_params *params);

int					parser(t_params *param, char **args);
int					usage(char *programme);
void				initialisation(t_params *parameters, t_philo **philosophers,
						pthread_mutex_t **forks);
void				free_it_all(t_params params, pthread_mutex_t **forks,
						t_philo **philosophers, pthread_t **watcher);

#endif
