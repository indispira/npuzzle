clear
# make re
# python npuzzle-gen.py -s 5 > puzzle4
# python3 npuzzle.py --file puzzle4
gcc -Wall -Wextra -Werror -g -fsanitize=address *.c && ./a.out
