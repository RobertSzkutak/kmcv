CC = g++
SOURCES = main.cpp util.cpp sdl.cpp html.cpp
OUTPUT = -o kmcv

kmcv:
	$(CC) $(SOURCES) $(OUTPUT) `sdl-config --cflags --libs`

clean:
	rm -f kmcv
