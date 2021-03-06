/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dim <dim@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/04 16:39:23 by dim               #+#    #+#             */
/*   Updated: 2022/02/17 17:22:34 by dim              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H
# include <stdlib.h>
# include <pthread.h>
# include <stdbool.h>

typedef struct s_info
{
	bool				alive;
	int					num_of_philo;
	int					time_die;
	int					time_eat;
	int					time_sleep;
	int					num_must_eat;
	int					done_philo;
	struct s_personal	*philosophers;
	pthread_mutex_t		*forks;
	pthread_mutex_t		mutex_for_print;
	pthread_mutex_t		mutex_for_check;
	long				start_time;
}	t_info;

typedef struct s_personal
{
	int				name;
	int				cnt_eaten;
	long			time_last_eat;
	t_info			*info;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_t		tid;
}	t_personal;

int		ft_free(t_info *info, char flag);
int		malloc_ptr(t_info *info);
void	init_mutex(t_info *info);
int		make_philo_fork(t_info *info);

void	print_state(t_personal *philo, char *msg);
long	get_mstime(void);
void	ft_mssleep(long usleep_time);
int		error_p(char *msg);
int		ft_atoi(char *str);

void	sleeping(t_personal *philo);
void	thinking(t_personal *philo);

void	check_philos(t_info *info);
void	*ft_philosopher(void *data);
int		create_thread(t_info *info);

t_info	*parsing(int argc, char **argv);

#endif