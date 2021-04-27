/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celeloup <celeloup@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/21 16:39:49 by celeloup          #+#    #+#             */
/*   Updated: 2021/04/21 15:56:56 by celeloup         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo_two.h"

void	*philosophizing(void *args)
{
	t_philo		*self;

	self = (t_philo *)args;
	usleep(self->param->time_eat * (self->id % 2) / 2);
	self->time_death = get_time() + self->param->time_die;
	while (self->param->stop)
	{
		eating(self);
		sleeping(self);
		message(self->id, THINK, self->param);
		if (self->param->nb_meal > 0)
			self->nb_meal++;
		if (self->param->nb_meal > 0 && self->nb_meal >= self->param->nb_meal)
			return (NULL);
	}
	return (NULL);
}

void	eating(t_philo *self)
{
	uint64_t	done_activity;

	if (self->param->stop == 0)
		return ;
	sem_wait(*(self->forks));
	sem_wait(*(self->forks));
	message(self->id, FORK, self->param);
	message(self->id, FORK, self->param);
	done_activity = get_time() + self->param->time_eat;
	self->time_death = get_time() + self->param->time_die;
	message(self->id, EAT, self->param);
	while (done_activity > get_time())
		usleep(50);
	sem_post(*(self->forks));
	sem_post(*(self->forks));
}

void	sleeping(t_philo *self)
{
	uint64_t	done_activity;

	message(self->id, SLEEP, self->param);
	done_activity = get_time() + self->param->time_sleep;
	while (done_activity > get_time())
		usleep(50);
}

int		message(int philosopher, int type, t_params *params)
{
	if (params->stop == 0)
		return (1);
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
