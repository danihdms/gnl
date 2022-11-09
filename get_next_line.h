#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stdio.h>

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 10
#endif

typedef struct	s_list {
	char	*content;
	struct s_list	*next;
	size_t	len;
}		t_list;

size_t	ft_strlen(const char *s);
int	contains_new_line(char	*buffer);
void	lst_add_back(t_list **remainder, char *buffer);
char	*sub_str(char const *s, unsigned int start, size_t len);
void	separate_last(t_list **remainder, char *buffer);
void	fill_remainder(int fd, t_list **remainder, int *read_int);
char	*concatenate_all_strings(t_list **remainder);
void	clear_remainder(t_list **remainder);
char	*get_next_line(int fd);

#endif
