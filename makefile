CC ?= gcc
CFLAGS := $(CFLAGS) -pedantic -Wall -Wextra -Werror -Wno-pointer-arith --debug

AR ?= ar
ARFLAGS = crs

UNAME := $(shell uname)
ifeq ($(UNAME),Linux)
	CFLAGS += -std=gnu17 -D _GNU_SOURCE
endif

.DEFAULT_GOAL := check

.PHONY: clean prep-test-db

# Efface tout les fichiers temporaires et les produits finaux.
clean:
	rm -rf build

# Crée le répertoire qui acceuille les fichiers temporaires et les produits finaux.
build:
	mkdir -p build

build/libvector.a: lib/vector/vector_api.c lib/vector/vector_api.h lib/vector/vector_types.h lib/vector/vector.h | build
	$(CC) $(CFLAGS) -I lib -c lib/vector/vector_api.c -o build/vector_api.o
	$(AR) $(ARFLAGS) build/libvector.a build/vector_api.o

build/libalgorithm.a: lib/algorithm/algorithm.c lib/algorithm/algorithm.h lib/vector/vector_api.h lib/vector/vector_types.h | build
	$(CC) $(CFLAGS) -I lib -c lib/algorithm/algorithm.c -o build/algorithm.o
	$(AR) $(ARFLAGS) build/libalgorithm.a build/algorithm.o

build/libdb.a: lib/db/db.c lib/db/db.h lib/vector/vector_api.h lib/vector/vector_types.h | build
	$(CC) $(CFLAGS) -I lib -c lib/db/db.c -o build/db.o
	$(AR) $(ARFLAGS) build/libdb.a build/db.o

build/liblogger.a: lib/logger/logger.c lib/logger/logger.h | build
	$(CC) $(CFLAGS) --debug -I lib -c lib/logger/logger.c -o build/logger.o
	$(AR) $(ARFLAGS) build/liblogger.a build/logger.o

build/liblumineats.a: lib/lumineats/lumineats.c lib/lumineats/lumineats.h lib/lumineats/predicates.c lib/lumineats/predicates.h lib/algorithm/algorithm.h lib/logger/logger.h lib/vector/vector_api.h lib/vector/vector_types.h | build
	$(CC) $(CFLAGS) -I lib -c lib/lumineats/lumineats.c -o build/lumineats.o
	$(CC) $(CFLAGS) -I lib -c lib/lumineats/predicates.c -o build/predicates.o
	$(AR) $(ARFLAGS) build/liblumineats.a build/lumineats.o build/predicates.o


build/test: build/libalgorithm.a build/libdb.a build/liblogger.a build/liblumineats.a build/libvector.a test/main.c | build
	$(CC) $(CFLAGS) test/main.c -I lib -L build -l lumineats -l algorithm -l db -l logger -l vector -o build/test

build/lumineats: build/libalgorithm.a build/libdb.a build/liblogger.a build/liblumineats.a build/libvector.a bin/ecrans.c bin/ecrans.h bin/main.c | build
	echo $(UNAME)
	echo $(CFLAGS)
	$(CC) $(CFLAGS) bin/ecrans.c bin/main.c -I bin -I lib -L build -l lumineats -l algorithm -l db -l logger -l vector -o build/lumineats

# S'assure de l'existence tout les programmes finaux (application, test, etc.)
# Par exemple : all: build/test build/appli
all: build/lumineats build/test

# Lance le programme de test.
check: build/test
	./build/test
