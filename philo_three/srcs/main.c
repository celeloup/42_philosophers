/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celeloup <celeloup@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/12 14:33:09 by celeloup          #+#    #+#             */
/*   Updated: 2021/04/27 10:57:09 by celeloup         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo_three.h"

int		main(int argc, char **argv)
{
	t_params	parameters;
	int			status;
	int			i;

	if (argc < 5 || argc > 6 || parser(&parameters, argv))
		return (usage(argv[0]));
	initialisation(&parameters);
	i = 0;
	while (i < parameters.nb_philo)
	{
		waitpid(-1, &status, 0);
		if (status == 256)
			break ;
		i++;
	}
	i = 0;
	while (i < parameters.nb_philo)
	{
		sem_post(parameters.death_event);
		i++;
	}
	free_it_all(parameters);
	return (0);
}

void	*did_anyone_die(void *death_event)
{
	sem_t *death;

	death = (sem_t*)death_event;
	sem_wait(death);
	exit(0);
}

void	*monitoring(void *param)
{
	t_params *parameters;
	uint64_t time;

	parameters = (t_params*)param;
	time = parameters->time_death;
	while (1)
	{
		if (time < get_time())
		{
			message(parameters->id, DIE, param);
			free_it_all(*parameters);
			exit(EXIT_FAILURE);
		}
		usleep(100);
		time = parameters->time_death;
	}
}
