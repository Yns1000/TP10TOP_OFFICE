////////////////////////// TP10

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "prod.h"

void destock(int quantite, int reference); // TODO

void lireCommande(char *ficCommande, char *Number); // TODO

int lireProchaineCommande() // pour lire l'int contenu dans nextFact
{
	FILE *f;
	int N;
	f = fopen("nextFact", "r");
	fread(&N, sizeof(int), 1, f);
	fclose(f);
	// printf("\n--->lu N=%d",N);
	return N;
}

void convertirNenChaine4(int N, char *N4) // convertit l'int N en une chaine de 4 caracteres
{										  // si N=1234 alors N4 sera égal à "1234"
	int m, c, d, u;
	char cm, cc, cd, cu;
	m = N / 1000;
	cm = 48 + m;
	N %= 1000;
	c = N / 100;
	cc = 48 + c;
	N %= 100;
	d = N / 10;
	cd = 48 + d;
	u = N % 10;
	cu = 48 + u;
	N4[0] = cm;
	N4[1] = cc;
	N4[2] = cd;
	N4[3] = cu;
	N4[4] = '\0';
}

void lireLesCommandes() // cette fonction ouvre tous les fichiers commandeXXX.txt avec XXXX démarrant à N
{
	FILE *ficCommande = NULL;
	int FINI = 0;
	int N = lireProchaineCommande(); // numero de la premiere commande qui sera lue et traitee
	char NNNN[5];
	char nomCommande[29];

	do // ce do while prend fin dès que fichier commandeXXXX.txt est absent
	{
		strcpy(nomCommande, "./commandes/commande");
		convertirNenChaine4(N, NNNN);
		printf("\n==>%s<==", NNNN);
		ficCommande = NULL;
		strcat(nomCommande, NNNN);
		strcat(nomCommande, ".txt");

		// printf("\n traitement de  %s",nomCommande);

		ficCommande = fopen(nomCommande, "rt"); // Ouverture en read / texte
		if (ficCommande != NULL)
		{ // le fichier commandeNNNN.txt existe
			printf("\n fichier %s present", nomCommande);
			lireCommande(nomCommande, NNNN); // à vous de coder cette fonction lors de ce TP9 //TODO

			fclose(ficCommande);
		}
		else
		{
			printf("\n\n toutes les commandes presentes ont ete traitees.\n\n");

			FILE *al = fopen("Alerte.txt", "rt");
			char buffer[TAILLE];

			if (fscanf(al, "%s", buffer) != EOF)
			{
				printf(" \n *** Vous avez une alerte stock ! \n *** Veuillez la consulter dans le fichier Alerte.txt. \n\n");
			}

			fclose(al);

			FILE *f = fopen("nextFact", "wt"); // on va ecrire la valeur de N dans enxtFact
			// pour
			fwrite(&N, 1, sizeof(int), f);
			fclose(f);
			FINI = 1;
		}

		N++;
	} while (FINI == 0);
}

int main()
{
	// creation d un fichier d'un seul int nommé nextFact et contenant l'int 1
	//  code à utiliser pour réinitialiser nextFact à 1 si besoin au cours du TP

	FILE *f;
	int N = 1;
	f = fopen("TMP/FirstnextFact", "wt");
	fwrite(&N, 1, sizeof(int), f);
	fclose(f);

	// PARTIE 1 du TP : sans Gestion de stock
	lireLesCommandes(); // lecture de tous les fichiers commandeXXX.txt (fichiers non traités jusqu'ici)

	// PARTIE 2 du TP : avec Gestion de stock
	// copiez coller votre travail précédent puis modifiez le
	// lireLesCommandes2();

	return 0;
}

void lireCommande(char *ficCommande, char Number[TAILLE])
{					   
	FILE *fCom = NULL; 
	FILE *fFac = NULL;

	// char chainetest[TAILLE];

	char client[TAILLE] = "";
	char facture[TAILLE] = "./factures/facture";
	float total = 0;
	int reference = 0, nbprod = 0;

	fCom = fopen(ficCommande, "r"); 

	strcat(Number, ".txt");
	strcat(facture, Number);

	fFac = fopen(facture, "wt");

	fscanf(fCom, "%s", client);

	float prix = 0;

	if (fCom != NULL) 
	{
		printf("\n-->Client = %s\n\n", client);
		if (fFac != NULL) 
		{
			fprintf(fFac, "Client : %s\n", client);
		}
		fclose(fFac);
		do
		{
			fscanf(fCom, "%d %d", &reference, &nbprod);

			if (!feof(fCom))
			{
				printf("-->Réf = %d   qtt=%d\n", reference, nbprod);

				FILE *fRef = NULL;
				fRef = fopen("produits.txt", "rt");
				int ref2 = 0;
				char nompro[TAILLE] = "";
				float HT = 0;

				if (fRef != NULL)
				{
					do
					{
						fscanf(fRef, "%d %s %f", &ref2, nompro, &prix);
						if (reference == ref2)
						{
							printf("--> Produit : %s | Prix : %.2f\n\n", nompro, prix);

							destock(nbprod, reference);

							total += (prix * nbprod);
							fFac = fopen(facture, "a");
							if (fFac != NULL)
							{
								HT = prix * nbprod;
								fprintf(fFac, "%d %s (PU=%.2f€) :: %.2f€\n", nbprod, nompro, prix, HT);
							}
							fclose(fFac);
						}
					} while (!feof(fRef));
					fclose(fRef);
				}
			}

		} while (!feof(fCom));
		fclose(fCom);

		fFac = fopen(facture, "a");
		fprintf(fFac, "\n		TOTAL = %.2f€", total);
		fclose(fFac);
	}
}

void destock(int quantite, int reference)
{
	FILE *fichier = NULL;
	FILE *fichier2 = NULL;

	fichier = fopen("stock.txt", "r+");
	fichier2 = fopen("TMP/test.txt", "w");

	int ref2 = 0;
	int qt2 = 0;
	int qtfinal = 0;
	int taille = 0;

	char chaineqt[TAILLE] = "";
	char refff[TAILLE] = "";

	if (fichier != NULL)
	{
		do
		{
			qtfinal = 0;
			fscanf(fichier, "%d %d", &ref2, &qt2);
		
		
			sprintf(chaineqt, "%d\n", qt2);
			sprintf(refff, "%d\n", ref2);

			if (!feof(fichier))
			{
				if (reference == ref2 && quantite != qt2)
				{
					qtfinal = qt2 - quantite;

					sprintf(chaineqt, "%d", qtfinal);
					strcat(refff, chaineqt);
					taille = strlen(refff);
					fseek(fichier, -taille, 1);

					fprintf(fichier, "%d %d\n", ref2, qtfinal);
					// printf("\n\n **DEBUG OUI %d %d** %d ***\n\n", ref2, qtfinal, taille);
					fprintf(fichier2, "\n %d %05d \n", ref2, qtfinal);
				}
				else
				{
					/*sprintf(chaineqt, "%d", qt2);
					strcat(refff, chaineqt);
					taille = strlen(refff);

					fseek(fichier, -taille, 1);*/

					fprintf(fichier2, "\n %d %05d \n", ref2, qt2);
					// printf("\n\n **DEBUG NON %d %d** \n\n", ref2, qt2);
				}
			}
			//}
		} while (!feof(fichier));
	}
	fclose(fichier);
	fclose(fichier2);

	fichier = fopen("TMP/test.txt", "r");
	fichier2 = fopen("stock.txt", "w");

	if (fichier2 != NULL)
	{
		do
		{

			fscanf(fichier, "%d %05d", &ref2, &qt2);
			if (!feof(fichier))
			{

				fprintf(fichier2, "%d %05d\n", ref2, qt2);
			}
		} while (!feof(fichier));
	}

	fclose(fichier2);
	fclose(fichier);

	remove("TMP/test.txt");

	fichier = fopen("stock.txt", "r");
	fichier2 = fopen("Alerte.txt", "w");

	if (fichier2 != NULL)
	{
		do
		{

			fscanf(fichier, "%d %d", &ref2, &qt2);
			if (qt2 <= 0)
			{
				if (!feof(fichier))
				{

					fprintf(fichier2, "Alerte ! Le stock du produit %d est vide !\n", ref2);
				}
			}
		} while (!feof(fichier));
	}

	fclose(fichier2);
	fclose(fichier);
}
