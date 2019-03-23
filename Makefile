client:
	make -f makeclient

broker:
	make -f makebroker
 
key:
	make -f makeKey

ALL:
	make -f makeclient
	make -f makebroker
	make -f makeKey
	
clean:
	rm -rf *.o *~ Broker Client keymanage
