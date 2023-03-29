CC = clang
CFLAGS = -Wall -Werror -Wextra -Wpedantic $(shell pkg-config --cflags gmp) -gdwarf-4
LFLAGS = $(shell pkg-config --libs gmp)

all: keygen encrypt decrypt

# make keygen and pull any other files need for that file 
keygen: keygen.o ss.o randstate.o numtheory.o
	$(CC) -o $@ $^ $(LFLAGS)

# make encrypt and pull any other files need for that file 
encrypt: encrypt.o ss.o randstate.o numtheory.o
	$(CC) -o $@ $^ $(LFLAGS)

# make decrypt and pull any other files need for that file 
decrypt: decrypt.o ss.o randstate.o numtheory.o
	$(CC) -o $@ $^ $(LFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

# remove .o files
clean:
	rm -f keygen encrypt decrypt *.o

# clean the keys 
cleankeys:
	rm -f *.{pub,priv}
	
cleanpubpriv:
	rm -f ss.pub ss.priv

scan-build: clean
	scan-build --use-cc=$(CC) make
	
format:
	clang-format -i -style=file *.[ch]

