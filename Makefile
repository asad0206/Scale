# all:
# 	g++ -I src/include -L src/lib -o main main.cpp -lmingw32 -lSDL2main -lSDL2

a: main.cpp
	g++ -I src/include -L src/lib -o main main.cpp -lmingw32 -lSDL2main -lSDL2

b: file2.cpp
	g++ -I src/include -L src/lib -o file2 file2.cpp -lmingw32 -lSDL2main -lSDL2

c: file3.cpp
	g++ -I src/include -L src/lib -o file3 file3.cpp -lmingw32 -lSDL2main -lSDL2