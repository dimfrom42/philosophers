/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dim <dim@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/04 16:39:19 by dim               #+#    #+#             */
/*   Updated: 2022/02/11 21:25:24 by dim              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	print_state(t_personal *philo, char *msg, int eat_flag)
{
	long	time;

	pthread_mutex_lock(philo->info->mutex_for_print);
	if (philo->info->alive)
	{
		time = get_mstime() - philo->info->start_time;
		printf("%ldms", time);
		printf(" %d %s\n", philo->name, msg);
		(void)eat_flag;
		// if (eat_flag)
		// {
		// 	// philo->time_last_eat = time;
		// 	philo->cnt_eaten++;
		// }
	}
	pthread_mutex_unlock(philo->info->mutex_for_print);
}

void	eating(t_personal *philo)
{
	pthread_mutex_lock(philo->info->mutex_for_check);
	print_state(philo, "is eating", 1);
	if (philo->info->alive)
	{
		ft_usleep(philo->info->time_eat);
		philo->time_last_eat = get_mstime();
		philo->cnt_eaten++;
	}
	pthread_mutex_lock(philo->info->mutex_for_check);
}

void	sleeping(t_personal *philo)
{
	print_state(philo, "is sleeping", 0);
	if (philo->info->alive)
		ft_usleep(philo->info->time_sleep);
}

void	thinking(t_personal *philo)
{
	print_state(philo, "is thinking", 0);
}

void	philo_odd(t_personal *philo)
{
	pthread_mutex_lock(philo->right_fork);
	print_state(philo, "has taken a fork", 0);
	pthread_mutex_lock(philo->left_fork);
	print_state(philo, "has taken a fork", 0);
	eating(philo);
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
	// sleeping(philo);
	// thinking(philo);
}

void	philo_even(t_personal *philo)
{
	pthread_mutex_lock(philo->left_fork);
	print_state(philo, "has taken a fork", 0);
	pthread_mutex_lock(philo->right_fork);
	print_state(philo, "has taken a fork", 0);
	eating(philo);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
	// sleeping(philo);
	// thinking(philo);
}

void	*ft_philosopher(void *data)
{
	t_personal	*philo;
	bool		*alive;

	philo = (t_personal *)data;
	alive = &philo->info->alive;
	while(alive)
	{
		if (philo->name % 2 == 1)
			philo_odd(philo);
		else
			philo_even(philo);
		if (philo->info->num_must_eat == philo->cnt_eaten || alive)
			break;
		sleeping(philo);
		thinking(philo);
	}
	return (NULL);
}

void	check_philos(t_info *info)
{
	int i;

	i = 0;
	while (info->alive)
	{
		i = 0;
		while (++i <= info->num_of_philo && info->alive)
		{
			pthread_mutex_lock(info->mutex_for_check);
			if (get_mstime() - info->philosophers[i].time_last_eat 
				> info->time_die)
				info->alive = false;
			pthread_mutex_unlock(info->mutex_for_check);
		}
		if (info->alive == false)
			break;
	}
}

int		create_thread(t_info *info)
{
	int	i;

	i = 0;
	while (++i <= info->num_of_philo)
	{
		if (pthread_create(&(info->philosophers[i].tid),
			NULL, ft_philosopher, (void *)&info->philosophers[i]) != 0)
			return (ft_free(info, 31));
	}
	check_philos(info);
	i = 0;
	while (++i <= info->num_of_philo)
		pthread_join(info->philosophers[i].tid, NULL);
	return (1);
}

int		malloc_ptr(t_info *info)
{
	info->philosophers = (t_personal *)malloc(sizeof
							(t_personal) * (info->num_of_philo + 1));
	if (info->philosophers == NULL)
		return (ft_free(info, 1));
	info->forks = (pthread_mutex_t *)malloc(sizeof(
						pthread_mutex_t) * (info->num_of_philo + 1));
	if (info->forks == NULL)
		return (ft_free(info, 3));
	info->mutex_for_print = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
	if (info->mutex_for_print == NULL)
		return (ft_free(info, 7));
	info->mutex_for_check = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
	if (info->mutex_for_check == NULL)
		return (ft_free(info, 15));
	return (1);
}

int		make_philo_fork(t_info *info)
{
	int	i;
	int	status;

	i = 0;
	status = malloc_ptr(info);
	if (!status)
		return (status);
	pthread_mutex_init(info->mutex_for_print, NULL);
	pthread_mutex_init(info->mutex_for_print, NULL);
	while (i++ < info->num_of_philo)
	{
		pthread_mutex_init(&(info->forks[i]), NULL);
		info->philosophers[i].name = i;
		info->philosophers[i].cnt_eaten = 0;
		info->philosophers[i].info = info;
		info->philosophers[i].left_fork = &info->forks[i];
		info->philosophers[i].right_fork =
			&info->forks[(i + 1) % info->num_of_philo];
	}
	return (1);
}

int		main(int argc, char *argv[])
{
	t_info	*info;

	info = parsing(argc, argv);
	if (info == NULL)
		return (error_p("argument error"));
	if (!make_philo_fork(info))
		return (error_p("creating error"));
	if (!create_thread(info))
		return (0);
	ft_free(info, 15);
	return (0);
}

