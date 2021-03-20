/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celeloup <celeloup@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/12 14:33:09 by celeloup          #+#    #+#             */
/*   Updated: 2021/03/20 14:05:29 by celeloup         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo_one.h"

int		message(int philosopher, int type, uint64_t start_time, pthread_mutex_t *write_lock)
{
	pthread_mutex_lock(write_lock);
	if (type == FORK)
		printf("%" PRIu64 " %d has taken a fork\n", get_time() \
			- start_time, philosopher);
	else if (type == EAT)
		printf("%" PRIu64 " %d is eating\n", get_time() \
			- start_time, philosopher);
	else if (type == SLEEP)
		printf("%" PRIu64 " %d is sleeping\n", get_time() \
			- start_time, philosopher);
	else if (type == THINK)
		printf("%" PRIu64 " %d is thinking\n", get_time() \
			- start_time, philosopher);
	else
		printf("**************** %" PRIu64 " %d is dead *****************\n", get_time() \
			- start_time, philosopher);
	if (type != DIE)
		pthread_mutex_unlock(write_lock);
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
	if (!param->nb_philo || !param->time_die || !param->time_eat \
		|| !param->time_sleep || !param->nb_meal)
	{
		printf("Error: wrong argument.\n");
		return (1);
	}
	param->start_time = 0;
	param->write_lock = malloc(sizeof(pthread_mutex_t*));
	param->stop = 1;
	pthread_mutex_init(param->write_lock, NULL);
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
	if (self->params->stop == 1)
		message(self->id, FORK, self->params->start_time, self->params->write_lock);
	if (self->params->stop == 1)
		message(self->id, FORK, self->params->start_time, self->params->write_lock);
	done_activity = get_time() + self->params->time_eat;
	self->time_death = get_time() + self->params->time_die;
	if (self->params->stop == 1)
		message(self->id, EAT, self->params->start_time, self->params->write_lock);
	while (done_activity > get_time())
		usleep(50);
	pthread_mutex_unlock(&(*self->forks)[left]);
	pthread_mutex_unlock(&(*self->forks)[right]);
}

void	*philosophizing(void *args)
{
	t_philo		*self;
	int			i;
	uint64_t	done_activity;
	int			left;
	int			right;

	self = (t_philo *)args;
	left = self->id - 1;
	right = (left + 1) % self->params->nb_philo;
	i = 1;
	while (self->params->start_time == 0)
		usleep(50);
	self->time_death = get_time() + self->params->time_die;
	while (self->params->stop)
	{
		if (self->params->stop == 1)
			eating(self, left, right);
		if (self->params->stop == 1)
			message(self->id, SLEEP, self->params->start_time, self->params->write_lock);
		done_activity = get_time() + self->params->time_sleep;
		while (done_activity > get_time())
			usleep(50);
		if (self->params->stop == 1)
			message(self->id, THINK, self->params->start_time, self->params->write_lock);
		i++;
		if (self->params->nb_meal > 0)
			self->nb_meal++;
		if (self->params->nb_meal > 0 && self->nb_meal >= self->params->nb_meal)
			return (NULL);
	}
	return (NULL);
}

void	initialisation(t_params *parameters, t_philo **philosophers, \
	pthread_mutex_t **forks)
{
	int i;

	*forks = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t) * parameters->nb_philo);
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
		(*philosophers)[i].params = parameters;
		(*philosophers)[i].forks = forks;
		pthread_create((*philosophers)[i].thread, NULL, philosophizing, &(*philosophers)[i]);
		i++;
	}
	parameters->start_time = get_time();
}

// printf("philo %d time death = %llu, time now = %llu\n", philosophers[i].id, philosophers[i].time_death, get_time());

void	*the_watcher(void *args)
{
	int			i;
	int			full_philosophers;
	t_philo		*philosophers;
	uint64_t	start_time;

	philosophers = (t_philo*)args;
	start_time = philosophers[0].params->start_time;
	usleep(philosophers[0].params->time_die * 1000);
	i = 0;
	full_philosophers = 0;
	while (1)
	{
		if (get_time() > philosophers[i].time_death && (philosophers[0].params->nb_meal < 0 || philosophers[i].nb_meal < philosophers[i].params->nb_meal))
			break ;
		if (philosophers[0].params->nb_meal > 0 && philosophers[i].nb_meal >= philosophers[i].params->nb_meal)
			full_philosophers++;
		if (i < philosophers[i].params->nb_philo - 1)
			i++;
		else if (full_philosophers != philosophers[i].params->nb_philo)
		{
			i = 0;
			full_philosophers = 0;
		}
		else if (philosophers[0].params->nb_meal > 0)
			break ;
		else
			i = 0;
	}
	// printf("out while full_philo = %d\n", full_philosophers);
	if (full_philosophers != philosophers[i].params->nb_philo || philosophers[0].params->nb_meal < 0)
	{
		message(philosophers[i].id, DIE, philosophers[i].params->start_time, philosophers[i].params->write_lock);
		philosophers[0].params->stop = 0;
	}
	return (NULL);
}

void	free_it_all(t_params params, pthread_mutex_t *forks, t_philo *philosophers)
{
	int i;

	i = 0;
	// printf("freeee !!!!");
	while (i < params.nb_philo)
	{
		pthread_detach(*philosophers[i].thread);
		pthread_mutex_destroy(&forks[i]);
		free(philosophers[i].thread);
		i++;
	}
	free(forks);
	free(philosophers);
	free(params.write_lock);
}

int		main(int argc, char **argv)
{
	t_params		parameters;
	t_philo			*philosophers;
	pthread_mutex_t	*forks;
	pthread_t		*watcher;
	int				i;

	if (argc < 5 || argc > 6 || parser(&parameters, argv))
		return (usage(argv[0]));
	initialisation(&parameters, &philosophers, &forks);
	watcher = malloc(sizeof(pthread_t*));
	pthread_create(watcher, NULL, the_watcher, philosophers);
	i = 0;
	pthread_join(*watcher, NULL);
	while (i < parameters.nb_philo)
	{
		pthread_join(*philosophers[i].thread, NULL);
		i++;
	}
	free_it_all(parameters, forks, philosophers);
	return (0);
}
