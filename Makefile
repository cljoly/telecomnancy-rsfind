rsfind: rsfind.c
	gcc -Wall -Werror -Wextra -o rsfind rsfind.c

test: rsfind test.sh
	bash test.sh
