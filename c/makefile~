include radio.makefile

hackathon:
	g++ -DRASPBERRY2 -c SX1272.cpp -o SX1272_pi2.o
	g++ -c arduPi_pi2.cpp -o arduPi_pi2.o
	g++ -c hackathon.cpp -o hackathon.o
	g++ -lrt -lpthread hackathon.o arduPi_pi2.o SX1272_pi2.o -o hackathon

lib: arduPi.o arduPi_pi2.o SX1272.o SX1272_pi2.o SX1272_wnetkey.o SX1272_pi2_wnetkey.o lora_gateway.o lora_las_gateway.o lora_gateway_pi2.o lora_las_gateway_pi2.o

clean:
	-rm *.o
