include radio.makefile

hackathon:
	g++ -lrt -lpthread hackathon.o sqlite3.o arduPi_pi2.o SX1272_pi2.o -o hackathon
	
hackathon.o:
	g++ -c hackathon.cpp -SQLITE_API -o sqlite3.o  -o hackathon.o
	
sqlite3.o:
	g++ -SQLITE_API -c sqlite3.c -o sqlite3.o
arduPi_pi2.o:
	g++ -c arduPi_pi2.cpp -o arduPi_pi2.o

SX1271_pi2.o:
	g++ -DRASPBERRY2 -c SX1272.cpp -o SX1272_pi2.o
# 	g++ -lrt -lpthread hackathon.o arduPi_pi2.o SX1272_pi2.o -o hackathon

clean:
	rm *.o hackathon
