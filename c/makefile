include radio.makefile


	
hackathon.o:
	g++ -c hackathon.cpp  -lsqlite3  -o hackathon.o
	
#sqlite3.o:
#	g++ -SQLITE_API -c sqlite3.c -o sqlite3.o
arduPi_pi2.o:
	g++ -c arduPi_pi2.cpp -o arduPi_pi2.o

SX1271_pi2.o:
	g++ -DRASPBERRY2 -c SX1272.cpp -o SX1272_pi2.o

hackathon:
	g++ -lrt -lpthread hackathon.o -lsqlite3 arduPi_pi2.o SX1272_pi2.o -o hackathon


sql:
	g++  -c  sqlite3.c -o sqlite3.o
	
clean:
	rm *.o hackathon
# 	g++ -lrt -lpthread hackathon.o arduPi_pi2.o SX1272_pi2.o -o hackathon
