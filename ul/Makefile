make: ccs811.c si7021.c i2c.c mq7.c led.c sensors.c
	arm-linux-gnueabihf-gcc -s -static ccs811.c -o ccs811
	arm-linux-gnueabihf-gcc -s -static si7021.c -o si7021
	arm-linux-gnueabihf-gcc -s -static i2c.c -o i2c
	arm-linux-gnueabihf-gcc -s -static mq7.c -o mq7
	arm-linux-gnueabihf-gcc -s -static led.c -o led
	arm-linux-gnueabihf-gcc -s -static sensors.c -o sensors
	
clean:
	rm ccs811 si7021 i2c mq7 led sensors
