.DEFAULT_GOAL := check

.PHONY: clean prep-test-db

# Efface tout les fichiers temporaires et les produits finaux.
clean:
	rm -rf build

# Crée le répertoire qui acceuille les fichiers temporaires et les produits finaux.
build:
	mkdir -p build

# Removes old testing folders and copies the standrd set of tables to it.
prep-test-db: | build
	rm -rf build/test-db
	cp -a test/db/. build/test-db

build/libvector.a: lib/vector/vector_api.c lib/vector/vector_api.h lib/vector/vector_types.h lib/vector/vector.h | build
	gcc -Wall -Wextra -Werror --debug -I lib -c lib/vector/vector_api.c -o build/vector_api.o
	ar crs build/libvector.a build/vector_api.o

build/libalgorithm.a: lib/algorithm/algorithm.c lib/algorithm/algorithm.h lib/vector/vector_api.h lib/vector/vector_types.h | build
	gcc -Wall -Wextra -Werror --debug -I lib -c lib/algorithm/algorithm.c -o build/algorithm.o
	ar crs build/libalgorithm.a build/algorithm.o

build/libdb.a: lib/db/db.c lib/db/db.h lib/vector/vector_api.h lib/vector/vector_types.h | build
	gcc -Wall -Wextra -Werror --debug -I lib -c lib/db/db.c -o build/db.o
	ar crs build/libdb.a build/db.o

build/liblogger.a: lib/logger/logger.c lib/logger/logger.h | build
	gcc -Wall -Wextra -Werror --debug -I lib -c lib/logger/logger.c -o build/logger.o
	ar crs build/liblogger.a build/logger.o

build/liblumineats.a: lib/lumineats/lumineats.c lib/lumineats/lumineats.h lib/algorithm/algorithm.h lib/logger/logger.h lib/vector/vector_api.h lib/vector/vector_types.h | build
	gcc -Wall -Wextra -Werror --debug -I lib -c lib/lumineats/lumineats.c -o build/lumineats.o
	ar crs build/liblumineats.a build/lumineats.o

build/test: build/libalgorithm.a build/libdb.a build/liblogger.a build/liblumineats.a build/libvector.a test/main.c | build
	gcc -Wall -Wextra -Werror --debug test/main.c -I lib -L build -l lumineats -l algorithm -l db -l logger -l vector -o build/test

build/lumineats: build/libalgorithm.a build/libdb.a build/liblogger.a build/liblumineats.a build/libvector.a bin/ecrans.c bin/ecrans.h bin/main.c | build
	gcc -Wall -Wextra -Werror --debug bin/ecrans.c bin/main.c -I lib -L build -l lumineats -l algorithm -l db -l logger -l vector -o build/lumineats

# S'assure de l'existence tout les programmes finaux (application, test, etc.)
# Par exemple : all: build/test build/appli
all: build/lumineats build/test prep-test-db

# Lance le programme de test.
check: build/test prep-test-db
	./build/test
