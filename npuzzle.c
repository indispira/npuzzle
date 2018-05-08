#include "npuzzle.h"

void					memory_allocation_error(void)
{printf("%s\n", __FUNCTION__);
	printf("Memory allocation failed.\n");
	exit(0);
}

void					free_node(t_node *node)
{//printf("%s\n", __FUNCTION__);
	for (unsigned int i = 0; node->grid[i]; i++)
		free(node->grid[i]);
	free(node);
	node = NULL;
}

void					print_grid(unsigned int **grid, unsigned int size)
{//printf("%s\n", __FUNCTION__);
	printf("Size of the Puzzle: %d\n", size);
	for (unsigned int i = 0; i < size; i++)
	{
		for (unsigned int j = 0; j < size; j++)
			printf("%3d ", grid[i][j]);
		printf("\n");
	}
}

unsigned int	generate_hash(unsigned int **grid, unsigned int size)
{//printf("%s\n", __FUNCTION__);
	unsigned int	hash = 0;

	for (unsigned int i = 0; i < size; i++)
		for (unsigned int j = 0; j < size; j++)
			hash = hash * PRIME + grid[i][j];
	return (hash);
}

void					create_goal(t_puzzle *puzzle, unsigned int size)
{//printf("%s\n", __FUNCTION__);
	char					dir = 0;
	char					moved = 0;
	unsigned int	**goal = NULL;
	unsigned int	x = 0;
	unsigned int	y = 0;
	t_node				*node = NULL;

	if (!(goal = calloc(size + 1, sizeof(unsigned int *))))
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
	if (!(node = calloc(1, sizeof(t_node))))
		memory_allocation_error();
	node->grid = goal;
	node->hash = generate_hash(goal, size);
	puzzle->goal = node;
}

void					search_final_coords(unsigned int n, unsigned int **grid, unsigned int size, int *x, int *y)
{//printf("%s\n", __FUNCTION__);
	for (unsigned int i = 0; i < size; i++)
		for (unsigned int j = 0; j < size; j++)
		{
			if (grid[i][j] == n)
			{
				*x = j;
				*y = i;
				return ;
			}
		}
}

unsigned int	heuristic_manhattan_distance(unsigned int **grid, unsigned int **goal, unsigned int size)
{//printf("%s\n", __FUNCTION__);
	unsigned int	dist = 0;
	int						x;
	int						y;

	for (int i = 0; i < (int)size; i++)
		for (int j = 0; j < (int)size; j++)
		{
			if (grid[i][j] != 0)
			{
				search_final_coords(grid[i][j], goal, size, &x, &y);
				dist += ABS(j - x) + ABS(i - y);
			}
		}
	return (dist);
}

void					add_open_node_sorted(t_puzzle *puzzle, t_node *node)
{//printf("%s\n", __FUNCTION__);
	t_node	*old = puzzle->open;
	t_node	*cur = puzzle->open;

	if (!cur)
		puzzle->open = node;
	else
	{
		while (cur && node->f >= cur->f)
		{
			old = cur;
			cur = cur->next;
		}
		if (cur == puzzle->open)
		{
			node->next = puzzle->open;
			puzzle->open = node;
		}
		else
		{
			node->next = old->next;
			old->next = node;
		}
	}
	// t_node *tmp = puzzle->open;
	// while (tmp)
	// {
	// 	print_grid(tmp->grid, puzzle->size);
	// 	printf("F %u\n", tmp->f);
	// 	tmp = tmp->next;
	// }
	// print_grid(node->grid, puzzle->size);
	// printf("F %u\n", node->f);
	// printf("Added node with %u\n", node->f);
}

void					transfer_open_node_to_closed(t_puzzle *puzzle)
{//printf("%s\n", __FUNCTION__);
	t_node	*ptr = NULL;

	ptr = puzzle->open;
	puzzle->open = ptr->next;
	ptr->next = puzzle->closed;
	puzzle->closed = ptr;
}

t_node				*check_if_in_list(t_node *list, unsigned int hash)
{//printf("%s\n", __FUNCTION__);
	while (list)
	{
		if (hash == list->hash)
			break ;
		list = list->next;
	}
	return (list);
}

void					delete_open_node(t_puzzle *puzzle, unsigned int hash)
{//printf("%s\n", __FUNCTION__);
	t_node	*old = NULL;
	t_node	*cur = puzzle->open;

	while (cur)
	{
		if (cur->hash == hash)
		{
			if (old)
				old->next = cur->next;
			else
				puzzle->open = NULL;
			free_node(cur);
			return ;
		}
		old = cur;
		cur = cur->next;
	}
}

void					compare_nodes(t_puzzle *puzzle, t_node *old, t_node *new)
{//printf("%s\n", __FUNCTION__);
	if (new->f < old->f)
	{
		delete_open_node(puzzle, old->hash);
		add_open_node_sorted(puzzle, new);
	}
	else
		free_node(new);
}

void					search_empty_case(unsigned int **grid, unsigned int *x, unsigned int *y, unsigned int size)
{//printf("%s\n", __FUNCTION__);
	unsigned int	i = 0;
	unsigned int	j = 0;

	for (j = 0; grid[j]; j++)
		for (i = 0; i < size; i++)
			if (grid[j][i] == 0)
			{
				*x = i;
				*y = j;
				return ;
			}
}

unsigned int	**copy_grid(unsigned int **grid, unsigned int size)
{//printf("%s\n", __FUNCTION__);
	unsigned int	**new = NULL;

	if (!(new = calloc(size + 1, sizeof(unsigned int *))))
		memory_allocation_error();
	for (unsigned int i = 0; grid[i]; i++)
	{
		if (!(new[i] = calloc(size, sizeof(unsigned int))))
			memory_allocation_error();
		memcpy(new[i], grid[i], size * sizeof(unsigned int));
	}
	return (new);
}

t_node				*create_node(t_puzzle *puzzle, unsigned int **grid)
{//printf("%s\n", __FUNCTION__);
	t_node	*node = NULL;

	if (!(node = calloc(1, sizeof(t_node))))
		memory_allocation_error();
	node->g = 0;
	node->h = heuristic_manhattan_distance(grid, puzzle->goal->grid, puzzle->size);
	node->f = node->g + node->h;
	node->hash = generate_hash(grid, puzzle->size);
	node->grid = grid;
	node->next = NULL;
	return (node);
}

void					compute_node(t_puzzle *puzzle, t_node *node)
{//printf("%s\n", __FUNCTION__);
	t_node	*check = NULL;

	if (check_if_in_list(puzzle->closed, node->hash))
	{
		free_node(node);
		return ;
	}
	if ((check = check_if_in_list(puzzle->open, node->hash)))
		compare_nodes(puzzle, check, node);
	else
		add_open_node_sorted(puzzle, node);
}

void					create_neighbors(t_puzzle	*puzzle, t_node *node)
{//printf("%s\n", __FUNCTION__);
	unsigned int	**grid = node->grid;
	unsigned int	**new_grid = NULL;
	unsigned int	size = puzzle->size;
	unsigned int	x = 0;
	unsigned int	y = 0;
	unsigned int	tmp;
	t_node				*new_node = NULL;

	search_empty_case(grid, &x, &y, size);
	// printf("Empty %u %u\n", x, y);
	if (x > 0)
	{
		// printf("LEFT\n");
		new_grid = copy_grid(grid, size);
		tmp = new_grid[y][x];
		new_grid[y][x] = new_grid[y][x - 1];
		new_grid[y][x - 1] = tmp;
		new_node = create_node(puzzle, new_grid);
		compute_node(puzzle, new_node);
	}
	if (y > 0)
	{
		// printf("UP\n");
		new_grid = copy_grid(grid, size);
		tmp = new_grid[y][x];
		new_grid[y][x] = new_grid[y - 1][x];
		new_grid[y - 1][x] = tmp;
		new_node = create_node(puzzle, new_grid);
		compute_node(puzzle, new_node);
	}
	if (x < size - 1)
	{
		// printf("RIGHT\n");
		new_grid = copy_grid(grid, size);
		tmp = new_grid[y][x];
		new_grid[y][x] = new_grid[y][x + 1];
		new_grid[y][x + 1] = tmp;
		new_node = create_node(puzzle, new_grid);
		compute_node(puzzle, new_node);
	}
	if (y < size - 1)
	{
		// printf("DOWN\n");
		new_grid = copy_grid(grid, size);
		tmp = new_grid[y][x];
		new_grid[y][x] = new_grid[y + 1][x];
		new_grid[y + 1][x] = tmp;
		new_node = create_node(puzzle, new_grid);
		compute_node(puzzle, new_node);
	}
}

unsigned int	astar(t_puzzle *puzzle)
{//printf("%s\n", __FUNCTION__);
	t_node	*node;
	unsigned int steps = 0;

	add_open_node_sorted(puzzle, create_node(puzzle, puzzle->start));
	while ((node = puzzle->open))
	{
		printf("Step %02u %u\t\t\t%u\n", steps, node->hash, node->f);
		// print_grid(node->grid, puzzle->size);
		if (steps > 15000)
			return (0);
		steps++;
		if (node->hash == puzzle->goal->hash)
			return (1);
		transfer_open_node_to_closed(puzzle);
		create_neighbors(puzzle, node);
	}
	return (0);
}

// int main(void)
// {
// 	t_puzzle puzzle;

// 	puzzle.size = 3;
// 	puzzle.start = calloc(puzzle.size + 1, sizeof(unsigned int *));
// 	for (unsigned int i = 0; i < puzzle.size; i++)
// 	{
// 		puzzle.start[i] = calloc(puzzle.size, sizeof(unsigned int));
// 		if (i == 0)
// 		{
// 			// puzzle.start[i][0] = 1;
// 			// puzzle.start[i][1] = 0;
// 			puzzle.start[i][0] = 1;
// 			puzzle.start[i][1] = 6;
// 			puzzle.start[i][2] = 0;
// 			// puzzle.start[i][0] = 15;
// 			// puzzle.start[i][1] = 11;
// 			// puzzle.start[i][2] = 5;
// 			// puzzle.start[i][3] = 0;
// 		}
// 		else if (i == 1)
// 		{
// 			// puzzle.start[i][0] = 3;
// 			// puzzle.start[i][1] = 2;
// 			puzzle.start[i][0] = 3;
// 			puzzle.start[i][1] = 7;
// 			puzzle.start[i][2] = 4;
// 			// puzzle.start[i][0] = 7;
// 			// puzzle.start[i][1] = 4;
// 			// puzzle.start[i][2] = 2;
// 			// puzzle.start[i][3] = 6;
// 		}
// 		else if (i == 2)
// 		{
// 			puzzle.start[i][0] = 8;
// 			puzzle.start[i][1] = 5;
// 			puzzle.start[i][2] = 2;
// 			// puzzle.start[i][0] = 13;
// 			// puzzle.start[i][1] = 12;
// 			// puzzle.start[i][2] = 14;
// 			// puzzle.start[i][3] = 3;
// 		}
// 		else if (i == 3)
// 		{
// 			puzzle.start[i][0] = 10;
// 			puzzle.start[i][1] = 8;
// 			puzzle.start[i][2] = 1;
// 			puzzle.start[i][3] = 9;
// 		}
// 	}
// 	create_goal(&puzzle, puzzle.size);
// 	print_grid(puzzle.start, puzzle.size);
// 	if (astar(&puzzle))
// 		printf("Success!!!\n");
// 	else
// 		printf("Unsolvable...\n");
// 	return (0);
// }




