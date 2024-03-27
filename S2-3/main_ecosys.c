#include <assert.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <strings.h>
#include "ecosys.h"



#define NB_PROIES 20
#define NB_PREDATEURS 20
#define T_WAIT 40000



int main(void) {

  // A completer. Part 2:
  
   // exercice 4, questions 2 et 4 
   //Test de bouger_animaux()
	Animal *ani = creer_animal(5,9,10.0);
   	printf("Avant la fonction bouger_animaux() sur ani\n");
   	printf("Ani : %d, %d, %.2f, [%d,%d]\n",ani->x,ani->y,ani->energie,ani->dir[0],ani->dir[1]);
   //ici on force les changements de coordonnées avec le while
   	while((ani->x == 5) && (ani->y == 9))
   		bouger_animaux(ani);
   
   	printf("Ani après bouger_animal() : %d, %d, %.2f, [%d,%d]\n", ani->x, ani->y, ani->energie, ani->dir[0], ani->dir[1]);
   
   //On teste la fonction reproduce()
   	for(int i = 0; i < 2; i++)
   		reproduce(&ani, p_reproduce_proie);

	printf("Reproduce() : \n");
  	Animal *a = ani;
  	while(a != NULL){
    		printf("%d, %d, %.2f, [%d,%d]\n", ani->x, ani->y, ani->energie, ani->dir[0], ani->dir[1]);
    		a = a->suivant;
  	}
  	ani = liberer_liste_animaux(ani);
   
 
     
   
   // Création du tableau du monde, pour l'herbe
   	srand(time(NULL));
  	float energie_proie = 10.0;
  	float energie_predateur = 100.0;
	int monde[SIZE_X][SIZE_Y];
  	for (int i=0;i<SIZE_X;i++){
  		for(int j=0;j<SIZE_Y;j++){
    			monde[i][j]=0;
  		}
	}

  // Création de 20 animaux pour liste_proie et liste_predateur.

  Animal *liste_proie = creer_animal(rand() % SIZE_X, rand() % SIZE_Y, energie_proie);
  Animal *liste_predateur = creer_animal(rand() % SIZE_X, rand() % SIZE_Y, energie_predateur);
	
	for(int i = 1; i < NB_PROIES; i++){
		ajouter_animal(rand() % SIZE_X, rand() % SIZE_Y, energie_proie, &liste_proie);
    ajouter_animal(rand() % SIZE_X, rand() % SIZE_Y, energie_predateur, &liste_predateur);
	}
	
	//Ouverture/création d'un fichier en mode Write pour pouvoir écrire notre simulation et le lire sur le terminal ensuite
	FILE *f = fopen("fichier.txt", "w");
	if (f == NULL) {
		printf("Erreur lors de l'ouverture.");
	  	exit(1);
    	}
  
  	fprintf(f, "itération proies predateurs\n");


  //Lancement de la simulation

	int cpt = 0;
  //Notre condition de la boucle repose sur la vérification des 2 listes non-nulles avec un compteur d'itération pour s'arrêter à un moment
	while((liste_proie || liste_predateur) && cpt<75){
	
		//on écrit dans le fichier notre compteur d'itération, le nombre de proies et de prédateurs à cet étape
		fprintf(f, "%d %d %d\n", cpt, compte_animal_it(liste_proie), compte_animal_it(liste_predateur));
		
		//on l'affiche dans le terminal
		afficher_ecosys(liste_proie, liste_predateur);
		
		//on met en pause l'exécution du programme pendant 1 sec
		usleep(1000000);
		
		//on efface pour permettre d'afficher les changements des animaux dans l'écosystème
		clear_screen();
		
		//modification de l'écosystème
		rafraichir_proies(&liste_proie, monde);
		rafraichir_predateurs(&liste_predateur, &liste_proie);
		rafraichir_monde(monde);
		cpt++;
  	}

	//Libération de toutes nos allocations de la simulation
	fclose(f);
  	liste_proie = liberer_liste_animaux(liste_proie);
  	liste_predateur = liberer_liste_animaux(liste_predateur);
   // exercice 8, question 1 -> Pas fait
   

  return 0;
}

