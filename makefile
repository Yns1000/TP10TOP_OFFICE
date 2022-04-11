c=gcc
op=-Wall -Wextra

all : main.c 	
	$(c) $(op)  main.c -o tp10

clean :
	rm -f tp10
	./scr.sh
	rm -f ./factures/*.txt alerte.txt 
	

# ./scr.sh est un script pour remettre l'entier de nextFact à 1
