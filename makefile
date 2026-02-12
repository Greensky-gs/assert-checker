CC=gcc
INPUT=main.c
OUTPUT=main.exe
FLAGS=-Wall -Wextra -g
SANTS=-fsanitize=address,undefined -g

$(OUTPUT): 
	rm -f $(OUTPUT)
	$(CC) $(INPUT) $(FLAGS) $(SANTS) -o $(OUTPUT)

launch:
	rm -f $(OUTPUT)
	make $(OUTPUT)
	clear
	./$(OUTPUT)
