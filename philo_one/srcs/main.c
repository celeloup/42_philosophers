#include "../includes/philo_one.h"

#include <stdlib.h>
#include <inttypes.h>

int nb_philo;
int time_to_die;
int time_to_eat;
int time_to_sleep;
int nb_lunch;
pthread_t	*philosophers;
pthread_mutex_t	*forks;
uint64_t start_time;

void	message(int philosopher, int type)
{
	if (type == FORK)
		printf("%" PRIu64 " %d has taken a fork\n",get_time() - start_time, philosopher + 1);
	else if (type == EAT)
		printf("%" PRIu64 " %d is eating\n",get_time() - start_time, philosopher + 1);
	else if (type == SLEEP)
		printf("%" PRIu64 " %d is sleeping\n",get_time() - start_time, philosopher + 1);
	else if (type == THINK)
		printf("%" PRIu64 " %d is thinking\n",get_time() - start_time, philosopher + 1);
	else
		printf("%" PRIu64 " %d is dead\n",get_time() - start_time, philosopher + 1);
}

void manger(int id)
{
	message(id, EAT);
	usleep(time_to_eat * 1000);
}

void *philosopher (void *arg)
{
	int *id = (int*)arg;
	int left = *id;
	int right = (left + 1) % nb_philo;
	start_time = get_time();
	uint64_t time_last_ate = get_time();
	int i = 1;
	while(i < nb_lunch + 1)
	{
		if (get_time() - time_last_ate > (uint64_t)time_to_die)
		{
			message(*id, DIE);
			exit(1);
		}
		if (left < right)
		{
			pthread_mutex_lock(&forks[left]);
			pthread_mutex_lock(&forks[right]);
		}
		else
		{
			pthread_mutex_lock(&forks[right]);
			pthread_mutex_lock(&forks[left]);
		}
		
		message(*id, FORK);
		message(*id, FORK);
		manger(*id);
		time_last_ate = get_time();
		pthread_mutex_unlock(&forks[left]);
		pthread_mutex_unlock(&forks[right]);
		message(*id, SLEEP);
		usleep(time_to_sleep * 1000);
		message(*id, THINK);
		if (nb_lunch != 0)
			i++;
	}
	return (NULL);
}


int main(int argc, char **argv) //int argc, char **argv
{
	if (argc < 5)
	{
		printf("usage: not good\n");
		return (EXIT_FAILURE);
	}
	nb_philo = ft_atoi(argv[1]);
	time_to_die = ft_atoi(argv[2]);
	time_to_eat = ft_atoi(argv[3]);
	time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		nb_lunch = ft_atoi(argv[5]);
	else
		nb_lunch = 0;
	long i;
	int id[nb_philo];

	for (i = 0; i < nb_philo; i++)
		id[i] = i;
	
	forks = malloc(sizeof(pthread_mutex_t) * nb_philo);
	philosophers = malloc(sizeof(pthread_t) * nb_philo);

	for (i = 0; i < nb_philo; i++)
	{
		pthread_mutex_init(&forks[i], NULL);
	}
		
	
	for (i = 0; i < nb_philo; i++)
		pthread_create(&philosophers[i], NULL, philosopher, (void*)&(id[i]));

	for (i = 0; i < nb_philo; i++)
		pthread_join(philosophers[i], NULL);

	return (0);
}