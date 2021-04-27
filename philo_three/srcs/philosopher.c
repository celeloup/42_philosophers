/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celeloup <celeloup@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/21 16:39:49 by celeloup          #+#    #+#             */
/*   Updated: 2021/04/27 12:31:16 by celeloup         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo_three.h"

void *didAnyoneDie(void *death_event)
{
	sem_t *death = (sem_t*)death_event;
	sem_wait(death);
	exit(0);
	return(NULL);
}

void *monitoring(void *param)
{
	t_params *parameters = (t_params*)param;
	uint64_t time = parameters->time_death;
	while (1)
	{
		if (time < get_time())
		{
			// printf("DEAAADDD !!\n");
			message(parameters->id, DIE, param);
			exit(EXIT_FAILURE);
		}
		usleep(100);
		time = parameters->time_death;
	}
}

void	philosophizing(int id, t_params *param)
{
	uint64_t	time_death_test;
	// pthread_t	death_notice;
	// int			nb_meal_ate;
	// pthread_t	monitor;

	param->nb_meal_ate = 0;
	param->id = id;
	pthread_create(&(param->death_notice), NULL, didAnyoneDie, param->death_event);
	time_death_test = get_time() + param->time_die;
	param->time_death = time_death_test;
	pthread_create(&(param->monitor), NULL, monitoring, param);
	// usleep(param->time_eat * (id % 2) / 2);
	while (1)
	{
		if (eating(param, id) == -1)
		{
			message(id, DIE, param);
			// printf("-------------- BAD EAT\n");
			// exit(EXIT_FAILURE);
		}
		if (sleeping(param, id) == -1)
		{
			message(id, DIE, param);
			// printf("-------------- BAD SLEEP\n");
			// exit(EXIT_FAILURE);
		}
		message(id, THINK, param);
		if (param->nb_meal > 0)
			param->nb_meal_ate++;
		if (param->nb_meal_ate == param->nb_meal)
			break;
	}
	exit(EXIT_SUCCESS);
}

int		eating(t_params *param, int id)
{
	uint64_t done;

	sem_wait(param->forks);
	message(id, FORK, param);
	// if (*time_death > get_time())
		sem_wait(param->forks);
	// else
		// return (-1);
	message(id, FORK, param);
	message(id, EAT, param);
	done = get_time() + param->time_eat;
	param->time_death = get_time() + param->time_die;
	while(done > get_time())
	{
		// if (*time_death < get_time())
			// break ;
		usleep(80);
	}
	sem_post(param->forks);
	sem_post(param->forks);
	// if (*time_death < get_time())
		// return (-1);
	return (0);
}

int		sleeping(t_params *param, int id)
{
	uint64_t done;

	message(id, SLEEP, param);
	done = get_time() + param->time_sleep;
	while(done > get_time())
	{
		// if (param->time_death < get_time())
		// 	return (-1);
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
