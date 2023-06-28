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

g: file7.cpp
	g++ -I src/include -L src/lib -Wall -m64 -o file7 file7.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image

h: file8.cpp
	g++ -I src/include -L src/lib -Wall -m64 -o file8 file8.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image

i: file9.cpp
	g++ -I src/include -L src/lib -Wall -m64 -o file9 file9.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image

j: file10.cpp
	g++ -I src/include -L src/lib -Wall -m64 -o file10 file10.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image

k: file11.cpp
	g++ -I src/include -L src/lib -Wall -m64 -o file11 file11.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image

l: file12.cpp
	g++ -I src/include -L src/lib -Wall -m64 -o file12 file12.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image

m: file13.cpp
	g++ -I src/include -L src/lib -Wall -m64 -o file13 file13.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image

n: file14.cpp
	g++ -I src/include -L src/lib -Wall -m64 -o file14 file14.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image

o: file15.cpp
	g++ -I src/include -L src/lib -Wall -m64 -o file15 file15.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image

p: file16.cpp
	g++ -I src/include -L src/lib -Wall -m64 -o file16 file16.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf

q: file17.cpp
	g++ -I src/include -L src/lib -Wall -m64 -o file17 file17.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf

r: file18.cpp
	g++ -I src/include -L src/lib -Wall -m64 -o file18 file18.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image