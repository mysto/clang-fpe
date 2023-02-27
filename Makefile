CFLAGS := $(shell printenv CFLAGS) -O2 -Wall -fPIC -Wno-deprecated-declarations
SO_LINKS = $(shell printenv LDFLAGS) -lm -lcrypto

UNAME := $(shell uname -s)
ifeq ($(UNAME),Darwin)
LIB = libfpe.a libfpe.dylib
else
LIB = libfpe.a libfpe.so
endif

EXAMPLE_SRC = example.c
EXAMPLE_EXE = example
OBJS = src/ff1.o src/ff3.o src/fpe_locl.o


all: $(LIB) $(EXAMPLE_EXE)

libfpe.a: $(OBJS)
	ar rcs $@ $(OBJS)

ifeq ($(UNAME),Darwin)
libfpe.dylib: $(OBJS)
	gcc -shared -fPIC -Wl,-install_name,libfpe.dylib $(OBJS) $(SO_LINKS) -o $@
else
libfpe.so: $(OBJS)
	gcc -shared -fPIC -Wl,-soname,libfpe.so $(OBJS) $(SO_LINKS) -o $@
endif

.PHONY = all clean install

src/ff1.o: src/ff1.c
	gcc $(CFLAGS) -c src/ff1.c -o $@

src/ff3.o: src/ff3.c
	gcc $(CFLAGS) -c src/ff3.c -o $@

src/fpe_locl.o: src/fpe_locl.c
	gcc $(CFLAGS) -c src/fpe_locl.c -o $@

$(EXAMPLE_EXE): $(EXAMPLE_SRC) $(LIB)
ifeq ($(UNAME),Darwin)
	gcc $(CFLAGS) -Wl, $(EXAMPLE_SRC) -L. -lfpe $(SO_LINKS) -Isrc -o $@
else
	gcc $(CFLAGS) -Wl,-rpath=\$$ORIGIN $(EXAMPLE_SRC) -L. -lfpe $(SO_LINKS) -Isrc -o $@
endif

test:
	python test2.py

clean:
	rm $(OBJS) $(EXAMPLE_EXE) $(LIB)

install:
	cp libfpe.so /usr/local/lib
	cp src/*.h /usr/local/include

