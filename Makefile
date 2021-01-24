#Makefile projektu noiceMaps
CC=gcc
CFLAGS = -std=c11 -Wall -Wextra -Werror
LFLAGS = -lm

NAME = c

SRC =  main.c noiceMaps.c noiceFilters.c noiceMixers.c noiceGenerators.c
DEPS = noiceMaps.h
# nazwy wszystkich plikow zrodlowych
OBJS = main.o noiceMaps.o noiceFilters.o noiceMixers.o noiceGenerators.o
# nazwy wszystkich modulow
YOU: $(SRC) $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(LFLAGS) $(OBJS) -o $(NAME) 
%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(NAME)
