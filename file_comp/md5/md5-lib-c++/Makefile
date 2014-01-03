md5lib.a: md5c.o
	ar -r libmd5++.a md5c.o

md5c.o	: md5c.cpp md5.h global.h
	g++ -O2 -c md5c.cpp

