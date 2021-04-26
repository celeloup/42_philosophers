/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celeloup <celeloup@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/12 14:33:09 by celeloup          #+#    #+#             */
/*   Updated: 2021/04/26 12:11:02 by celeloup         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo_three.h"

#include <signal.h>

int	message_test(int philosopher, int type, uint64_t time_start, sem_t **write_lock)
{
	sem_wait(*write_lock);
	if (type == FORK)
		printf("%" PRIu64 " %d has taken a fork\n", get_time()
			- time_start, philosopher);
	else if (type == EAT)
		printf("%" PRIu64 " %d is eating\n", get_time()
			- time_start, philosopher);
	else if (type == SLEEP)
		printf("%" PRIu64 " %d is sleeping\n", get_time()
			- time_start, philosopher);
	else if (type == THINK)
		printf("%" PRIu64 " %d is thinking\n", get_time()
			- time_start, philosopher);
	else
		printf("%" PRIu64 " %d is dead\n", get_time()
			- time_start, philosopher);
	if (type != DIE)
		sem_post(*write_lock);
	return (0);
}

int eat_test(int id, int time_eat, uint64_t time_start, uint64_t *time_to_die, sem_t **forks, sem_t **write_lock) {
	sem_wait(*forks);
	if (*time_to_die > get_time())
		sem_wait(*forks);
	else
		return (-1);
	message_test(id, EAT, time_start, write_lock);
	uint64_t done = get_time() + time_eat;
	*time_to_die = get_time() + 100;
	while(done > get_time())
	{
		if (*time_to_die < get_time())
			break ;
		usleep(50);
	}
	sem_post(*forks);
	sem_post(*forks);
	if (*time_to_die < get_time())
			return (-1);
	return (0);
}

int sleep_test(int id, int time_sleep, uint64_t time_start, uint64_t time_to_die, sem_t **write_lock) {
	message_test(id, SLEEP, time_start, write_lock);
	uint64_t done = get_time() + time_sleep;
	while(done > get_time())
	{
		if (time_to_die < get_time())
			return (-1);
		usleep(50);
	}
	return (0);
}

void *monitoring_test(void *death_notice)
{
	sem_t *death = (sem_t*)death_notice;
	sem_wait(death);
	printf("i know we dead !\n");
	exit(0);
	return(NULL);
}

int philosophising_test(int id, int time_die, int time_eat, int time_sleep, uint64_t time_start, sem_t **forks, sem_t **write_lock, sem_t *death_notice)
{
	pthread_t monitor;
	pthread_create(&monitor, NULL, monitoring_test, death_notice);
	uint64_t time_to_die = get_time() + time_die;
	while (1)
	{
		if (eat_test(id, time_eat, time_start, &time_to_die, forks, write_lock) == -1)
		{
			message_test(id, DIE, time_start, write_lock);
			exit(EXIT_FAILURE);
		}
		if (sleep_test(id, time_sleep, time_start, time_to_die, write_lock) == -1)
		{
			message_test(id, DIE, time_start, write_lock);
			exit(EXIT_FAILURE);
		}
		message_test(id, THINK, time_start, write_lock);
	}
	exit(EXIT_FAILURE);
}

int main()
{
	int time_die = 100;
	int time_eat = 200;
	int time_sleep = 200;
	int nb_philo = 6;
	pid_t process = 1;
	sem_t *forks;
	int test[nb_philo];

	sem_unlink("forks");
	forks = sem_open("forks", O_CREAT, 644, nb_philo);

	sem_unlink("write");
	sem_t *write_lock = sem_open("write", O_CREAT, 644, 1);

	sem_unlink("death");
	sem_t *death_event = sem_open("death", O_CREAT, 644, nb_philo);

	int i;
	i = 0;
	while (i < nb_philo)
	{
		sem_wait(death_event);
		i++;
	}
	i = 1;
	uint64_t start_time = get_time();
	while (i < nb_philo + 1 && process != 0)
	{
		process = fork();
		test[i - 1] = process;
		if (process == -1)
			exit(EXIT_FAILURE);
		else if (process == 0)
			philosophising_test(i, time_die, time_eat, time_sleep, start_time, &forks, &write_lock, death_event);
		else
			i++;
	}
	int status;
	i = 0;
	while (i < nb_philo)
	{
		waitpid(-1, &status, 0);
		if (status == 256)
			break;
		i++;
	}
	i = 0;
	while (i < nb_philo)
	{
		sem_post(death_event);
		// kill(test[i], SIGTERM);
		i++;
	}
	printf("hello :)\n");
	sem_close(forks);
	sem_close(write_lock);
	sem_close(death_event);
}


// int	main(int argc, char **argv)
// {
// 	t_params		parameters;
// 	t_philo			*philosophers;
// 	sem_t			*forks;
// 	pthread_t		*the_watcher;
// 	int				i;

// 	if (argc < 5 || argc > 6 || parser(&parameters, argv))
// 		return (usage(argv[0]));
// 	initialisation(&parameters, &philosophers, &forks);
// 	the_watcher = malloc(sizeof(pthread_t *));
// 	pthread_create(the_watcher, NULL, watching, philosophers);
// 	i = 0;
// 	pthread_join(*the_watcher, NULL);
// 	while (i < parameters.nb_philo)
// 	{
// 		pthread_join(*philosophers[i].thread, NULL);
// 		i++;
// 	}
// 	free_it_all(parameters, &forks, &philosophers, &the_watcher);
// 	return (0);
// }

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

int	monitoring(t_philo **philo, int *full, int *i)
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
