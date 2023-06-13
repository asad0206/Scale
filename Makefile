# all:
# 	g++ -I src/include -L src/lib -o main main.cpp -lmingw32 -lSDL2main -lSDL2

a: main.cpp
	g++ -I src/include -L src/lib -o main main.cpp -lmingw32 -lSDL2main -lSDL2

b: file2.cpp
	g++ -I src/include -L src/lib -o file2 file2.cpp -lmingw32 -lSDL2main -lSDL2

c: file3.cpp
	g++ -I src/include -L src/lib -m64 -o file3 file3.cpp -lmingw32 -lSDL2main -lSDL2

d: file4.cpp
	g++ -I src/include -L src/lib -Wall -m64 -o file4 file4.cpp -lmingw32 -lSDL2main -lSDL2

e: file5.cpp
	g++ -I src/include -L src/lib -Wall -m64 -o file5 file5.cpp -lmingw32 -lSDL2main -lSDL2

f: file6.cpp
	g++ -I src/include -L src/lib -Wall -m64 -o file6 file6.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image