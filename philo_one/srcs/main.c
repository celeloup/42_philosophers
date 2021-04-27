/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celeloup <celeloup@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/12 14:33:09 by celeloup          #+#    #+#             */
/*   Updated: 2021/04/22 11:57:22 by celeloup         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo_one.h"

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
	the_watcher = malloc(sizeof(pthread_t *));
	pthread_create(the_watcher, NULL, watching, philosophers);
	i = 0;
	pthread_join(*the_watcher, NULL);
	while (i < parameters.nb_philo)
	{
		pthread_join(*philosophers[i].thread, NULL);
		i++;
	}
	free_it_all(parameters, &forks, &philosophers, &the_watcher);
	return (0);
}

void	*watching(void *args)
{
	int			i;
	int			full;
	t_philo		*philo;

	philo = (t_philo *)args;
	usleep(philo[0].param->time_die * 1000);
	i = 0;
	full = 0;
	while (1)
	{
		if (get_time() > philo[i].time_death && (philo[0].param->nb_meal < 0
				|| philo[i].nb_meal < philo[i].param->nb_meal))
			break ;
		if (monitoring(&philo, &full, &i) == 1)
			break ;
		usleep(10);
	}
	did_someone_die(&philo, i, full);
	return (NULL);
}

int		monitoring(t_philo **philo, int *full, int *i)
{
	if ((*philo)[0].param->nb_meal > 0
		&& (*philo)[*i].nb_meal >= (*philo)[*i].param->nb_meal)
		(*full)++;
	if (*i < (*philo)[*i].param->nb_philo - 1)
		(*i)++;
	else if (*full != (*philo)[*i].param->nb_philo)
	{
		*i = 0;
		*full = 0;
	}
	else if ((*philo)[0].param->nb_meal > 0)
		return (1);
	else
		*i = 0;
	return (0);
}

void	did_someone_die(t_philo **philosophers, int i, int full_philosophers)
{
	if (full_philosophers != (*philosophers)[i].param->nb_philo
		|| (*philosophers)[0].param->nb_meal < 0)
	{
		message((*philosophers)[i].id, DIE, (*philosophers)[i].param);
		(*philosophers)[0].param->stop = 0;
	}
}
