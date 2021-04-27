/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celeloup <celeloup@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/21 16:39:49 by celeloup          #+#    #+#             */
/*   Updated: 2021/04/27 16:54:17 by celeloup         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo_three.h"

void	philosophizing(int id, t_params *param)
{
	pthread_t		death_notice;
	pthread_t		monitor;

	param->nb_meal_ate = 0;
	param->id = id;
	pthread_create(&death_notice, NULL, did_anyone_die, param->death_event);
	param->time_death = get_time() + param->time_die;
	pthread_create(&monitor, NULL, monitoring, param);
	while (1)
	{
		if (eating(param, id) == -1)
			message(id, DIE, param);
		if (sleeping(param, id) == -1)
			message(id, DIE, param);
		message(id, THINK, param);
		if (param->nb_meal > 0)
			param->nb_meal_ate++;
		if (param->nb_meal_ate == param->nb_meal)
			break ;
	}
	exit(EXIT_SUCCESS);
}

int		eating(t_params *param, int id)
{
	uint64_t done;

	sem_wait(param->forks);
	message(id, FORK, param);
	sem_wait(param->forks);
	message(id, FORK, param);
	message(id, EAT, param);
	done = get_time() + param->time_eat;
	param->time_death = get_time() + param->time_die;
	while (done > get_time())
		usleep(80);
	sem_post(param->forks);
	sem_post(param->forks);
	return (0);
}

int		sleeping(t_params *param, int id)
{
	uint64_t done;

	message(id, SLEEP, param);
	done = get_time() + param->time_sleep;
	while (done > get_time())
		usleep(80);
	return (0);
}

int		message(int philosopher, int type, t_params *params)
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
