CC := gcc
CFLAGS := `sdl2-config --libs --cflags` -ggdb3 -Wall -lm -Werror
BUILD := ./build

teclas:build
	$(CC) -o $(BUILD)/$@ main.c $(HDRS) $(CFLAGS) 

build:
	if [ ! -d build ]; then mkdir build; fi

clean:
	rm $(BUILD)/*

