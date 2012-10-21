CC = g++
SOURCES = main.cpp util.cpp sdl.cpp html.cpp
OUTPUT = -o cv

gradproject:
	$(CC) $(SOURCES) $(OUTPUT) `sdl-config --cflags --libs`

clean:
	rm -f cv
