#include "../philo_one.h"

#include <stdlib.h>
// #include <inttypes.h>

// int nb_philo;
// int time_to_die;
// int time_to_eat;
// int time_to_sleep;
// int nb_lunch;
// pthread_t	*philosophers;
// pthread_mutex_t	*forks;
// uint64_t start_time;

// void	message(int philosopher, int type)
// {
// 	if (type == FORK)
// 		printf("%" PRIu64 " %d has taken a fork\n",get_time() - start_time, philosopher + 1);
// 	else if (type == EAT)
// 		printf("%" PRIu64 " %d is eating\n",get_time() - start_time, philosopher + 1);
// 	else if (type == SLEEP)
// 		printf("%" PRIu64 " %d is sleeping\n",get_time() - start_time, philosopher + 1);
// 	else if (type == THINK)
// 		printf("%" PRIu64 " %d is thinking\n",get_time() - start_time, philosopher + 1);
// 	else
// 		printf("%" PRIu64 " %d is dead\n",get_time() - start_time, philosopher + 1);
// }

// void manger(int id)
// {
// 	message(id, EAT);
// 	usleep(time_to_eat * 1000);
// }

// void *philosopher (void *arg)
// {
// 	int *id = (int*)arg;
// 	int left = *id;
// 	int right = (left + 1) % nb_philo;
// 	start_time = get_time();
// 	uint64_t time_last_ate = get_time();
// 	int i = 1;
// 	while(i < nb_lunch + 1)
// 	{
// 		if (get_time() - time_last_ate > (uint64_t)time_to_die)
// 		{
// 			message(*id, DIE);
// 			exit(1);
// 		}
// 		if (left < right)
// 		{
// 			pthread_mutex_lock(&forks[left]);
// 			pthread_mutex_lock(&forks[right]);
// 		}
// 		else
// 		{
// 			pthread_mutex_lock(&forks[right]);
// 			pthread_mutex_lock(&forks[left]);
// 		}
		
// 		message(*id, FORK);
// 		message(*id, FORK);
// 		manger(*id);
// 		time_last_ate = get_time();
// 		pthread_mutex_unlock(&forks[left]);
// 		pthread_mutex_unlock(&forks[right]);
// 		message(*id, SLEEP);
// 		usleep(time_to_sleep * 1000);
// 		message(*id, THINK);
// 		if (nb_lunch != 0)
// 			i++;
// 	}
// 	return (NULL);
// }


// int main(int argc, char **argv) //int argc, char **argv
// {
// 	if (argc < 5)
// 	{
// 		printf("usage: not good\n");
// 		return (EXIT_FAILURE);
// 	}
// 	nb_philo = ft_atoi(argv[1]);
// 	time_to_die = ft_atoi(argv[2]);
// 	time_to_eat = ft_atoi(argv[3]);
// 	time_to_sleep = ft_atoi(argv[4]);
// 	if (argc == 6)
// 		nb_lunch = ft_atoi(argv[5]);
// 	else
// 		nb_lunch = 0;
// 	long i;
// 	int id[nb_philo];

// 	for (i = 0; i < nb_philo; i++)
// 		id[i] = i;
	
// 	forks = malloc(sizeof(pthread_mutex_t) * nb_philo);
// 	philosophers = malloc(sizeof(pthread_t) * nb_philo);

// 	for (i = 0; i < nb_philo; i++)
// 	{
// 		pthread_mutex_init(&forks[i], NULL);
// 	}
		
// 	for (i = 0; i < nb_philo; i++)
// 		pthread_create(&philosophers[i], NULL, philosopher, (void*)&(id[i]));

// 	for (i = 0; i < nb_philo; i++)
// 		pthread_join(philosophers[i], NULL);

// 	return (0);
// }

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
	if (!param->nb_philo || !param->time_die || !param->time_eat \
		|| !param->time_sleep || !param->nb_meal)
	{
		printf("Error: wrong argument.\n");
		return (1);
	}
	param->start_time = 0;
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

void	*philosophizing(void *args)
{
	t_philo	*self;

	self = (t_philo *)args;
	
	// while (self->params->start_time == 0)
	// {
	// 	usleep(50);
	// 	printf("not started yet");
	// }
	printf("philosopher %d is philosophizing, start-time = %llu\n", self->id, self->params->start_time);
	return (NULL);
}

void	initialisation(t_params *parameters, t_philo **philosophers, \
	pthread_mutex_t **forks)
{
	int i;

	*forks = malloc(sizeof(pthread_mutex_t) * parameters->nb_philo);
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
		(*philosophers)[i].time_death = 100;
		(*philosophers)[i].nb_meal = 0;
		(*philosophers)[i].thread = (pthread_t)malloc(sizeof(pthread_t));
		(*philosophers)[i].params = parameters;
		(*philosophers)[i].forks = forks;
		pthread_create(&((*philosophers)[i].thread), NULL, philosophizing, &(*philosophers)[i]);
		i++;
	}
}

// printf("nb_philo = %d\ntime_die = %d\ntime_eat = %d\ntime_sleep = %d\nnb_meal = %d\n", parameters.nb_philo, parameters.time_die, parameters.time_eat, parameters.time_sleep, parameters.nb_meal);

int main(int argc, char **argv)
{
	t_params parameters;
	t_philo *philosophers;
	pthread_mutex_t	*forks;

	if (argc < 5 || argc > 6 || parser(&parameters, argv))
		return(usage(argv[0]));
	initialisation(&parameters, &philosophers, &forks);
	int i = 0;
	while (i < parameters.nb_philo)
	{
		printf("%d %d %d\n", philosophers[i].id, philosophers[i].time_death, philosophers[i].nb_meal);
		i++;
	}
	return (0);
}