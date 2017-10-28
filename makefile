#
#                                                              
# makefile for filesafe
#
#                                                              *

PROGRAM=encrypt
DEBUG=

LDFLAGS = -L/usr/local/opt/openssl/lib -lssl -lcrypto -Wall  
CPPFLAGS= -I/usr/local/opt/openssl/include  -std=c++0x -pthread \


$(PROGRAM) : version decrypt filesafe.o 
			 c++  main_encrypt.cpp filesafe.o -o $(PROGRAM)  $(DEBUG)   $(LDFLAGS)  $(CPPFLAGS)

# decrypt
decrypt : main_decrypt.cpp filesafe.o 
	c++  main_decrypt.cpp filesafe.o  -o decrypt  $(DEBUG)    $(LDFLAGS)  $(CPPFLAGS)


# filesafe.o
filesafe.o : filesafe.cpp filesafe.h  
	c++ -c filesafe.cpp   -o filesafe.o  $(DEBUG)    $(LDFLAGS)  $(CPPFLAGS)

   
version: 
	@echo "V.1.0\n"	 
	

clean: 
	rm -f *.o	
	rm -f encrypt
	rm -f decrypt
	 
	