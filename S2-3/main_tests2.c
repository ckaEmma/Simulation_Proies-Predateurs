#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "ecosys.h"


int main(){
	srand(time(NULL));

	// On crée nos listes "liste_proie" et "liste_predateurs"

	Animal *liste_proie = creer_animal(rand() % SIZE_X, rand() % SIZE_Y, 10);
	Animal *liste_predateur = creer_animal(rand() % SIZE_X, rand() % SIZE_Y, 10);
	
	for(int i = 0; i < 19; i++){
		ajouter_animal(rand() % SIZE_X, rand() % SIZE_Y, 10, &liste_proie);
		ajouter_animal(rand() % SIZE_X, rand() % SIZE_Y, 10, &liste_predateur);
	}
	
	// On vérifie qu'on a bien crée 20 animaux pour chaque liste avec les fonctions compte_animal
	
	printf("%d\n", compte_animal_rec(liste_proie));
	printf("%d\n", compte_animal_it(liste_predateur));
	
	//Q.5 d'exo 6 qui nous demande de tester liberer_liste_animaux
		/*afficher_ecosys(liste_proie, liste_predateur);
	
		liste_proie = liberer_liste_animaux(liste_proie);
		liste_predateur = liberer_liste_animaux(liste_predateur);*/


	//Part 2 exo 3 : On teste nos fonctions ecrire_ecosys() et lire_ecosys()

	Animal *liste_proie_l = NULL;
	Animal *liste_predateur_l = NULL;
	ecrire_ecosys("ecosys.txt", liste_proie, liste_predateur);
	lire_ecosys("ecosys.txt", &liste_proie_l, &liste_predateur_l);

	afficher_ecosys(liste_proie_l, liste_predateur_l); printf("\n");

	//Affichage des champs de chaque éléments de nos listes dans le terminal
	Animal *proie = liste_proie_l;
	printf("Proies :\n");
    while (proie) {
  		printf("%d, %d, %f\n", proie->x, proie->y, proie->energie);
  		proie = proie->suivant;
    }
  
	Animal *predateur = liste_predateur_l;
  	printf("\nPrédateurs :\n");
  	while (predateur) {
  		printf("%d, %d, %f\n", predateur->x, predateur->y, predateur->energie);
  		predateur = predateur->suivant;
    }
  
  	//Libération de toutes nos allocations
	liste_proie = liberer_liste_animaux(liste_proie);
	liste_predateur = liberer_liste_animaux(liste_predateur);
	liste_proie_l = liberer_liste_animaux(liste_proie_l);
	liste_predateur_l = liberer_liste_animaux(liste_predateur_l);

	//Nous testerons enlever_animal() dans main_tests.c, et non ici.
	return 0;
}
