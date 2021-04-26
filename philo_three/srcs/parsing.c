/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celeloup <celeloup@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/21 16:38:11 by celeloup          #+#    #+#             */
/*   Updated: 2021/04/26 17:33:20 by celeloup         ###   ########.fr       */
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
	sem_unlink("write");
	param->write_lock = sem_open("write", O_CREAT, 644, 1);
	sem_unlink("forks");
	param->forks = sem_open("forks", O_CREAT, 644, param->nb_philo);
	sem_unlink("death");
	param->death_event = sem_open("death", O_CREAT, 644, param->nb_philo);
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

void	initialisation(t_params *param)
{
	int	i;
	pid_t process;

	i = 0;
	process = 1;
	while (i < param->nb_philo)
	{
		sem_wait(param->death_event);
		i++;
	}
	i = 1;
	param->start = get_time();
	while (i < param->nb_philo + 1 && process != 0)
	{
		process = fork();
		if (process == -1)
			exit(EXIT_FAILURE);
		else if (process == 0)
			philosophizing(i, param);
		else
			i++;
	}
}

void	free_it_all(t_params param)
{
	sem_close(param.forks);
	sem_close(param.write_lock);
	sem_close(param.death_event);
}
