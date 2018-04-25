#ifndef __NPUZZLE_H__
# define __NPUZZLE_H__

# include <stdio.h>

	typedef struct	s_puzzle
	{
		unsigned int	size;
		unsigned int	**cells;
	}								t_puzzle;

	void	print_puzzle(t_puzzle *puzzle);

#endif