/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celeloup <celeloup@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/12 14:33:09 by celeloup          #+#    #+#             */
/*   Updated: 2021/03/20 16:29:45 by celeloup         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo_one.h"

int		message(int philosopher, int type, t_params *params)
{
	if (params->stop == 0)
		return (1);
	pthread_mutex_lock(params->write_lock);
	if (type == FORK)
		printf("%" PRIu64 " %d has taken a fork\n", get_time() \
			- params->start, philosopher);
	else if (type == EAT)
		printf("%" PRIu64 " %d is eating\n", get_time() \
			- params->start, philosopher);
	else if (type == SLEEP)
		printf("%" PRIu64 " %d is sleeping\n", get_time() \
			- params->start, philosopher);
	else if (type == THINK)
		printf("%" PRIu64 " %d is thinking\n", get_time() \
			- params->start, philosopher);
	else
		printf("%" PRIu64 " %d is dead\n", get_time() \
			- params->start, philosopher);
	if (type != DIE)
		pthread_mutex_unlock(params->write_lock);
	return (0);
}

int		parser(t_params *param, char **args)
{
	param->nb_philo = ft_atoi(args[1]);
	param->time_die = ft_atoi(args[2]);
	param->time_eat = ft_atoi(args[3]);
	param->time_sleep = ft_atoi(args[4]);
	if (args[5])
		param->nb_meal = ft_atoi(args[5]);
	else
		param->nb_meal = -1;
	if (param->nb_philo <= 1 || param->time_die <= 0 || param->time_eat <= 0 \
		|| param->time_sleep <= 0 || (args[5] && param->nb_meal <= 0))
	{
		printf("Error: wrong argument.\n");
		return (1);
	}
	param->start = 0;
	param->write_lock = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(param->write_lock, NULL);
	param->stop = 1;
	return (0);
}

int		usage(char *programme)
{
	printf("usage: %s nb_philo time_die time_eat time_sleep [nb_meal]\n\
	nb_philo: number of philosophers (and forks)\n\
	time_die: (in ms) time before a philosopher starves to death\n\
	time_eat: (in ms) time a philosopher takes to eat\n\
	time_sleep: (in ms) time a philosopher takes to sleep\n\
	[nb_meal]: number of meals a philosopher must eat [optional]\n", programme);
	return (EXIT_FAILURE);
}

void	eating(t_philo *self, int left, int right)
{
	uint64_t	done_activity;

	if (self->param->stop == 0)
		return ;
	if (left < right)
	{
		pthread_mutex_lock(&(*self->forks)[left]);
		pthread_mutex_lock(&(*self->forks)[right]);
	}
	else
	{
		pthread_mutex_lock(&(*self->forks)[right]);
		pthread_mutex_lock(&(*self->forks)[left]);
	}
	message(self->id, FORK, self->param);
	message(self->id, FORK, self->param);
	done_activity = get_time() + self->param->time_eat;
	self->time_death = get_time() + self->param->time_die;
	message(self->id, EAT, self->param);
	while (done_activity > get_time())
		usleep(50);
	pthread_mutex_unlock(&(*self->forks)[left]);
	pthread_mutex_unlock(&(*self->forks)[right]);
}

void	sleeping(t_philo *self)
{
	uint64_t	done_activity;

	message(self->id, SLEEP, self->param);
	done_activity = get_time() + self->param->time_sleep;
	while (done_activity > get_time())
		usleep(50);
}

void	*philosophizing(void *args)
{
	t_philo		*self;
	int			left;
	int			right;

	self = (t_philo *)args;
	left = self->id - 1;
	right = (left + 1) % self->param->nb_philo;
	while (self->param->start == 0)
		usleep(50);
	self->time_death = get_time() + self->param->time_die;
	while (self->param->stop)
	{
		eating(self, left, right);
		sleeping(self);
		message(self->id, THINK, self->param);
		if (self->param->nb_meal > 0)
			self->nb_meal++;
		if (self->param->nb_meal > 0 && self->nb_meal >= self->param->nb_meal)
			return (NULL);
	}
	return (NULL);
}

void	initialisation(t_params *parameters, t_philo **philosophers, \
	pthread_mutex_t **forks)
{
	int i;

	*forks = malloc(sizeof(pthread_mutex_t) * (parameters->nb_philo));
	i = 0;
	while (i < parameters->nb_philo)
	{
		pthread_mutex_init(&(*forks)[i], NULL);
		i++;
	}
	*philosophers = malloc(sizeof(t_philo) * parameters->nb_philo);
	i = 0;
	while (i < parameters->nb_philo)
	{
		(*philosophers)[i].id = i + 1;
		(*philosophers)[i].time_death = 0;
		(*philosophers)[i].nb_meal = 0;
		(*philosophers)[i].thread = malloc(sizeof(pthread_t*));
		(*philosophers)[i].param = parameters;
		(*philosophers)[i].forks = forks;
		pthread_create((*philosophers)[i].thread, NULL, philosophizing, \
			&(*philosophers)[i]);
		i++;
	}
	parameters->start = get_time();
}

void	*watching(void *args)
{
	int			i;
	int			full_philosophers;
	t_philo		*philosophers;
	uint64_t	start;

	philosophers = (t_philo*)args;
	start = philosophers[0].param->start;
	usleep(philosophers[0].param->time_die * 1000);
	i = 0;
	full_philosophers = 0;
	while (1)
	{
		if (get_time() > philosophers[i].time_death \
			&& (philosophers[0].param->nb_meal < 0 \
			|| philosophers[i].nb_meal < philosophers[i].param->nb_meal))
			break ;
		if (philosophers[0].param->nb_meal > 0 \
			&& philosophers[i].nb_meal >= philosophers[i].param->nb_meal)
			full_philosophers++;
		if (i < philosophers[i].param->nb_philo - 1)
			i++;
		else if (full_philosophers != philosophers[i].param->nb_philo)
		{
			i = 0;
			full_philosophers = 0;
		}
		else if (philosophers[0].param->nb_meal > 0)
			break ;
		else
			i = 0;
	}
	if (full_philosophers != philosophers[i].param->nb_philo \
		|| philosophers[0].param->nb_meal < 0)
	{
		message(philosophers[i].id, DIE, philosophers[i].param);
		philosophers[0].param->stop = 0;
	}
	return (NULL);
}

void	free_it_all(t_params params, pthread_mutex_t *forks, \
	t_philo *philosophers, pthread_t *watcher)
{
	int i;

	i = 0;
	while (i < params.nb_philo)
	{
		pthread_detach(*philosophers[i].thread);
		pthread_mutex_destroy(&forks[i]);
		free(philosophers[i].thread);
		i++;
	}
	pthread_detach(*watcher);
	free(watcher);
	free(forks);
	free(philosophers);
	free(params.write_lock);
}

int		main(int argc, char **argv)
{
	t_params		parameters;
	t_philo			*philosophers;
	pthread_mutex_t	*forks;
	pthread_t		*the_watcher;
	int				i;

	if (argc < 5 || argc > 6 || parser(&parameters, argv))
		return (usage(argv[0]));
	initialisation(&parameters, &philosophers, &forks);
	the_watcher = malloc(sizeof(pthread_t*));
	pthread_create(the_watcher, NULL, watching, philosophers);
	i = 0;
	pthread_join(*the_watcher, NULL);
	while (i < parameters.nb_philo)
	{
		pthread_join(*philosophers[i].thread, NULL);
		i++;
	}
	free_it_all(parameters, forks, philosophers, the_watcher);
	return (0);
}
