#ifndef PHILO_ONE_H
# define PHILO_ONE_H

# include <stdio.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>
# include <stdint.h>

# define FORK 0
# define EAT 1
# define SLEEP 2
# define THINK 3
# define DIE 4

int			ft_strlen(char const *str);
int			ft_atoi(char const *str);
void		ft_putnbr_fd(uint64_t n, int fd);
uint64_t	get_time(void);

#endif