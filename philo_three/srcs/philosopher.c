/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celeloup <celeloup@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/21 16:39:49 by celeloup          #+#    #+#             */
/*   Updated: 2021/04/26 17:30:20 by celeloup         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo_three.h"

void *monitoring_thread(void *death_event)
{
	sem_t *death = (sem_t*)death_event;
	sem_wait(death);
	// printf("i know we dead !\n");
	exit(0);
	return(NULL);
}

void	philosophizing(int id, t_params *param)
{
	uint64_t	time_death;
	pthread_t	monitor;
	int			nb_meal_ate;

	nb_meal_ate = 0;
	pthread_create(&monitor, NULL, monitoring_thread, param->death_event);
	time_death = get_time() + param->time_die;
	usleep(param->time_eat * (id % 2) / 2);
	time_death = get_time() + param->time_die;
	while (1)
	{
		if (eating(param, id, &time_death) == -1)
		{
			message(id, DIE, param);
			printf("-------------- BAD EAT\n");
			exit(EXIT_FAILURE);
		}
		if (sleeping(param, id, time_death) == -1)
		{
			message(id, DIE, param);
			printf("-------------- BAD SLEEP\n");
			exit(EXIT_FAILURE);
		}
		message(id, THINK, param);
		if (param->nb_meal > 0)
			nb_meal_ate++;
		if (nb_meal_ate == param->nb_meal)
			break;
	}
	exit(EXIT_FAILURE);
}

int		eating(t_params *param, int id, uint64_t *time_death)
{
	uint64_t done;

	sem_wait(param->forks);
	if (*time_death > get_time())
		sem_wait(param->forks);
	else
		return (-1);
	message(id, FORK, param);
	message(id, FORK, param);
	message(id, EAT, param);
	done = get_time() + param->time_eat;
	*time_death = get_time() + 800;
	while(done > get_time())
	{
		if (*time_death < get_time())
			break ;
		usleep(80);
	}
	sem_post(param->forks);
	sem_post(param->forks);
	if (*time_death < get_time())
			return (-1);
	return (0);
}

int		sleeping(t_params *param, int id, uint64_t time_death)
{
	uint64_t done;

	message(id, SLEEP, param);
	done = get_time() + param->time_sleep;
	while(done > get_time())
	{
		if (time_death < get_time())
			return (-1);
		usleep(80);
	}
	return (0);
}

int	message(int philosopher, int type, t_params *params)
{
	sem_wait(params->write_lock);
	if (type == FORK)
		printf("%" PRIu64 " %d has taken a fork\n", get_time()
			- params->start, philosopher);
	else if (type == EAT)
		printf("%" PRIu64 " %d is eating\n", get_time()
			- params->start, philosopher);
	else if (type == SLEEP)
		printf("%" PRIu64 " %d is sleeping\n", get_time()
			- params->start, philosopher);
	else if (type == THINK)
		printf("%" PRIu64 " %d is thinking\n", get_time()
			- params->start, philosopher);
	else
		printf("%" PRIu64 " %d is dead\n", get_time()
			- params->start, philosopher);
	if (type != DIE)
		sem_post(params->write_lock);
	return (0);
}
