/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_three.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celeloup <celeloup@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/27 16:12:58 by celeloup          #+#    #+#             */
/*   Updated: 2021/04/27 16:13:00 by celeloup         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_THREE_H
# define PHILO_THREE_H

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
# include <semaphore.h>

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
	int				nb_meal_ate;
	int				id;
	uint64_t		start;
	uint64_t		time_death;
	sem_t			*write_lock;
	sem_t			*forks;
	sem_t			*death_event;
	pthread_t		death_notice;
	pthread_t		monitor;
}					t_params;

int					ft_strlen(char const *str);
int					ft_atoi(char const *str);
void				ft_putnbr_fd(uint64_t n, int fd);
uint64_t			get_time(void);

void				*monitoring(void *param);
void				*did_anyone_die(void *death_event);

void				philosophizing(int id, t_params *param);
int					eating(t_params *param, int id);
int					sleeping(t_params *param, int id);
int					message(int philosopher, int type, t_params *params);

int					parser(t_params *param, char **args);
int					usage(char *programme);
void				initialisation(t_params *parameters);
void				free_it_all(t_params param);

#endif
