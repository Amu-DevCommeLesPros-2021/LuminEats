.DEFAULT_GOAL := check

.PHONY: clean

# Efface tout les fichiers temporaires et les produits finaux.
clean:
	rm -rf build

# Crée le répertoire qui acceuille les fichiers temporaires et les produits finaux.
build:
	mkdir -p build

build/libvector.a: lib/vector/vector_api.c lib/vector/vector_api.h lib/vector/vector_types.h lib/vector/vector.h | build
	gcc -Wall -Wextra -Werror --debug -I lib/vector -c lib/vector/vector_api.c -o build/vector_api.o
	ar crs build/libvector.a build/vector_api.o

build/libalgorithm.a: lib/algorithm/algorithm.c lib/algorithm/algorithm.h | build
	gcc -Wall -Wextra -Werror --debug -I lib/vector -c lib/algorithm/algorithm.c -o build/algorithm.o
	ar crs build/libalgorithm.a build/algorithm.o

build/test: build/libalgorithm.a build/libvector.a test/main.c | build
	gcc -Wall -Wextra -Werror --debug test/main.c -I lib -L build -l algorithm -l vector -o build/test

build/lumineats: build/libalgorithm.a build/libvector.a bin/main.c | build
	gcc -Wall -Wextra -Werror --debug bin/main.c -I lib -L build -l algorithm -l vector -o build/lumineats

# S'assure de l'existence tout les programmes finaux (application, test, etc.)
# Par exemple : all: build/test build/appli
all: build/lumineats build/test

# Lance le programme de test.
check: build/test
	./build/test
