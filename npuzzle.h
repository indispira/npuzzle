#ifndef __NPUZZLE_H__
# define __NPUZZLE_H__

# include <stdio.h>
# include <stdlib.h>

	typedef struct	s_node
	{
		unsigned int	g;
		unsigned int	h;
		unsigned int	f;
		unsigned int	hash;
		unsigned int	**grid;
		struct s_node	*next;
	}								t_node;

	typedef struct	s_puzzle
	{
		unsigned int	size;
		unsigned int	**start;
		unsigned int	**goal;
		t_node				*open;
		t_node				*closed;

	}								t_puzzle;

	void						print_grid(unsigned int **grid, unsigned int size);
	void						create_goal(t_puzzle *puzzle, unsigned int size);

#endif