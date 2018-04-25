#include "npuzzle.h"

void print_puzzle(t_puzzle *puzzle)
{
	printf("Size of the Puzzle: %d\n", puzzle->size);
	printf("Address:            %p\n", puzzle->cells);
	for (unsigned int i = 0; i < puzzle->size; i++)
	{
		for (unsigned int j = 0; j < puzzle->size; j++)
			printf("%3d ", puzzle->cells[i][j]);
		printf("\n");
	}
}
