# Variables
CC = gcc
CFLAGS = -Wall -Iinclude
SRC_DIR = src

# Cibles principales
all: Morpion_Q Morpion_2Q Q_learning Sarsa

# Compilation des fichiers .c en exécutables
Morpion_Q: $(SRC_DIR)/Morpion_Q.c
	$(CC) -o $@ $< $(CFLAGS)

Morpion_2Q: $(SRC_DIR)/Morpion_2Q.c
	$(CC) -o $@ $< $(CFLAGS)

Q_learning: $(SRC_DIR)/Q_learning.c
	$(CC) -o $@ $< $(CFLAGS)

Sarsa: $(SRC_DIR)/Sarsa.c
	$(CC) -o $@ $< $(CFLAGS)

# Nettoyage des exécutables
clean:
	rm -f Morpion_Q Morpion_2Q Q_learning Sarsa

.PHONY: all clean
