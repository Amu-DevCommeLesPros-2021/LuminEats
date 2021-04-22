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

build/libvector.a: lib/vector/api.c lib/vector/api.h lib/vector/types.h lib/vector/vector.h | build
	$(CC) $(CFLAGS) -I lib -c lib/vector/api.c -o build/api.o
	$(AR) $(ARFLAGS) build/libvector.a build/api.o

build/libalgorithm.a: lib/algorithm/algorithm.c lib/algorithm/algorithm.h lib/vector/api.h lib/vector/types.h | build
	$(CC) $(CFLAGS) -I lib -c lib/algorithm/algorithm.c -o build/algorithm.o
	$(AR) $(ARFLAGS) build/libalgorithm.a build/algorithm.o

build/libdb.a: \
 lib/db/api.c lib/db/api.h \
 lib/db/tables.c lib/db/tables.h \
 lib/db/db.h lib/vector/types.h | build
	$(CC) $(CFLAGS) -I lib -c lib/db/api.c -o build/api.o
	$(CC) $(CFLAGS) -I lib -c lib/db/tables.c -o build/tables.o
	$(AR) $(ARFLAGS) build/libdb.a build/api.o build/tables.o

build/liblogger.a: lib/logger/logger.c lib/logger/logger.h | build
	$(CC) $(CFLAGS) --debug -I lib -c lib/logger/logger.c -o build/logger.o
	$(AR) $(ARFLAGS) build/liblogger.a build/logger.o

build/liblumineats.a: \
 lib/lumineats/accounts.c lib/lumineats/accounts.h \
 lib/lumineats/filter.c lib/lumineats/filter.h \
 lib/lumineats/lumineats.c lib/lumineats/lumineats.h \
 lib/lumineats/menu.c lib/lumineats/menu.h \
 lib/lumineats/predicates.c lib/lumineats/predicates.h \
 lib/lumineats/search.c lib/lumineats/search.h \
 lib/algorithm/algorithm.h lib/logger/logger.h lib/vector/api.h lib/vector/types.h | build
	$(CC) $(CFLAGS) -I lib -c lib/lumineats/accounts.c -o build/accounts.o
	$(CC) $(CFLAGS) -I lib -c lib/lumineats/filter.c -o build/filter.o
	$(CC) $(CFLAGS) -I lib -c lib/lumineats/lumineats.c -o build/lumineats.o
	$(CC) $(CFLAGS) -I lib -c lib/lumineats/menu.c -o build/menu.o
	$(CC) $(CFLAGS) -I lib -c lib/lumineats/predicates.c -o build/predicates.o
	$(CC) $(CFLAGS) -I lib -c lib/lumineats/search.c -o build/search.o
	$(AR) $(ARFLAGS) build/liblumineats.a build/accounts.o build/filter.o build/lumineats.o build/menu.o build/predicates.o build/search.o

# Programme de tests.
build/test: build/libalgorithm.a build/libdb.a build/liblogger.a build/liblumineats.a build/libvector.a test/main.c | build
	$(CC) $(CFLAGS) test/main.c -I lib -L build -l lumineats -l algorithm -l db -l logger -l vector -o build/test

# Application.
build/lumineats: build/libalgorithm.a build/libdb.a build/liblogger.a build/liblumineats.a build/libvector.a bin/ecrans.c bin/ecrans.h bin/main.c | build
	cp bin/*.csv build
	$(CC) $(CFLAGS) bin/ecrans.c bin/main.c -I bin -I lib -L build -l lumineats -l algorithm -l db -l logger -l vector -o build/lumineats

# S'assure de l'existence tout les programmes finaux (application, test, etc.)
# Par exemple : all: build/test build/appli
all: build/lumineats build/test

# Lance le programme de test.
check: build/test
	./build/test
