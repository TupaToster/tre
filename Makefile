CC=g++

CFLAGS+=-Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op -Wmissing-field-initializers -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -D_DEBUG -D_EJUDGE_CLIENT_SIDE

OBJ+=main.o struct/struct.o lib/flog.o lib/text.o akinator/akinator.o

HEADS=protos.h lib/flog.h struct/struct.h akinator/akinator.h

all: a.exe

a.exe: $(OBJ) $(HEADS)
	$(CC) $(OBJ) $(CFLAGS) -o a.exe

*.o: *.cpp
	$(CC) $(CFLAGS) *.cpp -c

clean:
	rm -rf *.o *.exe*
	rm -rf struct/*.o struct/*.exe*
	rm -rf akinator/*.o akinator/*.exe*
	rm -rf lib/*.o lib/*.exe*
	rm -rf *.png *.dot
	clear