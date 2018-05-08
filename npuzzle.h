#ifndef __NPUZZLE_H__
# define __NPUZZLE_H__

# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# define PRIME		257

# define ABS(x) 	(((x) < 0) ? -(x) : (x))

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
		t_node				*goal;
		t_node				*open;
		t_node				*closed;

	}								t_puzzle;

	void						print_grid(unsigned int **grid, unsigned int size);
	void						create_goal(t_puzzle *puzzle, unsigned int size);
	unsigned int		generate_hash(unsigned int **grid, unsigned int size);
	unsigned int		heuristic_manhattan_distance(unsigned int **grid, unsigned int **goal, unsigned int size);
	unsigned int		astar(t_puzzle *puzzle);

#endif