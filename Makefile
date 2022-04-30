CC=gcc
CFLAGS = -std=c11 -Wall -Wextra -Werror -Wno-implicit-fallthrough
LFLAGS = -lm

NAME = example.out

SRC =  main.c noiceMaps.c noiceFilters.c noiceMixers.c noiceGenerators.c
DEPS = noiceMaps.h
# binary files
OBJS = main.o noiceMaps.o noiceFilters.o noiceMixers.o noiceGenerators.o
# modules
YOU: $(SRC) $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(LFLAGS) $(OBJS) -o $(NAME) 
%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(NAME)
