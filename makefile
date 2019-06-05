all:
	@echo "Compilando..."
	@gcc cenariomain.c soil/*.c -o cenariomain.bin -Wall -lglut -lGL -lGLU -lSDLmain -lSDL -lSDL_mixer

executa: all
	@echo "Executando..."
	@./cenariomain.bin

limpa:
	@echo "Limpando arquivos..."
	@rm *.o *~ *.bin -f
