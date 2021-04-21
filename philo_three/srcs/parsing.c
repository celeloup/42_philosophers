/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celeloup <celeloup@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/21 16:38:11 by celeloup          #+#    #+#             */
/*   Updated: 2021/04/21 16:49:49 by celeloup         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo_three.h"

int	parser(t_params *param, char **args)
{
	param->nb_philo = ft_atoi(args[1]);
	param->time_die = ft_atoi(args[2]);
	param->time_eat = ft_atoi(args[3]);
	param->time_sleep = ft_atoi(args[4]);
	if (args[5])
		param->nb_meal = ft_atoi(args[5]);
	else
		param->nb_meal = -1;
	if (param->nb_philo <= 1 || param->time_die <= 0 || param->time_eat <= 0
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

int	usage(char *programme)
{
	printf("usage: %s nb_philo time_die time_eat time_sleep [nb_meal]\n\
	nb_philo: number of philosophers (and forks)\n\
	time_die: (in ms) time before a philosopher starves to death\n\
	time_eat: (in ms) time a philosopher takes to eat\n\
	time_sleep: (in ms) time a philosopher takes to sleep\n\
	[nb_meal]: number of meals a philosopher must eat [optional]\n", programme);
	return (EXIT_FAILURE);
}

void	initialisation(t_params *parameters, t_philo **philosophers,
	sem_t **forks)
{
	int	i;

	sem_unlink("forks");
	*forks = sem_open("forks", O_CREAT, 644, parameters->nb_philo);
	if (*forks == SEM_FAILED)
		printf("NULL !");
	*philosophers = malloc(sizeof(t_philo) * parameters->nb_philo);
	i = 0;
	parameters->start = get_time();
	while (i < parameters->nb_philo)
	{
		(*philosophers)[i].id = i + 1;
		(*philosophers)[i].time_death = 0;
		(*philosophers)[i].nb_meal = 0;
		(*philosophers)[i].thread = malloc(sizeof(pthread_t *));
		(*philosophers)[i].param = parameters;
		(*philosophers)[i].forks = forks;
		pthread_create((*philosophers)[i].thread, NULL, philosophizing,
			&(*philosophers)[i]);
		i++;
	}
}

void	free_it_all(t_params params, sem_t **forks,
	t_philo **philosophers, pthread_t **watcher)
{
	int	i;

	i = 0;
	while (i < params.nb_philo)
	{
		free((*philosophers)[i].thread);
		i++;
	}
	free(*watcher);
	sem_close(*forks);
	free(*philosophers);
	pthread_mutex_destroy(params.write_lock);
	free(params.write_lock);
}
