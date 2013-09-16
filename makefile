all: release
	@echo "Encontrará el programa en la ruta actual: ($PWD)/songoku"
install: release
	@mv songoku /usr/games
	@echo "El programa se ha instalado en /usr/games/songoku. A partir de ahora bastará con \"songoku\" para ejecutarlo."
debug: songoku.c dstr.h
	@gcc -g -Wall -o a.out songoku.c -p -D DEBUG
	@echo "Ha compilado usted una versión de depuración de nombre \"a.out\"."
release: songoku.c dstr.h
	@gcc -o songoku songoku.c -O3 -D RELEASE
	@echo "El programa se ha compilado y optimizado a nivel 3."
