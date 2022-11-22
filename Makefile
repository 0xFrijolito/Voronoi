CC := gcc
SRC := $(wildcard ./*.c)
OUT := ./vonoroi.exe

all:
	$(CC) $(SRC) -o $(OUT)