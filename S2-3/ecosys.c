#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "ecosys.h"
#include <string.h>

float p_ch_dir=0.01; //probabilite de changer de direction de deplacement
float p_reproduce_proie=0.4;
float p_reproduce_predateur=0.5;
int temps_repousse_herbe=-15;

/* PARTIE 1*/
/* Fourni: Part 1, exercice 4, question 2 */
Animal *creer_animal(int x, int y, float energie) {
  Animal *na = (Animal *)malloc(sizeof(Animal));
  assert(na);
  na->x = x;
  na->y = y;
  na->energie = energie;
  na->dir[0] = rand() % 3 - 1;
  na->dir[1] = rand() % 3 - 1;
  na->suivant = NULL;
  return na;
}


/* Fourni: Part 1, exercice 4, question 3 */
Animal *ajouter_en_tete_animal(Animal *liste, Animal *animal) {
  assert(animal);
  assert(!animal->suivant);
  animal->suivant = liste;
  return animal;
}

/*Part 1, exercice 6, question 2 */
void ajouter_animal(int x, int y,  float energie, Animal **liste_animal) {
  assert(liste_animal);
  assert((x>=0 && x<SIZE_X)&&(y>=0 && y<SIZE_Y));
  Animal *animal = creer_animal(x,y,energie);
  *liste_animal=ajouter_en_tete_animal(*liste_animal,animal);
}

/*Part 1, exercice 5, question 5 */

/*Ici 3 cas pour enlever : 1) la tête de "liste" est l'animal qu'on veut enlever ; 2) l'animal à enlever est le suivant de notre animal courant "p", on l'enlève et on force la sortie de la boucle ; 3) si aucun des 2, alors on met à jour nos itérants "prec" et "p" */

void enlever_animal(Animal **liste, Animal *animal) {
    Animal* prec = NULL;
    Animal* p = *liste;
    assert(liste);

    if (p == animal) {
        *liste = (*liste)->suivant;
        free(p);
    } else {
        while (p) {
            if (p->suivant == animal) {
                prec = p;
                p = p->suivant;
                prec->suivant = p->suivant;
                free(p);
                break;
            } else {
                prec = p;
                p = p->suivant;
            }
        }
    }
  return ;
}

/*Part 1, exercice 6, question 7 */

/*Fonctionnement : on parcourt tant que "p" n'est pas null, "prec" pointe sur l'élément courant de "p", on met à jour "p" pour qu"il pointe sur son suivant et on libère "prec", on répète jusqu'à ce que liste soit vide*/
Animal* liberer_liste_animaux(Animal *liste) {
	Animal *p = liste;
	Animal *prec = NULL;
	while(p!=NULL){
		prec = p;
		p=p->suivant;
		free(prec);
	}
	return NULL;
}

/* Fourni: part 1, exercice 4, question 4 */
unsigned int compte_animal_rec(Animal *la) {
  if (!la) return 0;
  return 1 + compte_animal_rec(la->suivant);
}

/* Fourni: part 1, exercice 4, question 4 */
unsigned int compte_animal_it(Animal *la) {
  int cpt=0;
  while (la) {
    ++cpt;
    la=la->suivant;
  }
  return cpt;
}



/* Part 1. Exercice 5, question 1, ATTENTION, ce code est susceptible de contenir des erreurs... */
void afficher_ecosys(Animal *liste_proie, Animal *liste_predateur) {
  unsigned int i, j;
  char ecosys[SIZE_X][SIZE_Y];
  Animal *pa=NULL;

  for (i = 0; i < SIZE_X; ++i) {
    for (j = 0; j < SIZE_Y; ++j) {
      ecosys[i][j]=' ';
    }
  }


  // On ajoute les proies
  pa = liste_proie;
  while (pa) {
    ecosys[pa->x][pa->y] = '*';
    pa=pa->suivant;
  }

  // On ajoute les predateurs
  pa = liste_predateur;
  while (pa) {
      if ((ecosys[pa->x][pa->y] == '@') || (ecosys[pa->x][pa->y] == '*')) { 
        ecosys[pa->x][pa->y] = '@';
      } else {
        ecosys[pa->x][pa->y] = 'O';
      }
    pa = pa->suivant;
  }

  // On affiche le tableau
  printf("+");
  for (j = 0; j < SIZE_Y; ++j) {
    printf("-");
  }  
  printf("+\n");
  for (i = 0; i < SIZE_X; ++i) {
    printf("|");
    for (j = 0; j < SIZE_Y; ++j) {
      putchar(ecosys[i][j]);
    }
    printf("|\n");
  }
  printf("+");
  for (j = 0; j < SIZE_Y; ++j) {
    printf("-");
  }
  printf("+\n");
  int nbproie=compte_animal_it(liste_proie);
  int nbpred=compte_animal_it(liste_predateur);
  
  printf("Nb proies : %5d\tNb predateurs : %5d\n", nbproie, nbpred);
  
  //il faut mettre i < SIZE_X et j < SIZE_Y
}


void clear_screen() {
  printf("\x1b[2J\x1b[1;1H");  /* code ANSI X3.4 pour effacer l'ecran */
}



/* PARTIE 2*/



/* Part 2. Exercice 4, question 1 */

/*Fonctionnement: 1) création de listeA qui pointe sur "la" et on vérifie que la liste n'est pas null ; 2) si notre condition est vérifié, on met à jour notre tableau dir avec des valeurs aléatoires entre -1 et 1 ; 3) on ajoute à nos coordonnées les nouvelles directions */
void bouger_animaux(Animal *la) {
  Animal *listeA = la;
  //assert(listeA);
  
	while(listeA != NULL){
		if (rand()/(float)RAND_MAX < p_ch_dir){
			listeA->dir[0] = rand()%3 - 1;
			listeA->dir[1] = rand()%3 - 1;
		}
		
		listeA->x = (listeA->x + listeA->dir[0] + SIZE_X) % SIZE_X;
		listeA->y = (listeA->y + listeA->dir[1] + SIZE_Y) % SIZE_Y;
		listeA = listeA->suivant;
	}
}

/* Part 2. Exercice 4, question 3 */
/*Fonctionnement: 1) Si "liste_animal" n'est pas vide, alors on initialise une variable "a" sinon elle est null ; 2) on parcourt la liste "a" ; on calcule une probabilité et si elle est plus petite que p_reproduce, on crée un nouvel animal avec les mêmes coordonnées que notre "a" courant ; son énergie est l'énergie de "a"/2 ; 3) sinon, on met à jour "a" avec son suivant. */
void reproduce(Animal **liste_animal, float p_reproduce) {
	Animal *a;
	if (liste_animal != NULL) {
  		a = *liste_animal;
	} else {
  		a = NULL;
	}
	while(a){
		if(rand()/(float)RAND_MAX < p_reproduce){
      			ajouter_animal(a->x, a->y, a->energie/2.0, liste_animal);
      			a->energie = a->energie/2.0;
    		}

    		a = a->suivant;
  	}

}


// Part 2. Exercice 6, question 1
/*Fonctionnement: 1) On fait bouger les proies avec bouger_animaux() ; 2) on initialise notre liste temporaire "listeA" avec la liste_proie si elle n'est pas vide ; 3) Parcours de la listeA : on baisse l'énergie de 1 de la proie courante ; 4) si son énergie est à 0, on utilise enlever_animal() pour la faire mourir ; 5) sinon si  à la case où se trouve la proie il y a de l'herbe, son énergie augmente selon le nombre d'herbes */
void rafraichir_proies(Animal **liste_proie, int monde[SIZE_X][SIZE_Y]) {
	bouger_animaux(*liste_proie);
  	Animal *listeA = liste_proie ? *liste_proie : NULL;

  	while(listeA != NULL){
    		listeA->energie-=1;
    		if (listeA->energie < 0){
      			enlever_animal(liste_proie, listeA);
    		}
    		//Condition ajoutée pour la q.3 de l'exo 7
    		else if (monde[listeA->x][listeA->y] > 0){
			listeA->energie += monde[listeA->x][listeA->y];
			monde[listeA->x][listeA->y] = temps_repousse_herbe;
    		}

    		listeA = listeA->suivant;
 	}

  	reproduce(liste_proie, p_reproduce_proie);
}

/* Part 2. Exercice 7, question 1 */
/*Fonctionnement: On parcourt la liste l, si on trouve un animal aux coordonnées demandées, alors on renvoie un pointeur sur l'animal correspondant, sinon NULL si il n'y en a aucun. */
Animal *animal_en_XY(Animal *l, int x, int y) {
	while(l != NULL){
		if((l->x==x)&&(l->y==y)){
			return l;
		}
		l=l->suivant;
	}
  	return NULL;
} 

/* Part 2. Exercice 7, question 2 */
void rafraichir_predateurs(Animal **liste_predateur, Animal **liste_proie) {
	bouger_animaux(*liste_predateur);
  	Animal *pred;
	if (liste_predateur) {
   		pred = *liste_predateur;
	} 
	else {
   		pred = NULL;
	}

  	while(pred){
    	pred->energie--;
    	if (pred->energie < 0){
      		enlever_animal(liste_predateur, pred);
    	}
    	else {
      		Animal *proie = animal_en_XY(*liste_proie, pred->x, pred->y);
      		if (proie != NULL){
        		pred->energie += proie->energie;
        		enlever_animal(liste_proie, proie);
      		}
    	}
    	pred = pred->suivant;
  	}
  	reproduce(liste_predateur, p_reproduce_predateur);
}


/* Part 2. Exercice 5, question 2 */
void rafraichir_monde(int monde[SIZE_X][SIZE_Y]){
	for(int i = 0; i < SIZE_X; i++){
    		for(int j = 0; j < SIZE_Y; j++){
      			monde[i][j]++;
      		}
      	}
}

void ecrire_ecosys(const char *nom_fichier, Animal *liste_proie, Animal *liste_predateur){
	FILE *f = fopen(nom_fichier, "w");
	if (f == NULL){
		printf("Erreur lors de l'ouverture.");
		exit(1);
	}

	fprintf(f, "<proie>\n");
	Animal *tmp = liste_proie;
	while(tmp){
		fprintf(f, "x=%d y=%d dir=[%d,%d] e=%f\n", tmp->x, tmp ->y, tmp->dir[0], tmp->dir[1], tmp->energie);
		tmp = tmp->suivant;
	}
	fprintf(f,"</proie>\n");

	fprintf(f,"<predateur>\n");
	tmp = liste_predateur;
	while(tmp){
		fprintf(f, "x=%d y=%d dir=[%d,%d] e=%f\n", tmp->x, tmp ->y, tmp->dir[0], tmp->dir[1], tmp->energie);
		tmp = tmp->suivant;
	}
	fprintf(f, "</predateur>\n");
	
	fclose(f);
}

void lire_ecosys(const char *nom_fichier, Animal **liste_proie, Animal **liste_predateur){
	FILE *f = fopen(nom_fichier, "r");
	if (f == NULL){
		printf("Erreur lors de l'ouverture.");
		exit(1);
	}

	char buffer[256]; //max 256 caractères par ligne, on vise plus haut.
	fgets(buffer, 256, f);
	assert(strncmp(buffer, "<proie>", 7) == 0);

	int x_lu, y_lu, dir0, dir1; float e_lu;
	fgets(buffer, 256, f);
	while(strncmp(buffer, "</proie>", 8) != 0){
		sscanf(buffer, "x=%d y=%d dir=[%d,%d] e=%f\n", &x_lu, &y_lu, &dir0, &dir1, &e_lu);
		Animal *a = creer_animal(x_lu, y_lu, e_lu);
		a->dir[0] = dir0; 
		a->dir[1] = dir1;
		a->suivant = *liste_proie;
		*liste_proie = a;
		fgets(buffer, 256, f);
	}

	fgets(buffer, 256, f);
	assert(strncmp(buffer, "<predateur>", 11) == 0);

	fgets(buffer, 256, f);
	while(strncmp(buffer, "</predateur>", 12) != 0){
		sscanf(buffer, "x=%d y=%d dir=[%d,%d] e=%f\n", &x_lu, &y_lu, &dir0, &dir1, &e_lu);
		Animal *a = creer_animal(x_lu, y_lu, e_lu);
		a->dir[0] = dir0; 
		a->dir[1] = dir1;
		a->suivant = *liste_predateur;
		*liste_predateur = a;
		fgets(buffer, 256, f);
	}

	fclose(f);
}









