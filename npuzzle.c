#include "npuzzle.h"

void					memory_allocation_error(void)
{
	printf("Memory allocation failed.\n");
	exit(0);
}

void					print_grid(unsigned int **grid, unsigned int size)
{
	printf("Size of the Puzzle: %d\n", size);
	printf("Address:            %p\n", grid);
	for (unsigned int i = 0; i < size; i++)
	{
		for (unsigned int j = 0; j < size; j++)
			printf("%3d ", grid[i][j]);
		printf("\n");
	}
}

void					create_goal(t_puzzle *puzzle, unsigned int size)
{
	unsigned int	**goal = NULL;
	unsigned int	x = 0;
	unsigned int	y = 0;
	char					dir = 0;
	char					moved = 0;

	if (!(goal = calloc(size, sizeof(unsigned int *))))
		memory_allocation_error();
	for (unsigned int i = 0; i < size; i++)
		if (!(goal[i] = calloc(size, sizeof(unsigned int))))
			memory_allocation_error();
	for (unsigned int i = 1; i < size * size; i++)
	{
		moved = 0;
		goal[y][x] = i;
		if (dir % 4 == 0)
		{
			if (x + 1 == size || goal[y][x + 1])
				dir++;
			else if (++x)
				moved++;
		}
		if (!moved && dir % 4 == 1)
		{
			if (y + 1 == size || goal[y + 1][x])
				dir++;
			else if (++y)
				moved++;
		}
		if (!moved && dir % 4 == 2)
		{
			if (x == 0 || goal[y][x - 1])
				dir++;
			else if (--x || 1)
				moved++;
		}
		if (!moved && dir % 4 == 3)
		{
			if (y == 0 || goal[y - 1][x])
				dir++;
			else if (--y || 1)
				moved++;
		}
		if (!moved)
			x++;
	}
	puzzle->goal = goal;
}
