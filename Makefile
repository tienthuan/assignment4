makemain: makeregions main.c regions.o
	clang -Wall -o main.c regions.o
makeregions: regions.c regions.h region_struct.h
	clang -Wall -c regions.c regions.o
	