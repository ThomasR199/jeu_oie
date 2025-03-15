// Version V3.0.3.1 censurée

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>
#include <sys/time.h>

#define LC 4
#define TAILLE 8

int count_dep = 0; // Compteur des retours au départ
int count_lancers = 0; 
bool professeurC = false;

// Découverte
bool decouverteDC = false;
bool decouverteDNI = false;
bool decouverteDSN = false;
bool decouverteDN = false;
bool decouverteBR = false;
bool decouvertePJ = false;
bool decouverteIM = false;
bool decouverteJE = false;
bool decouverteCA = false;
bool decouverteBC = false;
bool decouverteDE = false;
bool decouverteFA = false;
bool decouverteJA = false;
bool decouverteTR = false;
bool decouverteBO = false;
bool decouverteRE = false;
bool mdp[6] = {false,false,false,false,false,false};
int codes_mdp[3];
int codes_mdp_tot[6];

// Améliorations
float second_less = 0.0;
float money_give = 1.0;
float points_give = 1.0;

// Argent
struct Money{
  float r; // Argent réel
  float i; // Argent imaginaire
  float j; // Argent jacobique
  float c; // Commandes Linux
};
typedef struct Money money;
money argent = {.r = 100, .i = 0, .j = 0, .c = 0};
money argent_save = {.r = 0, .i = 0, .j = 0, .c = 0};
float points_count = 0.0;
int bitcoins = 0;
int pioche = 1;
int chance = 15;
int points_attente = 0;
int count_attente = 0;

// Position dans la dimension classique
int ligne_1 = 0;
int colonne_1 = 0;

int rand_max(){
  if (RAND_MAX == 32767){
      int a = rand() % 2;
      int b = rand();
      int c = rand();
      int ab = a * 32768 + b;
      int ab_shift = ab * 32768;
      int abc = ab_shift + c;
      return abc;
  } else {
      return rand();
  }
}

void reculer(int pas, int* ligne_1, int* colonne_1, char* dir_1, char* dir_2, bool aff);

const int tab[8][8] = // Dimension classique
{
  {0,1,2,3,4,5,6,7},
  {27,28,29,30,31,32,33,8},
  {26,47,48,49,50,51,34,9},
  {25,46,59,60,61,52,35,10},
  {24,45,58,63,62,53,36,11},
  {23,44,57,56,55,54,37,12},
  {22,43,42,41,40,39,38,13},
  {21,20,19,18,17,16,15,14},
};

int tab2[8][8]; // Dimension non initialisée
int tab3[8][8] = // Dimension nulle
{
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0}
};

int tab4[8][8]; // Dimension négative, Backrooms, Prison de Jacob
int tab5[8][8]; // Dimension imaginaire

int tab_morp[3][3] = {{0,0,0},{0,0,0},{0,0,0}}; // Morpion
int cartes[30]={0};

int sommeDep(int m, int d){
  // Somme des m/2^k avec k de 0 à d-1
  int s = 0;
  for (int i = 0; i < d; i += 1){
    s += m/pow(2,i);
  }
  return s;
}

void Sleep(int seconds){ // Problèmes de compatibilité de unistd.h
  struct timeval stop,start;
  gettimeofday(&start, NULL);
  gettimeofday(&stop, NULL);
  while((stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec <= seconds*1000000){
    gettimeofday(&stop, NULL);
  }
}

// MASTERMIND

void creeCombinaison(int comb[],int taille,int NC){
  for(int i = 0; i < taille; i += 1){
    comb[i] = rand_max() % NC;
  }
}

void choixCombinaison(int comb[],int taille,int NC){
  for(int i = 0; i < taille; i += 1){
    int x;
    printf("Entrez un nombre (n°%d) entre 0 et %d : ",i+1,NC-1);
    while (scanf("%d", &x) != 1 || x < 0 || x >= NC) {
      while (getchar()!= '\n');
      printf("Saisie invalide !\n");
    printf("Entrez un nombre (n°%d) entre 0 et %d : ",i+1,NC-1);
    }
    while (getchar()!= '\n');
    comb[i] = x;
  }
}

int nbBP(int cc[],int cj[],int taille,int NC){
  // Nombre de bien placés
  int nbP = 0;
  for(int i = 0; i < taille; i += 1){
    if (cc[i] == cj[i]){
      nbP += 1;
      cc[i] = NC; cj[i] = NC;
    }
  }
  return nbP;
}

int nbBC(int cc[],int cj[],int taille,int NC){
  // Nombre de bonnes couleurs
  int nbC = 0;
  for(int i = 0; i < taille; i += 1){
    if (cj[i] != NC){
      for(int j = 0; j < taille; j += 1){
        if (cc[j] == cj[i]){
          nbC += 1;
          cj[i] = NC; cc[j] = NC;
          break;
        }
      }
    }
  }
  return nbC;
}

void afficheTab(int tab[],int taille){
  // Affichage de la combinaison jouée
  printf("[");
  for(int i = 0; i < taille; i += 1){
    if (i == (taille - 1)){
      printf("%d",tab[i]);
    } else {
      printf("%d,",tab[i]);
    }
  }
  printf("]");
}

bool mastermind(int NT, int NC){
  int cc[LC];
  creeCombinaison(cc,LC,NC);
  int compt = 0;
  bool victoire_m = false;
  int cj[LC]; int cc2[LC]; int cj2[LC];
  while (compt < NT && !victoire_m){
    compt += 1;
    printf("Choisissez votre combinaison : \n");
    choixCombinaison(cj,LC,NC);
    for (int i = 0; i < LC; i += 1){
      cc2[i] = cc[i]; cj2[i] = cj[i];
    }
    int nbP = nbBP(cc2,cj2,LC,NC);
    int nbC = nbBC(cc2,cj2,LC,NC);
    printf("Tentative n° %d : ",compt);
    afficheTab(cj,LC);
    printf(" : il y a %d bien placé(s) et %d bonne(s) couleur(s)\n",nbP,nbC);
    if (nbP == LC){
      victoire_m = true;
    }
  }
  if (victoire_m){
    printf("BRAVO ! Vous avez gagné après %d coups.\n",compt);
    return true;
  } else {
    printf("PERDU, la combinaison était :");
    afficheTab(cc,LC);
    printf("\n");
    return false;
  }
}

// GRILLE JEU DE L'OIE

void afficheGrille(const int tab[TAILLE][TAILLE],int taille, int colonne_1, int ligne_1){
  // Affichage de la grille du jeu de l'oie
  for(int i = 0; i < taille; i += 1){
    for(int j = 0; j < taille; j += 1){
      printf("*-**-");
    }
    printf("*\n");
    for(int j = 0; j < taille; j += 1){
      if (i == colonne_1 && j == ligne_1) printf("| XX "); // Position du joueur
      else if (tab[i][j] < 10 && tab[i][j] >= 0) printf("| 0%d ",tab[i][j]); // Cases à 1 chiffre
      else printf("| %d ",tab[i][j]);
    }
    printf("|\n");
  }
  for(int j = 0; j < taille; j += 1){
    printf("*-**-");
  }
  printf("*\n\n");
}
void afficheGrilleIm(const int tab[TAILLE][TAILLE],int taille, int colonne_1, int ligne_1){
  // Affiche la grille en dimension imaginaire
  for(int i = 0; i < taille; i += 1){
    for(int j = 0; j < taille; j += 1){
      printf("*-*-*-");
    }
    printf("*\n");
    for(int j = 0; j < taille; j += 1){
      if (i == colonne_1 && j == ligne_1) printf("| XXX "); // Position du joueur
      else if (tab[i][j] == 0) printf("|  0  ");
      else if (tab[i][j] == 1) printf("|  i  ");
      else if (tab[i][j] < 10 && tab[i][j] >= 2) printf("| 0%di ",tab[i][j]); // Cases à 1 chiffre (2 à 10)
      else printf("| %di ",tab[i][j]);
    }
    printf("|\n");
  }
  for(int j = 0; j < taille; j += 1){
    printf("*-*-*-");
  }
  printf("*\n\n");
}
void afficheGrilleFaure(const int tab[TAILLE][TAILLE],int taille, int colonne_1, int ligne_1){
  for(int i = 0; i < taille; i += 1){
    for(int j = 0; j < taille; j += 1){
      printf("*-**-");
    }
    printf("*\n");
    for(int j = 0; j < taille; j += 1){
      printf("| ?? ");
    }
    printf("|\n");
  }
  for(int j = 0; j < taille; j += 1){
    printf("*-**-");
  }
  printf("*\n\n");
}

// DIMENSION CLASSIQUE

void dir(int* ligne, int* colonne, char* dir_1, char* dir_2){
  /* Calcule la direction du prochain mouvement du joueur
  *dir1 : le joueur avance
  *dir2 : le joueur recule
  */
  if (*ligne <= 3){
    if (*colonne >= 7-*ligne) *dir_1 = 'B';
    else if (*colonne <= *ligne-2) *dir_1 = 'H';
    else *dir_1 = 'D';

    if (*colonne < *ligne) *dir_2 = 'B';
    else if (*colonne >= 8-*ligne) *dir_2 = 'H';
    else *dir_2 = 'G';
  } else if (*ligne == 4){
    if (*colonne >= 5) *dir_1 = 'B';
    else if (*colonne <= 3) *dir_1 = 'H';
    else *dir_1 = 'G';

    if (*colonne <= 2) *dir_2 = 'B';
    else if (*colonne >= 4) *dir_2 = 'H';
    else *dir_2 = 'D';
  } else if (*ligne >= 5){
    if (*colonne >= *ligne + 1) *dir_1 = 'B';
    else if (*colonne <= 7-*ligne) *dir_1 = 'H';
    else *dir_1 = 'G';

    if (*colonne <= 6-*ligne) *dir_2 = 'B';
    else if (*colonne >= *ligne) *dir_2 = 'H';
    else *dir_2 = 'D';
  }
}

void avancer(int pas, int* ligne_1, int* colonne_1, char* dir_1, char* dir_2, bool aff){
  // Avance le joueur d'une case
  for(int i = 0; i < pas; i += 1){
    dir(ligne_1,colonne_1,dir_1,dir_2); // Calcule la direction du mouvement
    if (*ligne_1 == 4 && *colonne_1 == 3){ // Case 63 (arrivée)
      reculer(pas-i,ligne_1,colonne_1,dir_1,dir_2,false);
      break;
    } else if (*dir_1 == 'D'){
      *colonne_1 += 1;
    } else if (*dir_1 == 'B'){
      *ligne_1 += 1;
    } else if (*dir_1 == 'G'){
      *colonne_1 -= 1;
    } else {
      *ligne_1 -= 1;
    }
  }
  if (aff) printf("\nVous avez avancé de %d case(s)\n\n",pas);
}
void reculer(int pas, int* ligne_1, int* colonne_1, char* dir_1, char* dir_2, bool aff){
  // Recule le joueur d'une case
  for(int i = 0; i < pas; i += 1){
    dir(ligne_1,colonne_1,dir_1,dir_2);
    if (*ligne_1 == 0 && *colonne_1 == 0){ // Case 0
      avancer(pas-i,ligne_1,colonne_1,dir_1,dir_2,false);
      break;
    } else if (*dir_2 == 'D'){
      *colonne_1 += 1;
    } else if (*dir_2 == 'B'){
      *ligne_1 += 1;
    } else if (*dir_2 == 'G'){
      *colonne_1 -= 1;
    } else {
      *ligne_1 -= 1;
    }
  }
  if (aff) printf("\nVous avez reculé de %d case(s)\n\n",pas);
}

// AUTRES DIMENSIONS

void avancerCh(int pas, int* ligne, int* colonne, bool aff){ 
  // Avancer en dimension 2
  for(int i = 0; i < pas; i += 1){
    bool valide = false; // Mouvement aléatoire valide ?
    int dir;
    while(!valide){
      dir = rand_max() % 4; // 0 : Haut, 1 : Bas, 2 : Droite, 3 : Gauche
      if ((*ligne != 0 || dir != 0) && (*ligne != 7 || dir != 1) && (*colonne != 7 || dir != 2) && (*colonne != 0 || dir != 3)){
        valide = true; // Pour éviter d'avancer en dehors de la grille
      }
    }
    if (dir == 0) *ligne -= 1;
    else if (dir == 1) *ligne += 1;
    else if (dir == 2) *colonne += 1;
    else *colonne -= 1;
  }
  if (aff) printf("\nVous avez reculé de %d case(s)\n\n",pas);
}

// Aléatoire

int lancer(){
  return (rand_max() % 6) + 1; // Entre 1 et 6
}

int caseRandom(){
  return (rand_max() % 100); // Entre 0 et 99
}

bool JustePrix(int dim_a, int NT, int lim, int* justePrix){
  int combinaison = rand_max() % (lim + 1);
  *justePrix = combinaison; // Sauvegarde du juste prix (système d'argent)
  int choix; int counter = 0;
  bool vict = false;
  if (dim_a == 7) printf("%d 0 & %d : \n",rand_max(),lim);
  else printf("Choisissez un nombre entre 0 et %d : \n",lim);
  while (!vict && counter < NT){
    while (scanf("%d", &choix) != 1) {
      if (dim_a == 7) printf("%d",rand_max());
      else printf("Erreur de saisie\n");
      counter += 1;
      while (getchar()!='\n');
    }
    while (getchar()!='\n');
    if (combinaison > choix) {
      if (dim_a == 7) printf("choix < ...\n");
      else printf("Trop petit ...\n");
    } else if (combinaison < choix) {
      if (dim_a == 7) printf("choix > ...\n");
      else printf("Trop grand ...\n");
    } else {
      if (dim_a == 7) printf("%d\n",rand_max());
      else printf("Bravo !!\n");
      vict = true;
    }
    counter += 1;
  }
  if(vict) return true;
  else return false;
}

void MalusTP(int temps, int dim){
  if (dim == 2 || dim == 0 || dim == 5){
    // Dimensions classique, non initialisée
    printf("On ne discute qu'après avoir codé le TP en entier !\n");
    printf("Vous avez %f secondes à attendre\n\n",(float)temps-second_less);
    Sleep((float)temps-second_less);
    printf("Romain : Monsieur ! J'ai fini ! Je fais quoi après ?\n\n");
  } else if (dim == 3 || dim == 4){
    // Dimensions imaginaires
    printf("On ne discute qu'après avoir imaginé le TP en entier !\n");
    printf("Vous avez %fi secondes à attendre\n\n",(float)temps-second_less);
    Sleep((float)temps-second_less);
    printf("Romain imaginaire pur : Monsieur ! J'ai fini ! Je fais quoi après ?\n\n");
  } else if (dim == -1 || dim == -2 || dim == -3){
    // Dimensions négatives
    printf("TP : Monsieur ! J'ai fini Romain ! Je fais quoi après ?\n\n");
    printf("Vous avez -%f secondes à attendre\n\n",(float)temps-second_less);
    Sleep((float)temps-second_less);
    printf("On ne discute qu'après avoir codé le Romain en entier !\n\n");
  } else if (dim <= -6) {
    // Dimensions Faure
    printf("On ne code qu'après avoir discuté tous ensemble du TP en entier (même ici cette case vous hante) !\n");
    printf("Vous avez %f secondes à attendre\n\n",(float)temps-second_less);
    Sleep((float)temps-second_less);
  } else if (dim == 7){
    // Fond du trou
    printf("%d !\n",rand_max());
    printf("%d %f %d\n\n",rand_max(),(float)temps-second_less,rand_max());
    Sleep((float)temps-second_less);
  } else {
    // Dimensions Carcenac, nulle, Jansou
    printf("On ne code qu'après avoir discuté tous ensemble du TP en entier !\n");
    printf("Vous avez %f secondes à attendre\n\n",(float)temps-second_less);
    Sleep((float)temps-second_less);
  }
}

void Prison(int temps, int dim){
  if (dim == 6){
    int prison_rand = rand_max()%4;
    if (prison_rand == 0) Prison(temps,-1);
    else if (prison_rand == 1) Prison(temps,0);
    else if (prison_rand == 2) Prison(temps,3);
    else if (prison_rand == 3) Prison(temps,1);
  } else if (dim <= -1 && dim >= -3){
    // Dimensions négatives
    printf("La prison entre en vous !\n");
    if (temps-20-second_less < 0) printf("Attente : -%f secondes\n",20.0);
    else printf("Attente : -%f secondes\n",temps-10-second_less);
    Sleep(10);
    printf("Pour accompagner -- : vous\n");
    Sleep(temps-20-second_less);
    printf("Je vous ai enlevé 10 secondes.\n\n");
    Sleep(10);
  } else if (dim == 0){
    // Dimension nulle
    if (temps-20-second_less < 0) printf("Attente : 0^0 * %f secondes\n",20.0);
    else printf("Attente : 0^0 * %f secondes\n",temps-10-second_less);
    Sleep(10);
    printf("Pour vous accompagner : 0\n");
    Sleep(temps-20-second_less);
    printf("Je vous ai rajouté 0^0 * 10 secondes.\n\n");
    Sleep(10);
  } else if (dim == 7) {
    // Dimensions imaginaires
    printf("%d %d !\n",rand_max(),rand_max());
    if (temps-20-second_less < 0) printf("%d : %fi %d\n",rand_max(),20.0,rand_max());
    else printf("%d : %fi %d\n",rand_max(),temps-10-second_less,rand_max());
    Sleep(10);
    printf("%d : %d\n",rand_max(),rand_max());
    Sleep(temps-20-second_less);
    printf("%d 10i %d.\n\n",rand_max(),rand_max());
    Sleep(10);
  } else if (dim >= 3) {
    // Dimensions imaginaires
    printf("Vous entrez dans une prison imaginaire !\n");
    if (temps-20-second_less < 0) printf("Attente : %fi secondes\n",20.0);
    else printf("Attente : %fi secondes\n",temps-10-second_less);
    Sleep(10);
    printf("Pour vous accompagner : i\n");
    Sleep(temps-20-second_less);
    printf("Je vous ai rajouté 10i secondes.\n\n");
    Sleep(10);
  } else {
    // Dimensions Carcenac, classique, non initialisée
    printf("Vous entrez en prison !\n");
    if (temps-20-second_less < 0) printf("Attente : %f secondes\n",20.0);
    else printf("Attente : %f secondes\n",temps-10-second_less);
    Sleep(10);
    printf("Pour vous accompagner : ++\n");
    Sleep(temps-20-second_less);
    printf("Je vous ai rajouté 10 secondes.\n\n");
    Sleep(10);
  }
}

// RICK

void printRick(){
    printf("\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u28C0\u2840\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\n\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2880\u28F6\u28FF\u28FF\u28FF\u28FF\u28FF\u28C4\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\n\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2880\u28FF\u28FF\u28FF\u283F\u281F\u281B\u283B\u28FF\u2806\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\n\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u28B8\u28FF\u28FF\u28FF\u28C6\u28C0\u28C0\u2800\u28FF\u2802\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\n\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u28B8\u283B\u28FF\u28FF\u28FF\u2805\u281B\u280B\u2808\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\n\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2818\u28BC\u28FF\u28FF\u28FF\u28C3\u2820\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\n\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u28FF\u28FF\u28DF\u287F\u2803\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\n\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u28DB\u28DB\u28EB\u2844\u2800\u28B8\u28E6\u28C0\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\n\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2880\u28E0\u28F4\u28FE\u2846\u2838\u28FF\u28FF\u28FF\u2877\u2802\u2828\u28FF\u28FF\u28FF\u28FF\u28F6\u28E6\u28E4\u28C0\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\n\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u28E4\u28FE\u28FF\u28FF\u28FF\u28FF\u2847\u2880\u28FF\u287F\u280B\u2801\u2880\u2876\u282A\u28C9\u28B8\u28FF\u28FF\u28FF\u28FF\u28FF\u28C7\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\n\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2880\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u284F\u28B8\u28FF\u28F7\u28FF\u28FF\u28F7\u28E6\u2859\u28FF\u28FF\u28FF\u28FF\u28FF\u284F\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\n\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2808\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28C7\u28B8\u28FF\u28FF\u28FF\u28FF\u28FF\u28F7\u28E6\u28FF\u28FF\u28FF\u28FF\u28FF\u2847\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\n\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u28A0\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u2847\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\n\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u28B8\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28C4\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\n\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2838\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\n\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u28E0\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u287F\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\n\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u2803\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\n\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u28B9\u28FF\u28F5\u28FE\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28FF\u28EF\u2841\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\u2800\n\n");
}

bool RRR(int chance, int choix){
  // Roulette à Rickroll
  int nbAlea = (rand_max()%choix)+1;
  if (nbAlea <= chance){
    printf("Pas de chance !\n");
    printRick();
    printf("You have been rickrolled !\n\n");
    return true;
  }
  else{
    return false;
  }
}

void ParolesRick(){
  // Parole au hasard de 'Never Gonna Give You Up' de Rick Astley (case réclamée par Lucas D.)
  int parole = rand_max() % 342;
  printf("Mot (au sens informatique) n°%d :\n",parole+1);
  if (parole == 0) printf("We're\n");
  else if (parole == 1) printf("no\n");
  else if (parole == 2) printf("strangers\n");
  else if (parole == 3) printf("to\n");
  else if (parole == 4) printf("love\n");
  else if (parole == 5) printf("You\n");
  else if (parole == 6) printf("know\n");
  else if (parole == 7) printf("the\n");
  else if (parole == 8) printf("rules\n");
  else if (parole == 9) printf("and\n");
  else if (parole == 10) printf("so\n");
  else if (parole == 11) printf("do\n");
  else if (parole == 12) printf("I\n");
  else if (parole == 13) printf("(do\n");
  else if (parole == 14) printf("I)\n");
  else if (parole == 15) printf("A\n");
  else if (parole == 16) printf("full\n");
  else if (parole == 17) printf("commitment's\n");
  else if (parole == 18) printf("what\n");
  else if (parole == 19) printf("I'm\n");
  else if (parole == 20) printf("thinking\n");
  else if (parole == 21) printf("of\n");
  else if (parole == 22) printf("You\n");
  else if (parole == 23) printf("wouldn't\n");
  else if (parole == 24) printf("get\n");
  else if (parole == 25) printf("this\n");
  else if (parole == 26) printf("from\n");
  else if (parole == 27) printf("any\n");
  else if (parole == 28) printf("other\n");
  else if (parole == 29) printf("guy\n");
  else if (parole == 30) printf("I\n");
  else if (parole == 31) printf("just\n");
  else if (parole == 32) printf("wanna\n");
  else if (parole == 33) printf("tell\n");
  else if (parole == 34) printf("you\n");
  else if (parole == 35) printf("how\n");
  else if (parole == 36) printf("I'm\n");
  else if (parole == 37) printf("feeling\n");
  else if (parole == 38) printf("Gotta\n");
  else if (parole == 39) printf("make\n");
  else if (parole == 40) printf("you\n");
  else if (parole == 41) printf("understand\n");
  else if (parole == 42) printf("Never\n");
  else if (parole == 43) printf("gonna\n");
  else if (parole == 44) printf("give\n");
  else if (parole == 45) printf("you\n");
  else if (parole == 46) printf("up\n");
  else if (parole == 47) printf("Never\n");
  else if (parole == 48) printf("gonna\n");
  else if (parole == 49) printf("let\n");
  else if (parole == 50) printf("you\n");
  else if (parole == 51) printf("down\n");
  else if (parole == 52) printf("Never\n");
  else if (parole == 53) printf("gonna\n");
  else if (parole == 54) printf("run\n");
  else if (parole == 55) printf("around\n");
  else if (parole == 56) printf("and\n");
  else if (parole == 57) printf("desert\n");
  else if (parole == 58) printf("you\n");
  else if (parole == 59) printf("Never\n");
  else if (parole == 60) printf("gonna\n");
  else if (parole == 61) printf("make\n");
  else if (parole == 62) printf("you\n");
  else if (parole == 63) printf("cry\n");
  else if (parole == 64) printf("Never\n");
  else if (parole == 65) printf("gonna\n");
  else if (parole == 66) printf("say\n");
  else if (parole == 67) printf("goodbye\n");
  else if (parole == 68) printf("Never\n");
  else if (parole == 69) printf("gonna\n");
  else if (parole == 70) printf("tell\n");
  else if (parole == 71) printf("a\n");
  else if (parole == 72) printf("lie\n");
  else if (parole == 73) printf("and\n");
  else if (parole == 74) printf("hurt\n");
  else if (parole == 75) printf("you\n");
  else if (parole == 76) printf("We've\n");
  else if (parole == 77) printf("known\n");
  else if (parole == 78) printf("each\n");
  else if (parole == 79) printf("other\n");
  else if (parole == 80) printf("for\n");
  else if (parole == 81) printf("too\n");
  else if (parole == 82) printf("long\n");
  else if (parole == 83) printf("Your\n");
  else if (parole == 84) printf("heart's\n");
  else if (parole == 85) printf("been\n");
  else if (parole == 86) printf("aching,\n");
  else if (parole == 87) printf("but\n");
  else if (parole == 88) printf("you're\n");
  else if (parole == 89) printf("too\n");
  else if (parole == 90) printf("shy\n");
  else if (parole == 91) printf("to\n");
  else if (parole == 92) printf("say\n");
  else if (parole == 93) printf("it\n");
  else if (parole == 94) printf("(say\n");
  else if (parole == 95) printf("it)\n");
  else if (parole == 96) printf("Inside,\n");
  else if (parole == 97) printf("we\n");
  else if (parole == 98) printf("both\n");
  else if (parole == 99) printf("know\n");
  else if (parole == 100) printf("what's\n");
  else if (parole == 101) printf("been\n");
  else if (parole == 102) printf("going\n");
  else if (parole == 103) printf("on\n");
  else if (parole == 104) printf("(going\n");
  else if (parole == 105) printf("on)\n");
  else if (parole == 106) printf("We\n");
  else if (parole == 107) printf("know\n");
  else if (parole == 108) printf("the\n");
  else if (parole == 109) printf("game\n");
  else if (parole == 110) printf("and\n");
  else if (parole == 111) printf("we're\n");
  else if (parole == 112) printf("gonna\n");
  else if (parole == 113) printf("play\n");
  else if (parole == 114) printf("it\n");
  else if (parole == 115) printf("And\n");
  else if (parole == 116) printf("if\n");
  else if (parole == 117) printf("you\n");
  else if (parole == 118) printf("ask\n");
  else if (parole == 119) printf("me\n");
  else if (parole == 120) printf("how\n");
  else if (parole == 121) printf("I'm\n");
  else if (parole == 122) printf("feeling\n");
  else if (parole == 123) printf("Don't\n");
  else if (parole == 124) printf("tell\n");
  else if (parole == 125) printf("me\n");
  else if (parole == 126) printf("you're\n");
  else if (parole == 127) printf("too\n");
  else if (parole == 128) printf("blind\n");
  else if (parole == 129) printf("to\n");
  else if (parole == 130) printf("see\n");
  else if (parole == 131) printf("Never\n");
  else if (parole == 132) printf("gonna\n");
  else if (parole == 133) printf("give\n");
  else if (parole == 134) printf("you\n");
  else if (parole == 135) printf("up\n");
  else if (parole == 136) printf("Never\n");
  else if (parole == 137) printf("gonna\n");
  else if (parole == 138) printf("let\n");
  else if (parole == 139) printf("you\n");
  else if (parole == 140) printf("down\n");
  else if (parole == 141) printf("Never\n");
  else if (parole == 142) printf("gonna\n");
  else if (parole == 143) printf("run\n");
  else if (parole == 144) printf("around\n");
  else if (parole == 145) printf("and\n");
  else if (parole == 146) printf("desert\n");
  else if (parole == 147) printf("you\n");
  else if (parole == 148) printf("Never\n");
  else if (parole == 149) printf("gonna\n");
  else if (parole == 150) printf("make\n");
  else if (parole == 151) printf("you\n");
  else if (parole == 152) printf("cry\n");
  else if (parole == 153) printf("Never\n");
  else if (parole == 154) printf("gonna\n");
  else if (parole == 155) printf("say\n");
  else if (parole == 156) printf("goodbye\n");
  else if (parole == 157) printf("Never\n");
  else if (parole == 158) printf("gonna\n");
  else if (parole == 159) printf("tell\n");
  else if (parole == 150) printf("a\n");
  else if (parole == 151) printf("lie\n");
  else if (parole == 152) printf("and\n");
  else if (parole == 153) printf("hurt\n");
  else if (parole == 154) printf("you\n");
  else if (parole == 155) printf("Never\n");
  else if (parole == 156) printf("gonna\n");
  else if (parole == 157) printf("give\n");
  else if (parole == 158) printf("you\n");
  else if (parole == 159) printf("up\n");
  else if (parole == 160) printf("Never\n");
  else if (parole == 161) printf("gonna\n");
  else if (parole == 162) printf("let\n");
  else if (parole == 163) printf("you\n");
  else if (parole == 164) printf("down\n");
  else if (parole == 165) printf("Never\n");
  else if (parole == 166) printf("gonna\n");
  else if (parole == 167) printf("run\n");
  else if (parole == 168) printf("around\n");
  else if (parole == 169) printf("and\n");
  else if (parole == 170) printf("desert\n");
  else if (parole == 171) printf("you\n");
  else if (parole == 172) printf("Never\n");
  else if (parole == 173) printf("gonna\n");
  else if (parole == 174) printf("make\n");
  else if (parole == 175) printf("you\n");
  else if (parole == 176) printf("cry\n");
  else if (parole == 177) printf("Never\n");
  else if (parole == 178) printf("gonna\n");
  else if (parole == 179) printf("say\n");
  else if (parole == 180) printf("goodbye\n");
  else if (parole == 181) printf("Never\n");
  else if (parole == 182) printf("gonna\n");
  else if (parole == 183) printf("tell\n");
  else if (parole == 184) printf("a\n");
  else if (parole == 185) printf("lie\n");
  else if (parole == 186) printf("and\n");
  else if (parole == 187) printf("hurt\n");
  else if (parole == 188) printf("you\n");
  else if (parole == 189) printf("We've\n");
  else if (parole == 190) printf("known\n");
  else if (parole == 191) printf("each\n");
  else if (parole == 192) printf("other\n");
  else if (parole == 193) printf("for\n");
  else if (parole == 194) printf("too\n");
  else if (parole == 195) printf("long\n");
  else if (parole == 196) printf("Your\n");
  else if (parole == 197) printf("heart's\n");
  else if (parole == 198) printf("been\n");
  else if (parole == 199) printf("aching,\n");
  else if (parole == 200) printf("but\n");
  else if (parole == 201) printf("you're\n");
  else if (parole == 202) printf("too\n");
  else if (parole == 203) printf("shy\n");
  else if (parole == 204) printf("ton");
  else if (parole == 205) printf("say\n");
  else if (parole == 206) printf("it\n");
  else if (parole == 207) printf("(say\n");
  else if (parole == 208) printf("it)\n");
  else if (parole == 209) printf("Inside,\n");
  else if (parole == 210) printf("we\n");
  else if (parole == 211) printf("both\n");
  else if (parole == 212) printf("know\n");
  else if (parole == 213) printf("what's\n");
  else if (parole == 214) printf("been\n");
  else if (parole == 215) printf("going\n");
  else if (parole == 216) printf("on\n");
  else if (parole == 217) printf("(going\n");
  else if (parole == 218) printf("on)\n");
  else if (parole == 219) printf("We\n");
  else if (parole == 220) printf("know\n");
  else if (parole == 221) printf("the\n");
  else if (parole == 222) printf("game\n");
  else if (parole == 223) printf("and\n");
  else if (parole == 224) printf("we're\n");
  else if (parole == 225) printf("gonna\n");
  else if (parole == 226) printf("play\n");
  else if (parole == 227) printf("it\n");
  else if (parole == 228) printf("I\n");
  else if (parole == 229) printf("just\n");
  else if (parole == 230) printf("wanna\n");
  else if (parole == 231) printf("tell\n");
  else if (parole == 232) printf("you\n");
  else if (parole == 233) printf("how\n");
  else if (parole == 234) printf("I'm\n");
  else if (parole == 235) printf("feeling\n");
  else if (parole == 236) printf("Gotta\n");
  else if (parole == 237) printf("make\n");
  else if (parole == 238) printf("you\n");
  else if (parole == 239) printf("understand\n");
  else if (parole == 240) printf("Never\n");
  else if (parole == 241) printf("gonna\n");
  else if (parole == 242) printf("give\n");
  else if (parole == 243) printf("you\n");
  else if (parole == 244) printf("up\n");
  else if (parole == 245) printf("Never\n");
  else if (parole == 246) printf("gonna\n");
  else if (parole == 247) printf("let\n");
  else if (parole == 248) printf("you\n");
  else if (parole == 249) printf("down\n");
  else if (parole == 250) printf("Never\n");
  else if (parole == 251) printf("gonna\n");
  else if (parole == 252) printf("run\n");
  else if (parole == 253) printf("around\n");
  else if (parole == 254) printf("and\n");
  else if (parole == 255) printf("desert\n");
  else if (parole == 256) printf("you\n");
  else if (parole == 257) printf("Never\n");
  else if (parole == 258) printf("gonna\n");
  else if (parole == 259) printf("make\n");
  else if (parole == 260) printf("you\n");
  else if (parole == 261) printf("cry\n");
  else if (parole == 262) printf("Never\n");
  else if (parole == 263) printf("gonna\n");
  else if (parole == 264) printf("say\n");
  else if (parole == 265) printf("goodbye\n");
  else if (parole == 266) printf("Never\n");
  else if (parole == 267) printf("gonna\n");
  else if (parole == 268) printf("tell\n");
  else if (parole == 269) printf("a\n");
  else if (parole == 270) printf("lie\n");
  else if (parole == 271) printf("and\n");
  else if (parole == 272) printf("hurt\n");
  else if (parole == 273) printf("you\n");
  else if (parole == 274) printf("Never\n");
  else if (parole == 275) printf("gonna\n");
  else if (parole == 276) printf("give\n");
  else if (parole == 277) printf("you\n");
  else if (parole == 278) printf("up\n");
  else if (parole == 279) printf("Never\n");
  else if (parole == 280) printf("gonna\n");
  else if (parole == 281) printf("let\n");
  else if (parole == 282) printf("you\n");
  else if (parole == 283) printf("down\n");
  else if (parole == 284) printf("Never\n");
  else if (parole == 285) printf("gonna\n");
  else if (parole == 286) printf("run\n");
  else if (parole == 287) printf("around\n");
  else if (parole == 288) printf("and\n");
  else if (parole == 289) printf("desert\n");
  else if (parole == 290) printf("you\n");
  else if (parole == 291) printf("Never\n");
  else if (parole == 292) printf("gonna\n");
  else if (parole == 293) printf("make\n");
  else if (parole == 294) printf("you\n");
  else if (parole == 295) printf("cry\n");
  else if (parole == 296) printf("Never\n");
  else if (parole == 297) printf("gonna\n");
  else if (parole == 298) printf("say\n");
  else if (parole == 299) printf("goodbye\n");
  else if (parole == 300) printf("Never\n");
  else if (parole == 301) printf("gonna\n");
  else if (parole == 302) printf("tell\n");
  else if (parole == 303) printf("a\n");
  else if (parole == 304) printf("lie\n");
  else if (parole == 305) printf("and\n");
  else if (parole == 306) printf("hurt\n");
  else if (parole == 307) printf("you\n");
  else if (parole == 308) printf("Never\n");
  else if (parole == 309) printf("gonna\n");
  else if (parole == 310) printf("give\n");
  else if (parole == 311) printf("you\n");
  else if (parole == 312) printf("up\n");
  else if (parole == 313) printf("Never\n");
  else if (parole == 314) printf("gonna\n");
  else if (parole == 315) printf("let\n");
  else if (parole == 316) printf("you\n");
  else if (parole == 317) printf("down\n");
  else if (parole == 318) printf("Never\n");
  else if (parole == 319) printf("gonna\n");
  else if (parole == 320) printf("run\n");
  else if (parole == 321) printf("around\n");
  else if (parole == 322) printf("and\n");
  else if (parole == 323) printf("desert\n");
  else if (parole == 324) printf("you\n");
  else if (parole == 325) printf("Never\n");
  else if (parole == 326) printf("gonna\n");
  else if (parole == 327) printf("make\n");
  else if (parole == 328) printf("you\n");
  else if (parole == 329) printf("cry\n");
  else if (parole == 330) printf("Never\n");
  else if (parole == 331) printf("gonna\n");
  else if (parole == 332) printf("say\n");
  else if (parole == 333) printf("goodbye\n");
  else if (parole == 334) printf("Never\n");
  else if (parole == 335) printf("gonna\n");
  else if (parole == 336) printf("tell\n");
  else if (parole == 337) printf("a\n");
  else if (parole == 338) printf("lie\n");
  else if (parole == 339) printf("and\n");
  else if (parole == 340) printf("hurt\n");
  else if (parole == 341) printf("you\n");
}

// Pile ou face
int PF(){
  srand(time(NULL));
  int pf = rand_max() % 2;
  return pf;
}

// Complexité n^3
void NCube(int lim, int dim){
  if (dim == 7) printf("%d (%d)\n",rand_max(),rand_max());
  else printf("Bienvenue dans une complexité en Theta (n^3)\n");
  int nombres[3] = {rand_max(),rand_max(),rand_max()};
  int liste_nombres[3] = {0,0,0};
  for(int i = 0; i < lim; i += 1){
    for(int j = 0; j < lim; j += 1){
      for(int k = 0; k < lim; k += 1){
        liste_nombres[0] = i;
        liste_nombres[1] = j;
        liste_nombres[2] = k;
        if (dim == 7) printf("%d : %d %d %d\n",rand_max(),nombres[i],nombres[j],nombres[k]);
        else if (dim == 1 || dim <= -4 || dim >= 6) printf("Passage : %d %d %d\n",i,j,k);
        else if (dim == 2) printf("Passage : %d %d %d\n",nombres[i],nombres[j],nombres[k]);
        else if (dim >= 3){
          printf("Passage : ");
          for (int l = 0; l < 3; l+=1){
            // Ici pire que du n^3
            if (liste_nombres[l] == 0) printf("0 ");
            else if (liste_nombres[l] == 1) printf("i ");
            else printf("%di ",liste_nombres[l]);
          }
          printf("\n");
        }
        else if (dim >= -3 && dim <= -1) printf("Passage : -%d -%d -%d\n",i,j,k);
        else if (dim == 0){
          printf("Passage : ");
          for (int l = 0; l < 3; l+=1){
            if (liste_nombres[l] == 0) printf("0 ");
            else if (liste_nombres[l] == 1) printf("0^0 ");
            else if (liste_nombres[l] == 2) printf("0^0+0^0 ");
            else printf("(0^0+0^0)!+0^0 ");
          }
          printf("\n");
        }
        Sleep(1);
      }
    }
  }
}

// MORPION

void afficheMorpion(){
  printf("  ");
  for(int i = 0; i < 3; i+=1){ // Affichage caractères (en haut)
    printf("  %c ",(char) (i + 97));
  }
  printf("\n");
  for(int i = 0; i < 3; i+=1){
    printf("  ");
    for(int j = 0; j < 3; j+=1){
      printf("*-*-");
    }
    printf("*\n%d ",i+1);
    for(int j = 0; j < 3; j+=1){
      printf("| %d ",tab_morp[i][j]);
    }
    printf("|\n");
  }
  printf("  ");
  for(int i = 0; i < 3; i+=1){
    printf("*-*-");
  }
  printf("*\n");
}

bool testCombMorpion(int ligne, int colonne, int player){
  // Vérifie si le joueur a gagné
  // Vérification diagonales
  if ((tab_morp[0][0] == player) && (tab_morp[1][1] == player) && (tab_morp[2][2] == player)) return true;
  else if ((tab_morp[0][2] == player) && (tab_morp[1][1] == player) && (tab_morp[2][0] == player)) return true;
  // Vérification colonnes
  for (int i = 0; i < 3; i += 1){
    if ((tab_morp[0][i] == player) && (tab_morp[1][i] == player) && (tab_morp[2][i] == player)) return true;
  }
  // Vérification lignes
  for (int i = 0; i < 3; i += 1){
    if ((tab_morp[i][0] == player) && (tab_morp[i][1] == player) && (tab_morp[i][2] == player)) return true;
  }
  return false;
}

int MorpionDebile(){
  printf("Bienvenue dans le morpion à IA stupide !\n");
  printf("Vous : 1 ; Ordinateur : 2\n\n");
  bool vict_1 = false, vict_2 = false, valide_ia = false;
  char col; int lig; int compt = 0; // compt : gérer les matchs nuls
  int col_ia = 3, lig_ia = 3;
  afficheMorpion();
  while(!vict_1 && !vict_2 && compt <= 9){
    valide_ia = false;
    printf("Entrer une case (ex : a1) : ");
    while(scanf("%c%d",&col,&lig) != 2 || (int) col-97 < 0 || (int) col-97 > 2 || lig > 3 || lig < 1 || tab_morp[lig-1][(int) col-97] != 0){ // Vérification si case déjà remplie
      printf("SAISIE INCORRECTE !\n");
      while(getchar()!='\n');
    }
    while(getchar()!='\n');
    tab_morp[lig-1][(int) col-97] = 1; // Remplissage de la case
    compt += 1;
    vict_1 = testCombMorpion(lig-1,(int) col-97,1); // Test de la combinaison du joueur (victoire)
    if (vict_1) return 1;
    while (!valide_ia){
      col_ia = rand_max() % 3; lig_ia = rand_max() % 3;
      if (tab_morp[lig_ia][col_ia] == 0){
        valide_ia = true;
        tab_morp[lig_ia][col_ia] = 2;
      }
    }
    compt += 1;
    vict_2 = testCombMorpion(lig_ia,col_ia,2); // Test de la combinaison de l'IA (victoire)
    afficheMorpion();
  }
  if (vict_2) return 2;
  else return 0;
  // 0 : Match nul, 1 : Victoire du joueur, 2 : Victoire de l'IA
}

// MACHINE À SOUS

void affichageResultats(int nb1, int nb2, int nb3){
  int tab[3] = {nb1,nb2,nb3};
  printf("*-*-*-*-*-*-*-*-*-*\n");
  for(int i = 0; i < 3; i += 1){
    printf("| ");
    if (tab[i] == 0) printf("HDD ");
    else if (tab[i] == 1) printf("SSD ");
    else if (tab[i] == 2) printf("RAM ");
    else if (tab[i] == 3) printf("CAC ");
    else if (tab[i] == 4) printf("REG ");
    else if (tab[i] == 5) printf("CPU ");
  }
  printf("|\n");
  printf("*-*-*-*-*-*-*-*-*-*\n");
}

void affichageGainArgent(int argent, int dim){
  if (dim == 2) printf("Argent : +%f\n",argent*money_give);
  else if (dim >= -3 && dim <= -1) printf("Argent : -%f\n",argent*money_give);
  else if (dim == 0) printf("Argent sauvegardé (mis à 0 à la fin si négatif) : +%f\n",argent*money_give);
  else if (dim >= 3){
    if (decouverteJE) printf("Argent de Jean-Eudes : +%f\n",argent*money_give);
    else printf("Argent de ??? : +%f\n",argent*money_give);
  } else if (dim <= -4) printf("Commandes Linux : +%f\n",argent*money_give);
}

void machineASous(int lancers, int dim){
  float argentGagne = 0;
  printf("HDD - ??? - ??? : %f\n",2*money_give);
  printf("SSD - ??? - ??? : %f\n",2*money_give);
  printf("HDD - HDD - ??? : %f\n",5*money_give);
  printf("SSD - SDD - ??? : %f\n",5*money_give);
  printf("HDD - HDD - HDD : %f\n",10*money_give);
  printf("SSD - SDD - SDD : %f\n",10*money_give);
  printf("RAM - RAM - RAM : %f\n",25*money_give);
  printf("CAC - CAC - CAC : %f\n",50*money_give);
  printf("REG - REG - REG : %f\n",100*money_give);
  printf("CPU - CPU - CPU : %f\n\n",5000*money_give);
  for(int i = 0; i < lancers; i+=1){
    char car = '\0';
    // Affichage de l'argent actuel
    if (dim == 2) printf("Argent (compte) : %f\n",argent.r+argentGagne);
    else if (dim >= -3 && dim <= -1) printf("Argent (compte) : %f\n",argent.r-argentGagne);
    else if (dim == 0) {
      if (argentGagne >= 0) printf("Argent sauvegardé (compte) : %f\n",argent_save.r+argentGagne);
      else printf("Argent sauvegardé (compte) : %f\n",argent_save.r);
    } else if (dim >= 3){
      if (decouverteJE) printf("Argent de Jean-Eudes (compte) : %f\n",argent.i+argentGagne);
      else printf("Argent de ??? : %f\n",argent.i+argentGagne);
    } else if (dim <= -4) printf("Commandes Linux (compte) : %f\n",argent.c+argentGagne);
    printf("Il reste %d lancers\n",lancers-i);
    // Lancer
    if (dim <= -1 && dim >= -3) printf("Appuyez sur r pour lancer (Argent : +%f) : ",3*(2-money_give));
    else printf("Appuyez sur r pour lancer (Argent : %f) : ",-3*(2-money_give));
    while(scanf("%c",&car) != 1 || car != 'r'){
      printf("Appuyez sur r pour lancer : ");
      while(getchar()!='\n');
    }
    while(getchar()!='\n');
    // Tirage de la machine à sous
    argentGagne -= 3*(2-money_give);
    int rand_1 = rand_max()%6;
    int rand_2 = rand_max()%6;
    int rand_3 = rand_max()%6;
    affichageResultats(rand_1,rand_2,rand_3);
    if (rand_1 == 5 && rand_2 == 5 && rand_3 == 5){
      affichageGainArgent(5000, dim);
      argentGagne += 5000*money_give;
    } else if (rand_1 == 4 && rand_2 == 4 && rand_3 == 4){
      affichageGainArgent(100, dim);
      argentGagne += 100*money_give;
    } else if (rand_1 == 3 && rand_2 == 3 && rand_3 == 3){
      affichageGainArgent(50, dim);
      argentGagne += 50*money_give;
    } else if (rand_1 == 2 && rand_2 == 2 && rand_3 == 2){
      affichageGainArgent(25, dim);
      argentGagne += 25*money_give;
    } else if (rand_1 == 1 && rand_2 == 1 && rand_3 == 1){
      affichageGainArgent(10, dim);
      argentGagne += 10*money_give;
    } else if (rand_1 == 0 && rand_2 == 0 && rand_3 == 0){
      affichageGainArgent(10, dim);
      argentGagne += 10*money_give;
    } else if (rand_1 == 1 && rand_2 == 1){
      affichageGainArgent(5, dim);
      argentGagne += 5*money_give;
    } else if (rand_1 == 0 && rand_2 == 0){
      affichageGainArgent(5, dim);
      argentGagne += 5*money_give;
    } else if (rand_1 == 1){
      affichageGainArgent(2, dim);
      argentGagne += 2*money_give;
    } else if (rand_1 == 0){
      affichageGainArgent(2, dim);
      argentGagne += 2*money_give;
    } else {
      affichageGainArgent(0, dim);
    }
  }
  // Affichage de l'argent après tirage
  if (dim == 2) printf("Argent (compte) : %f\n",argent.r+argentGagne);
  else if (dim >= -3 && dim <= -1) printf("Argent (compte) : %f\n",argent.r-argentGagne);
  else if (dim == 0) {
      if (argentGagne >= 0) printf("Argent sauvegardé (compte) : %f\n",argent_save.r+argentGagne);
      else printf("Argent sauvegardé (compte) : %f\n",argent_save.r);
  } else if (dim >= 3){
    if (decouverteJE) printf("Argent de Jean-Eudes (compte) : %f\n",argent.i+argentGagne);
    else printf("Argent de ??? (compte) : %f\n",argent.i+argentGagne);
  } else if (dim <= -4) printf("Commandes Linux (compte) : %f\n",argent.c+argentGagne);
  if (dim == 2) argent.r += argentGagne;
  else if (dim >= -3 && dim <= -1) argent.r -= argentGagne;
  else if (dim == 0 && argentGagne >= 0) argent_save.r += argentGagne;
  else if (dim >= 3) argent.i += argentGagne;
  else if (dim <= -4) argent.c += argentGagne;
}

int minageBitcoin(int dim_a){
  // Minage du Bitcoin (pour dimension de Schrödinger)
  int dim = dim_a;
  printf("Vous avez miné %d bitcoin(s) !\n",pioche);
  bitcoins += pioche;
  if (rand_max() % 20 == 0) {
    printf("COUP CRITIQUE ! Vous avez miné %d bitcoin(s) supplémentaire(s) !\n",pioche);
    bitcoins += pioche;
  }
  if (rand_max() % 50 == 49 && chance > 1) {
    chance -= 1;
    printf("Votre chance de miner un bitcoin est augmentée (1 chance sur %d).\n",chance);
  }
  if (bitcoins >= 10 && pioche == 1) {
    printf("Vous changez de pioche ! Vous minerez désormais 2 bitcoins par minage !\n");
    pioche = 2;
  } else if (bitcoins >= 25 && pioche <= 2){
    printf("Vous changez de pioche ! Vous minerez désormais 5 bitcoins par minage !\n");
    pioche = 5;
  } else if (bitcoins >= 100 && pioche <= 5){
    printf("Vous changez de pioche ! Vous minerez désormais 10 bitcoins par minage !\n");
    pioche = 10;
  } else if (bitcoins >= 250 && pioche <= 10){
    printf("Vous changez de pioche ! Vous minerez désormais 25 bitcoins par minage !\n");
    pioche = 25;
  } else if (bitcoins >= 500 && pioche <= 25){
    printf("Vous changez de pioche ! Vous minerez désormais 50 bitcoins par minage !\n");
    pioche = 50;
  } else if (bitcoins >= 2000 && pioche <= 50){
    printf("Vous changez de pioche ! Vous minerez désormais 100 bitcoins par minage !\n");
    pioche = 100;
  } else if (bitcoins >= 5000 && pioche <= 100){
    printf("Vous changez de pioche ! Vous minerez désormais 250 bitcoins par minage !\n");
    pioche = 250;
  } else if (bitcoins >= 20000 && pioche <= 250){
    printf("Vous changez de pioche ! Vous minerez désormais 1000 bitcoins par minage !\n");
    pioche = 1000;
  } else if (bitcoins >= 100000 && pioche <= 1000){
    printf("Vous changez de pioche ! Vous minerez désormais 4000 bitcoins par minage !\n");
    pioche = 4000;
  } else if (bitcoins >= 500000){
    dim = 12;
  }
  if (bitcoins >= 1900 && dim_a != -8){
    if (dim_a != -8){
      printf("Vous êtes téléporté en dimension de Schrödinger !\n");
      dim = -8;
      if (chance >= 2) chance = 1;
    }
  }
  printf("\n");
  return dim;
}

void cartesTirage(int cartes_rand, int dim_a){
  // Système de cartes à collectionner
  if (dim_a == 2){
    if (cartes_rand < 200){
      printf("Carte N°1 - Dimension non initialisée\n");
      printf("'int a;'\n");
      printf("Une bel entier non initialisé !\n\n");
      cartes[0] += 1;
    } else if (cartes_rand < 400){
      printf("Carte N°2 - Dimension non initialisée\n");
      printf("'char b;'\n");
      printf("Un beau caractère non initialisé !\n\n");
      cartes[1] += 1;
    } else if (cartes_rand < 600){
      printf("Carte N°3 - Dimension non initialisée\n");
      printf("'int c[5];'\n");
      printf("Un beau tableau d'entiers non initialisés !\n\n");
      cartes[2] += 1;
    } else if (cartes_rand < 800){
      printf("Carte N°4 - Dimension non initialisée\n");
      printf("'bool d;'\n");
      printf("Un beau booléen non initialisée à ne pas utiliser dans un if sous peine de choses bizarres !\n\n");
      cartes[3] += 1;
    } else if (cartes_rand < 1000){
      printf("Carte N°5 - Dimension non initialisée\n");
      printf("'int* e;'\n");
      printf("Un beau pointeur non initialisé ! Parfait pour faire planter des choses !\n\n");
      cartes[4] += 1;
    } else {
      printf("Carte S1 - Dimension non initialisée\n");
      printf("Vous pourrez utiliser cette carte pour gagner quand vous atteindrez une case Utilisation !\n\n");
      cartes[25] += 1;
    }
  } else if (dim_a >= -3 && dim_a <= -1){
    if (cartes_rand < 200){
      printf("Carte N°6 - Dimension négative\n");
      printf("'printf(c[-1]);'\n");
      printf("Le beau retour au Python !\n\n");
      cartes[5] += 1;
    } else if (cartes_rand < 400){
      printf("Carte N°7 - Dimension négative\n");
      printf("'unsigned int g = -1;'\n");
      printf("Le fameux entier positif négatif !\n\n");
      cartes[6] += 1;
    } else if (cartes_rand < 600){
      printf("Carte N°8 - Dimension négative\n");
      printf("'int* h = -1;'\n");
      printf("La fameuse adresse mémoire négative !\n\n");
      cartes[7] += 1;
    } else if (cartes_rand < 800){
      printf("Carte N°9 - Dimension négative\n");
      printf("'int longueur = -1;'\n");
      printf("Je crois que quelqu'un a oublié les valeurs absolues !\n\n");
      cartes[8] += 1;
    } else if (cartes_rand < 1000){
      printf("Carte N°10 - Dimension négative\n");
      printf("'int surface = -1;'\n");
      printf("Une belle surface négative issue de la longueur négative !\n\n");
      cartes[9] += 1;
    } else {
      printf("Carte S2 - Dimension négative\n");
      printf("Vous pourrez utiliser cette carte pour gagner quand vous atteindrez une case Utilisation !\n\n");
      cartes[26] += 1;
    }
  } else if (dim_a == 0){
    if (cartes_rand < 200){
      printf("Carte N°11 - Dimension nulle\n");
      printf("'int i[5] = {0};'\n");
      printf("Un beau tableau bien initialisé avec des 0 !\n\n");
      cartes[10] += 1;
    } else if (cartes_rand < 400){
      printf("Carte N°12 - Dimension nulle\n");
      printf("'int* j = NULL;'\n");
      printf("Ah le beau pointeur nul !\n\n");
      cartes[11] += 1;
    } else if (cartes_rand < 600){
      printf("Carte N°13 - Dimension nulle\n");
      printf("'int cesttropbienunbooleen = false;'\n");
      printf("Un identifiant magnifique !\n\n");
      cartes[12] += 1;
    } else if (cartes_rand < 800){
      printf("Carte N°14 - Dimension nulle\n");
      printf("char k = ';';\n");
      printf("Un point-virgule.\n\n");
      cartes[13] += 1;
    } else if (cartes_rand < 1000){
      printf("Carte N°15 - Dimension nulle\n");
      printf("'printf(0^0);'\n\n");
      printf("Le beau printf sans guillemets !\n\n");
      cartes[14] += 1;
    } else {
      printf("Carte S3 - Dimension nulle\n");
      printf("Vous pourrez utiliser cette carte pour gagner quand vous atteindrez une case Utilisation !\n\n");
      cartes[27] += 1;
    }
  } else if (dim_a >= 3){
    if (cartes_rand < 200){
      printf("Carte N°16 - Dimension imaginaire\n");
      printf("'sqrt(-1)'\n");
      printf("Dans la dimension physicienne, cela donnerait j !\n\n");
      cartes[15] += 1;
    } else if (cartes_rand < 400){
      printf("Carte N°17 - Dimension imaginaire\n");
      printf("'e^(i*pi) = -1'\n");
      printf("Oui.\n\n");
      cartes[16] += 1;
    } else if (cartes_rand < 600){
      printf("Carte N°18 - Dimension imaginaire\n");
      printf("'sqrt(-4) = 2i'\n");
      printf("Certains voulaient mettre MPsqrt(-4) pour MP2I sur les sweats !\n\n");
      cartes[17] += 1;
    } else if (cartes_rand < 800){
      printf("Carte N°19 - Dimension imaginaire\n");
      printf("'(1+i)^4 = -4'\n");
      printf("Encore une fois, oui.\n\n");
      cartes[18] += 1;
    } else if (cartes_rand < 1000){
      printf("Carte N°20 - Dimension imaginaire\n");
      printf("'sqrt(i)'\n");
      printf("Une belle racine carrée de complexe, c'est beau !\n\n");
      cartes[19] += 1;
    } else {
      printf("Carte S4 - Dimension imaginaire\n");
      printf("Vous pourrez utiliser cette carte pour gagner quand vous atteindrez une case Utilisation !\n\n");
      cartes[28] += 1;
    }
  } else if (dim_a <= -4){
    if (cartes_rand < 200){
      printf("Carte N°21 - Dimension Carcenac\n");
      printf("'++'\n");
      printf("Un bel élément du C qui vous ravira !\n\n");
      cartes[20] += 1;
    } else if (cartes_rand < 400){
      printf("Carte N°22 - Dimension Carcenac\n");
      printf("double free\n");
      printf("Cela aurait été mieux avec un seul...\n\n");
      cartes[21] += 1;
    } else if (cartes_rand < 600){
      printf("Carte N°23 - Dimension Carcenac\n");
      printf("'--((a++)--)'\n");
      printf("Le plaisir que vous devez avoir en voyant cela !\n\n");
      cartes[22] += 1;
    } else if (cartes_rand < 800){
      printf("Carte N°24 - Dimension Carcenac\n");
      printf("'printf(tableau)'\n");
      printf("Un printf... sans guillements... d'un tableau !\n\n");
      cartes[23] += 1;
    } else if (cartes_rand < 1000){
      printf("Carte N°25 - Dimension Carcenac\n");
      printf("malloc sans free\n");
      printf("On adore ne pas rendre au tas !\n\n");
      cartes[24] += 1;
    } else {
      printf("Carte S5 - Dimension Carcenac\n");
      printf("Vous pourrez utiliser cette carte pour gagner quand vous atteindrez une case Utilisation !\n\n");
      cartes[29] += 1;
    }
  }
  for(int i = 0; i < 30; i += 1){
    if (cartes[i] > 0){
      if (i < 25) printf("| %d ",i+1);
      else printf("| S%d ",i-24);
    }
  }
  printf("|\n\n");
}

int utilisationCarteSecrete(int dim_a){
  // Utilisation des cartes secrètes
  int dim = dim_a;
  bool carteSecrete = false;
  for (int i = 25; i < 30; i += 1){
    if (cartes[i] != 0){
      // Carte secrète utilisable
      carteSecrete = true;
      for (int j = 0; j < cartes[i]; j+=1){
        if (i == 25) printf("S1 : Dimension non initialisée\n");
        else if (i == 26) printf("S2 : Dimension négative\n");
        else if (i == 27) printf("S3 : Dimension nulle\n");
        else if (i == 28) printf("S4 : Dimension imaginaire\n");
        else if (i == 29) printf("S5 : Dimension Carcenac\n");
      }
    }
  }
  if (carteSecrete){ // Carte secrète à utiliser
    int num_carte = 0;
    if (dim_a == 7) printf("%d (S?) : ",rand_max());
    else printf("Choisissez la carte secrète à utiliser (S?) : ");
    while(scanf("%d",&num_carte) != 1 || num_carte < 1 || num_carte > 5 || cartes[num_carte+24] == 0){
      if (dim_a == 7) printf("%d (S?) : ",rand_max());
      else printf("Choisissez la carte secrète à utiliser (S?) : ");
      while(getchar()!='\n');
    }
    while(getchar()!='\n');
    cartes[num_carte+24] -= 1;
    if (dim_a == 7) printf("%d !\n",rand_max());
    else printf("Vous utilisez une carte secrète !\n");
    dim = 10;
  } else if (dim_a == 7) { // Pas de carte secrète
    printf("%d !\n",rand_max());
    printf("%d %f %d\n\n",rand_max(),100*money_give,rand_max());
    argent_save.r += 100*money_give;
  } else { // Pas de carte secrète
    printf("Vous ne pouvez pas utiliser de carte secrète actuellement !\n");
    printf("Vous recevez donc %f d'argent sauvegardé en compensation\n\n",100*money_give);
    argent_save.r += 100*money_give;
  }
  return dim;
}

// CHAUD-FROID

char chaud_froid[8][8] = {
  {'\0','\0','\0','\0','\0','\0','\0','\0'},
  {'\0','\0','\0','\0','\0','\0','\0','\0'},
  {'\0','\0','\0','\0','\0','\0','\0','\0'},
  {'\0','\0','\0','\0','\0','\0','\0','\0'},
  {'\0','\0','\0','\0','\0','\0','\0','\0'},
  {'\0','\0','\0','\0','\0','\0','\0','\0'},
  {'\0','\0','\0','\0','\0','\0','\0','\0'},
  {'\0','\0','\0','\0','\0','\0','\0','\0'}
};

void afficheChaudFroid(){
  printf("     1   2   3   4   5   6   7   8\n");
  printf("   *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*\n");
  for (int i = 0; i < 8; i += 1){
    printf("%c  ",(char) i+97);
    for (int j = 0; j < 8; j += 1){
      if (chaud_froid[i][j] == '\0') printf("|   ");
      else printf("| %c ",chaud_froid[i][j]);
    }
    printf("|\n");
    printf("   *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*\n");
  }
  printf("\n");
}

char calculchaudFroid(int x_pc, int y_pc, int x_ut_c, char y_ut_c){
  char resultat = '\0';
  int x_ut = x_ut_c - 1;
  int y_ut = (int) y_ut_c - 97;
  if (x_ut == x_pc && y_ut == y_pc) resultat = 'V';
  else if (abs(x_ut - x_pc) <= 1 && abs(y_ut - y_pc) <= 1) resultat = 'B'; // Moins de 1 case de distance : Brûlant
  else if (abs(x_ut - x_pc) <= 2 && abs(y_ut - y_pc) <= 2) resultat = 'C'; // Moins de 2 cases de distance : Chaud
  else if (abs(x_ut - x_pc) <= 3 && abs(y_ut - y_pc) <= 3) resultat = 'T'; // Moins de 3 cases de distance : Tiède
  else if (abs(x_ut - x_pc) <= 5 && abs(y_ut - y_pc) <= 5) resultat = 'F'; // Moins de 5 cases de distance : Froid
  else resultat = 'G'; // Glacé
  return resultat;
}

void chaudFroid(int dim){
  // Réinitialisation du chaud-froid
  for (int i = 0; i < 8; i += 1){
    for (int j = 0; j < 8; j += 1){
      chaud_froid[i][j] = '\0';
    }
  }
  bool victoire = false;
  if (dim == 7) printf("%d ! %d ++ %d !\n",rand_max(),rand_max(),rand_max());
  else printf("Bienvenue au chaud-froid ! Vous devez retrouver le ++ pour l'arrêter !\n");
  int x = rand_max() % 8;
  int y = rand_max() % 8;
  while (!victoire){
    afficheChaudFroid(); // Affichage de la grille
    if (dim == 7){
      printf("V : %d   B : %d    C : %d\n",rand_max(),rand_max(),rand_max());
      printf("T : %d   F : %d     G : %d\n",rand_max(),rand_max(),rand_max());  
    } else {
      printf("V : Victoire   B : Brûlant    C : Chaud\n");
      printf("T : Tiède      F : Froid      G : Glacé\n");
    }
    int coord_x = 0;
    char coord_y = '\0';
    if (dim == 7) printf("%d (ex : a1) : ",rand_max());
    else printf("Indiquer les coordonnées (ex : a1) : ");
    while (scanf("%c%d",&coord_y,&coord_x) != 2){
      while(getchar()!='\n');
      if (dim == 7) printf("%d (ex : a1) : ",rand_max());
      else printf("Indiquer les coordonnées (ex : a1) : ");
    }
    while(getchar()!='\n');
    char result = calculchaudFroid(x, y, coord_x, coord_y); // Calcul de la lettre
    chaud_froid[(int) coord_y - 97][coord_x - 1] = result; 
    if (result == 'V') {
      // Victoire
      afficheChaudFroid();
      victoire = true;
      if (dim == 7) printf("%d ! \n\n",rand_max());
      else printf("Vous avez gagné !\n\n");
    } else {
      printf("\n\n");
    }
  }
}

// CASES ET PORTAILS

int changementDim(int dim_a){
  int dim = dim_a;
  int rand_dim = rand_max() % 1600;
  int rand_dim_1 = rand_max() % 1000;
  if (dim_a == 1){ // Dimension classique
    if (rand_dim_1 < sommeDep(250,count_dep)){
      printf("Vous avez beaucoup de chance ! Le portail vers la dimension non initialisée est actuellement fermé.\n\n");
    } else if (rand_dim >= 990 + sommeDep(5,count_dep)) {
      printf("Le portail a subi un bug ! Vous passez en dimension négative !\n\n");
      dim = -1;
    } else {
      printf("Vous passez en dimension non initialisée !\n\n");
      dim = 2;
    }
  } else if (dim_a == 2){ // Dimension non initialisée
    if (rand_dim >= 1300){
      printf("Le portail a subi un bug ! Vous passez en dimension négative !\n\n");
      dim = -1;
    } else if (rand_dim >= 800 - sommeDep(250,count_dep)){
      printf("Vous retournez en dimension classique !\n\n");
      dim = 1;
    } else {
      printf("Le portail est fermé pour travaux (ça me rappelle la salle 315) !\n\n");
    }
  } else if (dim_a == 0) { // Dimension nulle
    if (rand_dim_1 >= 900 + sommeDep(50,count_dep)){
      printf("Vous retournez en dimension non initialisée !\n\n");
      dim = 2;
    } else if (rand_dim < 20 + sommeDep(20,count_dep)){
      printf("Vous êtes téléporté en dimension Carcenac !\n\n");
      dim = -4;
    } else if (rand_dim >= 100 & rand_dim < 102 - sommeDep(1, count_dep)) { 
      printf("Pas de chance... Vous êtes téléporté en dimension négative !\n\n");
      dim = -1;
    } else {
      printf("Le portail est fermé par la forme indéterminée 0/0 !\n\n");
    }
  } else if (dim_a == -1){ // Dimension négative
    if (rand_dim_1 >= 900 - sommeDep(200,count_dep)){
      printf("Le portail a été redémarré, vous retournez en dimension non initialisée !\n\n");
      dim = 2;
    } else if (rand_dim_1 >= 650 - sommeDep(25,count_dep) && rand_dim_1 < 700){
      printf("Vous êtes téléporté dans la dimension de redirection !\n\n");
      dim = 5;
    } else if (rand_dim_1 >= 400 && rand_dim_1 < 420 + sommeDep(20,count_dep)){
      printf("Vous êtes téléporté en dimension Carcenac !\n\n");
      dim = -4;
    } else if (rand_dim_1 < 100 - sommeDep(20,count_dep)){
      printf("Vous êtes téléporté dans les backrooms !\n\n");
      dim = -2;
    } else {
      printf("Le portail est fermé par un bug !\n\n");
    }
  } else if (dim_a == -2){ // Backrooms
    if (rand_dim_1 < 5 + sommeDep(5,count_dep)){
      printf("Vous passez au niveau 1 (Jean-Eudes) !\n\n");
      dim = 4;
    } else if (rand_dim_1 >= 670 - sommeDep(15,count_dep) && rand_dim_1 < 700){
      printf("Vous êtes téléporté dans la dimension de redirection !\n\n");
      dim = 5;
    } else if (rand_dim_1 >= 995 - sommeDep(5,count_dep)){
      printf("Vous passez au niveau -1 (M. Carcenac) !\n\n");
      dim = -5;
    } else {
      printf("Le portail est fermé car vous êtes dans les Backrooms !\n\n");
    }
  } else if (dim_a == -3){ // Prison de M. Jacob
    if (rand_dim_1 == 0){
      if (!professeurC){
        printf("Vous avez sauvé le professeur C !\n\n");
        professeurC = true;
      }
      printf("Bravo !!! Vous êtes transféré dans les Backrooms de M. Carcenac !\n\n");
      dim = -5;
    } else if (rand_dim_1 >= 690 - sommeDep(5,count_dep) && rand_dim_1 < 700){
      printf("Vous êtes téléporté dans la dimension de redirection !\n\n");
      dim = 5;
    } else {
      printf("Le portail n'existe pas car vous êtes dans une cellule de la prison de M. Jacob !\n\n");
    }
  } else if (dim_a == 3){ // Dimension imaginaire
    if (rand_dim_1 >= 950 + sommeDep(25,count_dep)){
      printf("Vous retournez en dimension nulle !\n\n");
      dim = 0;
    } else if (rand_dim_1 < 10 - sommeDep(5,count_dep)){
      printf("Vous retournez en dimension non initialisée !\n\n");
      dim = 2;
    } else if (rand_dim_1 >= 10 && rand_dim_1 < 12 - sommeDep(1, count_dep)) {
      printf("Vous allez en dimension négative !\n\n");
      dim = -1;
    } else if (rand_dim_1 >= 20 && rand_dim_1 < 30 + sommeDep(5,count_dep)) { 
      printf("Vous allez en dimension Carcenac !\n\n");
      dim = -4;
    } else {
      printf("Le portail est imaginaire, vous ne pouvez donc pas l'atteindre !\n\n");
    }
  } else if (dim_a == 4){ // Backrooms de Jean-Eudes
    if (rand_dim_1 >= 960 - sommeDep(20,count_dep)){
      printf("Vous êtes emmené dans les backrooms de M. Carcenac !\n\n");
      dim = -5;
    } else {
      printf("Le portail n'existe pas, car il est imaginaire !\n\n");
    }
  } else if (dim == -4){ // Dimension Carcenac
    if (rand_dim_1 >= 980 + sommeDep(10,count_dep)){
      printf("Vous retournez en dimension négative !\n\n");
      dim = -1;
    } else if (rand_dim_1 < 20 - sommeDep(10,count_dep)){
      printf("Vous retournez en dimension non initialisée !\n\n");
      dim = 2;
    } else {
      printf("Le portail est bloqué par un point-virgule manquant !\n\n");
    }
  } else if (dim == -5){ // Backrooms de M. Carcenac
    printf("Le portail est fermé car vous êtes dans les Backrooms !\n\n");
  } else if (dim == -6){ // Local technique
    printf("Le portail n'existe pas dans ce dépotoir...\n\n");
  } else if (dim == -7){ // Backrooms de M. Faure
    printf("Le portail n'a pas été acheté car c'est trop cher !\n\n");
  } else if (dim == 6){ // Backrooms de M. Jansou
    printf("Le portail n'est pas dérivable !\n\n");
  } else if (dim == 7){ // Fond du trou
    printf("%d !\n\n",rand_max());
  } else if (dim == -8){ // Dimension de Schrödinger
    printf("Il y a une incertitude quant à votre position. Vous devez rester là.\n\n");
  } else if (dim == 5){ // Dimension de redirection
    printf("Vous attendez là et vous vous calmez manuellement.\n\n");
  } 
  return dim;
}

int changementDim2(){
  int dim_rand = rand_max() % 1000;
  int dim = 5;
  if (dim_rand < 500 - 100*count_dep){
    if (rand_max() % 8 == 0){
      printf("Vous êtes téléporté dans les Backrooms !\n\n");
      dim = -2;
    } else {
      printf("Vous êtes téléporté en dimension négative !\n\n");
      dim = -1;
    }
  } else if (dim_rand < 990 - 5*count_dep){
    int dim_rand_2 = rand_max()%10;
    if (dim_rand_2 == 0 || dim_rand_2 == 5){
      printf("Vous êtes téléporté dans les backrooms de M. Faure !\n\n");
      dim = -7;
    } else if (dim_rand_2 == 1){
      printf("Vous êtes téléporté dans les backrooms de M. Carcenac !\n\n");
      dim = -5;
    } else if (dim_rand_2 == 2 || dim_rand_2 == 6){
      printf("Vous êtes téléporté dans les backrooms de Jean-Eudes !\n\n");
      dim = 4;
    } else if (dim_rand_2 == 3 || dim_rand_2 == 7 || dim_rand_2 == 8){
      printf("Vous êtes téléporté dans les backrooms de M. Jansou !\n\n");
      dim = 6;
    } else if (dim_rand_2 == 4 || dim_rand_2 == 9){
      printf("Vous êtes téléporté au fond du trou !\n\n");
      dim = 7;
    }
  } else {
    printf("Vous êtes téléporté en dimension de Schrödinger !\n\n");
    dim = -8;
    if (chance >= 2) chance = 1;
  }
  return dim;
}

int case1(int* ligne, int* colonne, int cases, int* recule, int* avance, int dim_a, int pas){
  int dim = dim_a;
  int case_rand = (rand_max() % 62) + 1;
  if (dim == -4 && *ligne == 4 && *colonne == 3){
    *ligne = 0; *colonne = 0;
  } else if ((cases == 0 && dim_a == 2) || (cases == 0 && dim_a == -1)){ // Case 0 (dimensions non initialisée, négative)
    printf("Vous passez en dimension nulle !\n\n");
    dim = 0;
  } else if (rand_max() % 100 == 0 && dim_a == 0){ // Case 0 (dimension nulle)
    printf("Vous passez en dimension imaginaire !\n\n");
    dim = 3;
  } else if (cases == 0){ // Case 0 (autres dimensions)
    dim = case1(ligne,colonne,(rand_max() % 63)+1,recule,avance,dim_a,pas); // Choisit une action aléatoire d'une case entre 1 et 63
  } else if (cases == 1){ // Case 01
    dim = changementDim(dim_a);
    if (dim == dim_a){
      dim = case1(ligne,colonne,case_rand,recule,avance,dim_a,pas);
    }
  } else if (cases == 2){ // Case 02
    dim = changementDim(dim_a);
    if (dim == dim_a){
      dim = case1(ligne,colonne,case_rand,recule,avance,dim_a,pas);
    }
  } else if (cases == 3){ // Case 03
    dim = changementDim(dim_a);
    if (dim == dim_a){
      dim = case1(ligne,colonne,case_rand,recule,avance,dim_a,pas);
    }
  } else if (cases == 4){ // Case 04
    if (dim_a == 1){ // Dimension classique
      printf("Vous reculez de %d cases !\n\n",pas+2);
      *recule = pas+2;
    } else if (dim_a == 2){ // Dimension non initialisée
      printf("Amélioration : -0.5 seconde d'attente\n\n");
      second_less += 0.5;
    } else if (dim_a >= -3 && dim_a <= -1){ // Dimensions négatives
      printf("Amélioration : +1 pourcent par don d'argent\n\n");
      money_give *= 1.01;
    } else if (dim_a == 0){ // Dimension nulle
      printf("Tous les argents sont multipliés par 1.01 !\n\n");
      argent.r += fabs(argent.r) * 1.01;
      argent.i += fabs(argent.i) * 1.01;
      argent.j += fabs(argent.j) * 1.01;
      argent.c += fabs(argent.c) * 1.01;
    } else if (dim_a >= 6 || dim_a == -8){ // Dimensions M. Jansou
      if (dim_a == 7) printf("%d%d%d 1.5 !\n\n",rand_max(),rand_max(),rand_max());
      else printf("Tous les argents sont multipliés par 1.5 !\n\n");
      argent.r += fabs(argent.r) * 1.5;
      argent.i += fabs(argent.i) * 1.5;
      argent.j += fabs(argent.j) * 1.5;
      argent.c += fabs(argent.c) * 1.5;
    } else if (dim_a <= -6){ // Dimensions M. Faure
      printf("Tous les argents sont multipliés par 1.25 !\n\n");
      argent.r += fabs(argent.r) * 1.25;
      argent.i += fabs(argent.i) * 1.25;
      argent.j += fabs(argent.j) * 1.25;
      argent.c += fabs(argent.c) * 1.25;
    } else if (dim_a <= -4){ // Dimensions Carcenac
      printf("Votre argent négatif est transformé en positif !\n\n");
      if (argent.r < 0) argent.r *= -1;
      if (argent.i < 0) argent.i *= -1;
      if (argent.j < 0) argent.j *= -1;
      if (argent.c < 0) argent.c *= -1;
    } else if (dim_a >= 3){ // Dimensions imaginaires
      if (argent.r < 0){
        printf("Argent : +%f\n\n",-argent.r * 0.1);
        argent.r += (-argent.r * 0.1);
      } else if (argent.r == 0){
        printf("Argent : +1000\n\n");
        argent.r = 1000;
      } else {
        printf("Tous vos comptes sont sauvegardés !\n\n");
        argent_save.r = argent.r;
        argent.r = 0;
        argent_save.i = argent.i;
        argent.i = 0;
        argent_save.j = argent.j;
        argent.j = 0;
        argent_save.c = argent.c;
        argent.c = 0;
      }
    }
  } else if (cases == 5){ // Case 05
    dim = changementDim(dim_a);
    if (dim == dim_a){
      dim = case1(ligne,colonne,case_rand,recule,avance,dim_a,pas);
    }
  } else if (cases == 6){ // Case 06
    MalusTP(15,dim_a);
    if (dim_a == 1){
      printf("Vous reculez d'une case !\n\n");
      *recule = 1;
    }
  } else if (cases == 7){ // Case 07
    // Dimension classique : +2 cases ; Autres dimensions : dimension aléatoire
    if (dim_a == 1) {
      printf("Vous avancez de 2 cases (environ) !\n\n");
      *avance = (rand_max() % 2) + 1;
    } else if (dim_a == 2) {
      if (rand_max() % 1000 < 950 - sommeDep(50,count_dep)){
        printf("L'aléatoire a décidé que vous devez être téléporté en dimension négative !\n\n");
        dim = -1;
      } else {
        printf("L'aléatoire a décidé que vous devez être téléporté en dimension nulle !\n\n");
        dim = 0;
      }
    } else if (dim_a == 0 || dim_a == -2 || dim_a == 6 || dim_a <= -7) { 
      printf("L'aléatoire a décidé... vous restez là !\n\n");
    } else if (dim_a <= -6) {
      printf("L'aléatoire a décidé : vous êtes téléporté dans les backrooms de M. Faure !\n\n");
      dim = -7;
    } else if (dim_a <= -4) {
      printf("L'aléatoire a décidé : vous êtes téléporté dans les backrooms de M. Carcenac !\n\n");
      dim = -5;
    } else if (dim_a == 7) { 
      printf("%d : %d !\n\n",rand_max(),rand_max());
    } else if (dim_a >= 3) { 
      printf("L'aléatoire a décidé : vous êtes téléporté dans les backrooms de Jean-Eudes !\n\n");
      dim = 4;
    } else {
      if (dim_a == -3 && !professeurC){
        printf("Vous avez sauvé le professeur C !\n");
        professeurC = true;
      }
      printf("L'aléatoire a décidé : vous allez dans les Backrooms !\n\n");
      dim = -2;
    }
  } else if (cases == 8){ // Case 08
    if (dim_a >= -3 && dim_a <= -1){
      printf("-1 banane.\n");
      printf("(Maxime a été demandé par cette case)\n\n");
    } else if (dim_a == 2){
      printf("%d bananes.\n",rand_max());
      printf("(Cette case a été déclarée par Maxime)\n\n");
    } else if (dim_a == 0){
      printf("0 banane.\n");
      printf("(Cette case a été multipliée par 0 par Maxime)\n\n");
    } else if (dim_a == -8){
      printf("Un nombre indéterminé de bananes.\n");
      printf("(Cette case a été indéterminée par Maxime)\n\n");
    } else if (dim_a <= -6){
      printf("p bananes.\n");
      printf("(Cette case a été transformée en Laplace par Maxime)\n\n");
    } else if (dim_a >= 6){
      if (dim_a == 7) printf("Oh un retour temporaire du langage !\n");
      printf("2*3=5 bananes.\n");
      printf("(Cette case a été i^2=1 par Maxime)\n\n");
    } else if (dim_a <= -4){
      printf("pwd bananes.\n");
      printf("(Cette case a été commandée par Maxime)\n\n");
    } else if (dim_a >= 3){
      printf("i banane.\n");
      printf("(Cette case a été imaginée par Maxime)\n\n");
    } else {
      printf("Une banane.\n");
      printf("(Cette case a été demandée par Maxime)\n\n");
    }
  } else if (cases == 9){ // Case 09
    dim = changementDim(dim_a);
    if (dim == dim_a){
      dim = case1(ligne,colonne,case_rand,recule,avance,dim_a,pas);
    }
  } else if (cases == 10){ // Case 10
    dim = changementDim(dim_a);
    if (dim == dim_a){
      dim = case1(ligne,colonne,case_rand,recule,avance,dim_a,pas);
    }
  } else if (cases == 11){ // Case 11
    if (dim_a == 1){
      if(mastermind(12,8)){
        if (dim_a == -1){
          printf("Bravo, 3 cases avancent par rapport à vous !\n\n");
          *recule = 3;
        } else {
          printf("Bravo, vous avancez de 3 cases !\n\n");
          *avance = 3;
        }
      } else {
        if (dim_a == -1){
          printf("Bon c'était dur, mais une case recule par rapport à vous !\n\n");
          *avance = 1;
        } else {
          printf("Bon c'était dur, mais vous reculez d'une case !\n\n");
          *recule = 1;
        }
      }
    } else {
      chaudFroid(dim_a);
    }
  } else if (cases == 12){ // Case 12
    if (dim_a == 1){
      printf("Vous reculez de 4 cases car 12 = 6 * 2\n\n");
      *recule = 4;
    } else if (dim_a <= -6) { 
      cartesTirage(rand_max()%1001,(rand_max()%10)-5);
    } else {
      cartesTirage(rand_max()%1001,dim_a); // Cartes à collectionner
    }
  } else if (cases == 13){ // Case 13
    if (dim_a == 1){
      if (rand_max() % 2 == 0){
        printf("Vous avancez de 2 cases (Merci l'aléatoire) !\n\n");
        *avance = 2;
      } else {
        printf("Vous reculez de 2 cases (Non merci l'aléatoire) !\n\n");
        *recule = 2;
      }
    } else if (dim_a == 2) {
      if (rand_max() % 1000 < 950 - sommeDep(50,count_dep)){
        printf("L'aléatoire a décidé que vous devez être téléporté en dimension négative !\n\n");
        dim = -1;
      } else {
        printf("L'aléatoire a décidé que vous devez être téléporté en dimension nulle !\n\n");
        dim = 0;
      }
    } else if (dim_a == 0 || dim_a == -2 || dim_a == 6 || dim_a <= -7) { 
      printf("L'aléatoire a décidé... vous restez là !\n\n");
    } else if (dim_a <= -6) {
      printf("L'aléatoire a décidé : vous êtes téléporté dans les backrooms de M. Faure !\n\n");
      dim = -7;
    } else if (dim_a <= -4) {
      printf("L'aléatoire a décidé : vous êtes téléporté dans les backrooms de M. Carcenac !\n\n");
      dim = -5;
    } else if (dim_a == 7) { 
      printf("%d : %d\n\n",rand_max(),rand_max());
    } else if (dim_a >= 3) { 
      printf("L'aléatoire a décidé : vous êtes téléporté dans les backrooms de Jean-Eudes !\n\n");
      dim = 4;
    } else {
      if (dim_a == -3 && !professeurC) printf("Le professeur C a été rattrapé... Vous n'avez pas réussi à le sauver.\n");
      printf("L'aléatoire a décidé : vous allez dans les Backrooms !\n\n");
      dim = -2;
    }
  } else if (cases == 14){ // Case 14
    if (dim_a == 1){
      printRick();
      printf("You have been rickrolled !\n\n");
      printf("Vous reculez de 2 cases.\n\n");
      *recule = 2;
    } else if (rand_max() % 50 == 0 && dim_a > -6 && dim_a < 6) { 
      printf("Vous êtes téléporté quelque part ???\n\n");
      dim = -6;
    } else if (points_count >= 400 && !professeurC) { 
      if (decouvertePJ) printf("Vous avez 400 points ! M. Jacob a libéré le professeur C !\n\n");
      else printf("Vous avez 400 points ! Le professeur C a été libéré !\n\n");
      professeurC = true;
    } else if (points_count >= 500) { 
      if (dim_a == -5){
        dim = 11;
      } else {
        printf("Vous avez 500 points : vous êtes téléporté dans les Backrooms de M. Carcenac !\n\n");
        dim = -5;
      }
    } else { // Affichage argent actuel (réel + dons)
      if(dim_a == 7) printf("%d %f %d !\n\n",rand_max(),points_give,rand_max());
      else printf("Vous gagnez %f point(s) !\n\n",points_give);
      points_count += points_give;
    }
  } else if (cases == 15){ // Case 15
    if (dim_a == 1){ // Pile ou face
      int PouF;
      printf("Pile ou Face (Pile : 0, Face : 1) : ");
      if (scanf("%d",&PouF) == 1 && PouF == PF()){
        printf("Bravo ! Vous avancez d'une case !\n\n");
        *avance = 1;
      } else {
        printf("Perdu ! Vous reculez de 3 cases !\n\n");
        *recule = 3;
      }
      while(getchar()!='\n');
    } else if (dim_a == 2 || dim_a <= -6 || dim_a >= 6) {
      cartesTirage(rand_max()%1001,(rand_max()%10)-5); // Cartes à collectionner de toute dimension (dimension non initialisée, Faure, Jansou)
    } else {
      machineASous(5,dim_a);
    }
  } else if (cases == 16){ // Case 16 (case demandée par Alexandre)
    if (dim_a >= -3  && dim_a <= -1){
      printf("Case 3 avance !\n");
      printf("FEUR !\n");
      Sleep(3);
      printf("OUGE !\n\n");
      *avance = 3;
    } else if (dim_a == 1) {
      printf("Vous avancez de 3 cases !\n");
      printf("Quoi ? \n");
      Sleep(3);
      printf("FEUR ! \n\n");
      *avance = 3;
    } else if (dim_a == 2){
      printf("Bravo ! Argent : +%f\n\n",1000*money_give);
      argent.r += 1000*money_give;
    } else if (dim_a == 0){
      printf("Don du zéro !\n");
      if (argent.r < 0){
        printf("Votre découvert est remboursé !\n\n");
        argent.r = 0;
      } else if (argent.r == 0){
        printf("Argent : +%f.\n\n",100*money_give);
        argent.r = 100*money_give;
      } else {
        printf("Votre argent est multiplié par 3 !\n\n");
        argent.r *= 3;
      }
    } else if (dim_a >= 6 || dim_a == -8){
      if (dim_a == 7) printf("%d : +%f.\n\n",rand_max(),500*money_give);
      else printf("Pour tout le monde : +%f.\n\n",500*money_give);
      argent.r += 500*money_give;
      argent.i += 500*money_give;
      argent.j += 500*money_give;
      argent.c += 500*money_give;
    } else if (dim_a >= 3){
      if (decouverteJE) printf("Argent donné à Jean-Eudes : +%f.\n\n",100*money_give);
      else printf("Argent donné à ??? : +%f.\n\n",100*money_give);
      argent.i += 100*money_give;
    } else if (dim_a <= -6){
      printf("Pour tout le monde (on a fait des économies : c'est pas cher) : +%f.\n\n",100*money_give);
      argent.r += 100*money_give;
      argent.i += 100*money_give;
      argent.j += 100*money_give;
      argent.c += 100*money_give;
    } else if (dim_a <= -4){
      printf("Commandes Linux : +%f.\n\n",100*money_give);
      argent.c += 100*money_give;
    }
  } else if (cases == 17){ // Case 17
    MalusTP(20,dim_a);
    if (dim_a == 1){
      printf("Vous reculez de 2 cases !\n\n");
      *recule = 2;
    }
  } else if (cases == 18){ // Case 18 (case ++)
    if (dim_a <= -1 && dim_a >= -3){
      printf("++ vous a utilisé !\n");
      printf("-3 cases vous reculent !\n\n");
      *avance = 3;
    } else if (dim_a == 7) { 
      printf("%d %d...\n",rand_max(),rand_max());
      printf("%d %d %d %d!\n\n",rand_max(),rand_max(),rand_max(),rand_max());
      *recule = 3;
    } else if (dim_a >= 6){
      printf("Attaque déception...\n");
      printf("Vous perdez 3 points !\n\n");
      *recule = 3;
    } else if (dim_a == -8){
      printf("Vous avez tenté de déterminer votre position !\n");
      printf("Vous perdez rien du tout!\n\n");
      *recule = 3;
    } else if (dim_a <= -6){
      printf("Vous avez utilisé le pignouf crémaillère !\n");
      printf("Vous perdez 1.1 point !\n\n");
      *recule = 3;
    } else if (dim_a <= -4){
      printf("Vous avez utilisé ++ !\n");
      printf("Vous perdez 3 points (cases) !\n\n");
      *recule = 3;
    } else if (dim_a >= 3){
      printf("Vous avez pensé à utiliser ++ !\n");
      printf("Vous reculez de 3i cases !\n\n");
      *recule = 3;
    } else {
      printf("Vous avez utilisé ++ (slide 80, diapo 8) !\n");
      printf("Vous reculez de 3 cases !\n\n");
      *recule = 3;
    }
  } else if (cases == 19){ // Case 19
    dim = changementDim(dim_a);
    if (dim == dim_a){
      dim = case1(ligne,colonne,case_rand,recule,avance,dim_a,pas);
    }
  } else if (cases == 20){ // Case 20 (juste prix)
    int justePrix1;
    bool jp_1 = JustePrix(dim_a,20,10000,&justePrix1);
    if (dim_a == 1){
      if (jp_1){
        printf("Bravo, vous avancez de 3 cases !\n\n");
        *avance = 3;
      } else {
        printf("Perdu ! Vous reculez de 2 cases !\n\n");
        *recule = 2;
      }
    } else if (dim_a == 2){
      if (jp_1){
        printf("Bravo ! Argent : +%f\n\n",justePrix1*money_give);
        argent.r += justePrix1*money_give;
      } else {
        printf("Perdu ! Vous êtes téléporté en dimension négative !\n\n");
        dim = -1;
      }
    } else if (dim_a == -1){
      if (jp_1){
        if (decouvertePJ) printf("Bravo ! Argent donné à M. Jacob : %f\n\n",justePrix1*money_give);
        else printf("Bravo ! Argent donné à ??? : %f\n\n",justePrix1*money_give);
        argent.j += justePrix1*money_give;
      } else {
        printf("Perdu ! Vous êtes téléporté dans les Backrooms !\n\n");
        dim = -2;
      }
    } else if (dim_a == -2){
      if (jp_1){
        if (decouvertePJ) printf("Bravo ! Argent donné à M. Jacob : %f\n\n",justePrix1*money_give);
        else printf("Bravo ! Argent donné à ??? : %f\n\n",justePrix1*money_give);
        argent.j += justePrix1*money_give;
      } else {
        printf("Perdu ! Vous êtes téléporté dans la Prison de M. Jacob !\n\n");
        dim = -3;
      }
    } else if (dim_a == -3){
      if (jp_1){
        if (decouvertePJ) printf("Bravo ! Argent donné à M. Jacob : %f\n\n",justePrix1*money_give);
        else printf("Bravo ! Argent donné à ??? : %f\n\n",justePrix1*money_give);
        argent.j += justePrix1*money_give;
      } else {
        printf("Perdu ! Vous êtes téléporté... dans une autre partie de la prison !\n\n");
      }
    } else if (dim_a == 0){
      if (jp_1){
        printf("Bravo ! Argent : +%f\n\n",justePrix1*money_give);
        argent.r += justePrix1*money_give;
      } else {
        printf("Perdu ! Vous êtes téléporté en dimension non initialisée !\n\n");
        dim = 2;
      }
    } else if (dim_a == 3){
      if (jp_1){
        if (decouverteJE) printf("Bravo ! Argent donné à Jean-Eudes : %f\n\n",justePrix1*money_give);
        else printf("Bravo ! Argent donné à ??? : %f\n\n",justePrix1*money_give);
        argent.i += justePrix1*money_give;
      } else {
        printf("Perdu ! Vous êtes téléporté dans les backrooms de Jean-Eudes !\n\n");
        dim = 4;
      }
    } else if (dim_a == 4){
      if (jp_1){
        if (decouverteJE) printf("Bravo ! Argent donné à Jean-Eudes : %f\n\n",justePrix1*money_give);
        else printf("Bravo ! Argent donné à ??? : %f\n\n",justePrix1*money_give);
        argent.i += justePrix1*money_give;
      } else {
        printf("Perdu ! Vous êtes téléporté dans la prison de M. Jacob !\n\n");
        dim = -3;
      }
    } else if (dim_a == -4){
      if (jp_1){
        printf("Bravo ! Commandes Linux : +%f\n\n",justePrix1*money_give);
        argent.c += justePrix1*money_give;
      } else {
        printf("Perdu ! Vous êtes téléporté dans la dimension négative !\n\n");
        dim = -1;
      }
    } else if (dim_a == -5){
      if (jp_1){
        printf("Bravo ! Commandes Linux : +%f\n\n",justePrix1*money_give);
        argent.c += justePrix1*money_give;
      } else {
        printf("Perdu ! Vous êtes téléporté dans la dimension Carcenac !\n\n");
        dim = -4;
      }
    } else if (dim_a == 6 || dim_a == -8) { // Backrooms de M. Jansou
      if (jp_1){
        int argent_rand = rand_max() % 4;
        if (argent_rand == 0){
          printf("Bravo ! Argent : +%f\n\n",justePrix1*money_give);
          argent.r += justePrix1*money_give;
        } else if (argent_rand == 1){
          if (decouverteJE) printf("Bravo ! Argent donné à Jean-Eudes : %f\n\n",justePrix1*money_give);
          else printf("Bravo ! Argent donné à ??? : %f\n\n",justePrix1*money_give);
          argent.i += justePrix1*money_give;
        } else if (argent_rand == 2){
          if (decouvertePJ) printf("Bravo ! Argent donné à M. Jacob : %f\n\n",justePrix1*money_give);
          else printf("Bravo ! Argent donné à ??? : %f\n\n",justePrix1*money_give);
          argent.j += justePrix1*money_give;
        } else if (argent_rand == 3){
          printf("Bravo ! Commandes Linux : +%f\n\n",justePrix1*money_give);
          argent.c += justePrix1*money_give;
        }
      }
    } else if (dim_a == 7) { // Fond du trou
      if (jp_1){
        int argent_rand = rand_max() % 4;
        printf("%d ! %d : +%f\n\n",rand_max(),rand_max(),justePrix1*money_give);
        if (argent_rand == 0) argent.r += justePrix1*money_give;
        else if (argent_rand == 1) argent.i += justePrix1*money_give;
        else if (argent_rand == 2) argent.j += justePrix1*money_give;
        else if (argent_rand == 3) argent.c += justePrix1*money_give;
      } else {
        printf("Perdu !\n\n");
      }
    } else { // Dimensions de M. Faure
      if (jp_1){
        if (decouverteJE) printf("Bravo ! Argent donné à Jean-Eudes : %f\n\n",justePrix1*money_give);
        else printf("Bravo ! Argent donné à ??? : %f\n\n",justePrix1*money_give);
        argent.i += justePrix1*money_give;
      } else {
        printf("Perdu ! Vous êtes téléporté dans le dépotoir !\n\n");
        dim = -6;
      }
    }
  } else if (cases == 21){ // Case 21
    if (!mdp[0]){
      if (dim_a == 7) printf("%d n°1 : 14076\n",rand_max());
      else printf("Mot de passe n° 1 : 14076\n");
      printf("PS : Vous pouvez le noter.\n\n");
      mdp[0] = true;
    } else if (!mdp[3]){
      if (dim_a == 7) printf("%d n°4 : %d\n",rand_max(),codes_mdp[0]);
      else printf("Mot de passe n° 4 : %d\n",codes_mdp[0]);
      printf("PS : Vous pouvez le noter.\n\n");
      mdp[3] = true;
    } else if (dim_a == 7) {
      if (!decouverteBC) printf("??? %d %f %d !\n\n",rand_max(),100*money_give,rand_max());
      else printf("%d %f %d (-5) !\n\n",rand_max(),100*money_give,rand_max());
      argent.c += 100*money_give;
    } else {
      if (!decouverteJE) printf("??? gagne %f d'argent !\n\n",100*money_give);
      else printf("Jean-Eudes gagne %f d'argent !\n\n",100*money_give);
      argent.i += 100*money_give;
    }
  } else if (cases == 22){ // Case 22 (prison)
    Prison(30,dim_a);
  } else if (cases == 23){ // Case 23 (complexité n^3)
    NCube(3,dim_a);
  } else if (cases == 24){ // Case 24
    dim = changementDim(dim_a);
    if (dim == dim_a){
      dim = case1(ligne,colonne,case_rand,recule,avance,dim_a,pas);
    }
  } else if (cases == 25){ // Case 25 (case --)
    if (dim_a <= -1 && dim_a >= -3){
      printf("-- vous a utilisé !\n");
      printf("-3 cases vous reculent !\n\n");
      *avance = 3;
    } else if (dim_a == 7) { 
      printf("%d %d...\n",rand_max(),rand_max());
      printf("%d %d %d %d!\n\n",rand_max(),rand_max(),rand_max(),rand_max());
    } else if (dim_a >= 6){
      printf("Attaque déception... Avant tu faisais bien mieux que ça !\n");
      printf("Vous perdez 3 points !\n\n");
      *recule = 3;
    } else if (dim_a == -8){
      printf("Vous avez tenté de déterminer votre position !\n");
      printf("Vous perdez rien du tout!\n\n");
      *recule = 3;
    } else if (dim_a <= -6){
      printf("Vous avez trop bijectivé le manivelle-bâti !\n");
      printf("Vous perdez 3 points !\n\n");
      *recule = 3;
    } else if (dim_a <= -4){
      printf("Vous avez utilisé -- !\n");
      printf("Vous perdez 3 points (cases) !\n\n");
      *recule = 3;
    } else if (dim_a >= 3){
      printf("Vous avez pensé à utiliser -- !\n");
      printf("Vous reculez de 3i cases !\n\n");
      *recule = 3;
    } else {
      printf("Vous avez utilisé -- (slide 81, diapo 8) !\n");
      printf("Vous reculez de 3 cases !\n\n");
      *recule = 3;
    }
  } else if (cases == 26){ // Case 26 (case TP)
    MalusTP(25,dim_a);
    if (dim_a == 1){
      printf("Vous reculez de 2 cases !\n\n");
      *recule = 2;
    }
  } else if (cases == 27){ // Case 27
    if (dim_a == 1){
      printRick();
      printf("You have been rickrolled !\n\n");
      printf("Vous reculez de 3 cases.\n\n");
      *recule = 3;
    } else if (rand_max() % 50 == 0 && dim_a > -6 && dim_a < 6) { 
      printf("Vous êtes téléporté quelque part ???\n\n");
      dim = -6;
    } else { // Affichage argent actuel (réel + dons)
      if(dim_a == 7) printf("%d %f %d !\n\n",rand_max(),points_give,rand_max());
      else printf("Vous gagnez %f point(s) !\n\n",points_give);
      points_count += points_give;
    }
  } else if (cases == 28){ // Case 28
    if (!mdp[1]){
      if (dim_a == 7) printf("%d n° 2 : 176634.\n",rand_max());
      else printf("Mot de passe n° 2 : 176634\n");
      printf("PS : Vous pouvez le noter.\n\n");
      mdp[1] = true;
    } else if (!mdp[4]){
      if (dim_a == 7) printf("%d n° 5 : %d.\n",rand_max(),codes_mdp[1]);
      else printf("Mot de passe n° 5 : %d\n",codes_mdp[1]);
      printf("PS : Vous pouvez le noter.\n\n");
      mdp[4] = true;
    } else if (dim_a == 7) {
      if (!decouverteBC) printf("??? %d %f %d !\n\n",rand_max(),100*money_give,rand_max());
      else printf("%d %f %d (-5) !\n\n",rand_max(),100*money_give,rand_max());
      argent.c += 100*money_give;
    } else {
      if (!decouverteJE) printf("??? gagne %f d'argent !\n\n",100*money_give);
      else printf("Jean-Eudes gagne %f d'argent !\n\n",100*money_give);
      argent.i += 100*money_give;
    }
  } else if (cases == 29){ // Case 29 (calculs sympathiques)
    if (dim_a == 1 || dim_a == 2 || dim_a <= -4) printf("Calculer (29*3)/4 [en C]\n");
    else if (dim_a == 0 || dim_a == 6) printf("Calculer ((0^0+0^0)^(0^0+0^0))!-0^0-0^0-0^0\n");
    else if (dim_a == 7) printf("((0^0+0^0)^(0^0+0^0))!-0^0-0^0-0^0\n");
    else if (dim_a >= 3) printf("Calculer 3i*7i*e^(i*pi)\n");
    else if (dim_a >= -3 && dim_a <= -1) printf("Calculer (-8*(-3))-3\n");
    int calcul_1;
    if (scanf("%d",&calcul_1) == 1 && calcul_1 == 21){
      if (dim_a == 7) printf("%d ! %d ! %d !\n\n",rand_max(),rand_max(),rand_max());
      else printf("Bravo ! Vous reculez de 8 cases ! Wouhou !\n\n");
    } else {
      if (dim_a == 7) printf("%d ! %d !\n\n",rand_max(),rand_max());
      else printf("La réponse était 21 ! Donc vous reculez de 8 cases !\n\n");
    }
    *recule = 8;
    while(getchar()!='\n');
  } else if (cases == 30){ // Case 30
    if (dim_a == 1){
      bool rrr_1 = RRR(2,3);
      if (rrr_1){
        printf("Vous reculez de 2 cases !\n\n");
        *recule = 2;
      } else {
        printf("Vous avancez de 2 cases (Vous avez évité Rick) !\n\n");
        *avance = 2;
      }
    } else if (dim_a <= -6 || dim_a >= 6) {
      cartesTirage(rand_max()%1001,(rand_max()%10)-5); // Cartes à collectionner de toute dimension (dimensions Faure)
    } else {
      machineASous(10, dim_a);
    }
  } else if (cases == 31){ // Case 31
    if (dim_a == 1 || dim_a == 2){ // Fonction mystère sur une ligne
      printf("#include <stdio.h>\nint mystere(int a,int b,int c,int d, int e){return a*b-c*e*d+b*c*e-b*d*d*d*e*a*b*c*d*e+e*d*d*d*e*e*e*e*c;}int main(){int b = mystere(8*8-8/8-8-8+8+8+8/8*8-8/8,5-5*5-5*5*5*5*5+5*5-5*5-5*5*5*5,2*2+2*2-2*2-2*2+2*2*2,-3*3*3+3-3*3,4*4+(4*4-4)*4);return 0;}\n");
      int lig_c_1;
      printf("Que vaut b après exécution de la fonction mystère ? ");
      if(scanf("%d",&lig_c_1) == 1){
        if (lig_c_1 == 1743963940){
          printf("Bravo ! Vous avancez d'une case !\n\n");
          *avance = 1;
        }
        else {
          printf("Perdu ! Vous reculez de 4 cases !\n\n");
          *recule = 4;
        }
      } else {
        printf("Vous marquez n'importe quoi ! Reculez de 4 cases !\n\n");
        *recule = 4;
      }
      while(getchar()!='\n');
    } else if (dim_a <= -6 || dim_a >= 6) {
      cartesTirage(rand_max()%1001,(rand_max()%10)-5); // Cartes à collectionner de toute dimension (dimensions Faure)
    } else {
      cartesTirage(rand_max() % 1001,dim_a); // Cartes à collectionner
    }
  } else if (cases == 32){ // Case 32
    if (dim_a == 1){
      if(mastermind(10,8)){
        printf("Bravo, vous avancez de 4 cases !\n\n");
        *avance = 4;
      } else {
        printf("Bon c'était dur, mais vous reculez de 2 cases !\n\n");
        *recule = 2;
      }
    } else {
      chaudFroid(dim_a);
    }
  } else if (cases == 33){ // Case 33
    dim = changementDim(dim_a);
    if (dim == dim_a){
      dim = case1(ligne,colonne,case_rand,recule,avance,dim_a,pas);
    }
  } else if (cases == 34){ // Case 34
    int justePrix2;
    bool jp_2 = JustePrix(dim_a,15,10000,&justePrix2);
    if (dim_a == 1){
      if (jp_2){
        printf("Bravo, vous avancez de 4 cases !\n\n");
        *avance = 4;
      } else {
        printf("Perdu ! Vous reculez de 2 cases !\n\n");
        *recule = 2;
      }
    } else if (dim_a == 2){
      if (jp_2){
        printf("Bravo ! Argent : +%f\n\n",justePrix2*money_give);
        argent.r += justePrix2*money_give;
      } else {
        printf("Perdu ! Vous êtes téléporté en dimension négative !\n\n");
        dim = -1;
      }
    } else if (dim_a == -1){
      if (jp_2){
        if (decouvertePJ) printf("Bravo ! Argent donné à M. Jacob : %f\n\n",justePrix2*money_give);
        else printf("Bravo ! Argent donné à ??? : %f\n\n",justePrix2*money_give);
        argent.j += justePrix2*money_give;
      } else {
        printf("Perdu ! Vous êtes téléporté dans les Backrooms !\n\n");
        dim = -2;
      }
    } else if (dim_a == -2){
      if (jp_2){
        if (decouvertePJ) printf("Bravo ! Argent donné à M. Jacob : %f\n\n",justePrix2*money_give);
        else printf("Bravo ! Argent donné à ??? : %f\n\n",justePrix2*money_give);
        argent.j += justePrix2*money_give;
      } else {
        printf("Perdu ! Vous êtes téléporté dans la Prison de M. Jacob !\n\n");
        dim = -3;
      }
    } else if (dim_a == -3){
      if (jp_2){
        if (decouvertePJ) printf("Bravo ! Argent donné à M. Jacob : %f\n\n",justePrix2*money_give);
        else printf("Bravo ! Argent donné à ??? : %f\n\n",justePrix2*money_give);
        argent.j += justePrix2*money_give;
      } else {
        printf("Perdu ! Vous êtes téléporté... dans une autre partie de la prison !\n\n");
      }
    } else if (dim_a == 0){
      if (jp_2){
        printf("Bravo ! Argent : +%f\n\n",justePrix2*money_give);
        argent.r += justePrix2*money_give;
      } else {
        printf("Perdu ! Vous êtes téléporté en dimension non initialisée !\n\n");
        dim = 2;
      }
    } else if (dim_a == 3){
      if (jp_2){
        if (decouverteJE) printf("Bravo ! Argent donné à Jean-Eudes : %f\n\n",justePrix2*money_give);
        else printf("Bravo ! Argent donné à ??? : %f\n\n",justePrix2*money_give);
        argent.i += justePrix2*money_give;
      } else {
        printf("Perdu ! Vous êtes téléporté dans les backrooms de Jean-Eudes !\n\n");
        dim = 4;
      }
    } else if (dim_a == 4){
      if (jp_2){
        if (decouverteJE) printf("Bravo ! Argent donné à Jean-Eudes : %f\n\n",justePrix2*money_give);
        else printf("Bravo ! Argent donné à ??? : %f\n\n",justePrix2*money_give);
        argent.i += justePrix2*money_give;
      } else {
        printf("Perdu ! Vous êtes téléporté dans la prison de M. Jacob !\n\n");
        dim = -3;
      }
    } else if (dim_a == -4){
      if (jp_2){
        printf("Bravo ! Commandes Linux : +%f\n\n",justePrix2*money_give);
        argent.c += justePrix2*money_give;
      } else {
        printf("Perdu ! Vous êtes téléporté dans la dimension négative !\n\n");
        dim = -1;
      }
    } else if (dim_a == -5){
      if (jp_2){
        printf("Bravo ! Commandes Linux : +%f\n\n",justePrix2*money_give);
        argent.c += justePrix2*money_give;
      } else {
        printf("Perdu ! Vous êtes téléporté dans la dimension Carcenac !\n\n");
        dim = -4;
      }
    } else if (dim_a == 7) { // Fond du trou
      if (jp_2){
        int argent_rand = rand_max() % 4;
        printf("%d ! %d : +%f\n\n",rand_max(),rand_max(),justePrix2*money_give);
        if (argent_rand == 0) argent.r += justePrix2*money_give;
        else if (argent_rand == 1) argent.i += justePrix2*money_give;
        else if (argent_rand == 2) argent.j += justePrix2*money_give;
        else if (argent_rand == 3) argent.c += justePrix2*money_give;
      } else {
        printf("Perdu !\n\n");
      }
    } else if (dim_a == 6) { // Backrooms de M. Jansou
      if (jp_2){
        int argent_rand = rand_max() % 4;
        if (argent_rand == 0){
          printf("Bravo ! Argent : +%f\n\n",justePrix2*money_give);
          argent.r += justePrix2*money_give;
        } else if (argent_rand == 1){
          if (decouverteJE) printf("Bravo ! Argent donné à Jean-Eudes : %f\n\n",justePrix2*money_give);
          else printf("Bravo ! Argent donné à ??? : %f\n\n",justePrix2*money_give);
          argent.i += justePrix2*money_give;
        } else if (argent_rand == 2){
          if (decouverteJE) printf("Bravo ! Argent donné à Jean-Eudes : %f\n\n",justePrix2*money_give);
          else printf("Bravo ! Argent donné à ??? : %f\n\n",justePrix2*money_give);
          argent.j += justePrix2*money_give;
        } else if (argent_rand == 3){
          printf("Bravo ! Commandes Linux : +%f\n\n",justePrix2*money_give);
          argent.c += justePrix2*money_give;
        }
      } else {
        printf("Perdu ! Vous êtes téléporté dans les backrooms de M. Faure !\n\n");
        dim = -7;
      }
    } else { // Dimensions de M. Faure
      if (jp_2){
        if (decouverteJE) printf("Bravo ! Argent donné à Jean-Eudes : %f\n\n",justePrix2*money_give);
        else printf("Bravo ! Argent donné à ??? : %f\n\n",justePrix2*money_give);
        argent.i += justePrix2*money_give;
      } else {
        printf("Perdu ! Vous êtes téléporté dans le dépotoir !\n\n");
        dim = -6;
      }
    }
  } else if (cases == 35){ // Case 35
    MalusTP(30,dim_a);
    if (dim_a == 1){
      printf("Vous reculez de 3 cases !\n\n");
      *recule = 3;
    }
  } else if (cases == 36){ // Case 36 (case où il faut mettre oui)
    int mn;
    if (dim_a == 1) printf("Avez-vous marqué votre nom (0 pour non, 1 pour oui) ? ");
    else if (dim_a == 2) printf("Avez-vous initialisé vos variables (0 pour non, 1 pour oui) ? ");
    else if (dim_a == 7) printf("%d (0,1) ? ",rand_max());
    else if ((dim_a >= -3 && dim_a <= -1) || dim_a >= 6) printf("Avez-vous évité d'écrire sqrt(-1) (0 pour non, 1 pour oui) ? ");
    else if (dim_a == 0) printf("Avez-vous évité de tenter d'afficher un élément trop lointain d'un tableau (0 pour non, 1 pour oui) ? ");
    else if (dim_a == -8) printf("Avez-vous tenté de miner 500 000 bitcoins (0 pour non, 1 pour oui) ? ");
    else if (dim_a <= -6) printf("Avez-vous évité de marquer pignouf-crémaillère dans votre copie (0 pour non, 1 pour oui) ? ");
    else if (dim_a <= -4) printf("Avez-vous rendu votre DM (0 pour non, 1 pour oui) ? ");
    else printf("Avez-vous évité d'écrire sqrt(i) (0 pour non, 1 pour oui) ? ");
    if (scanf("%d",&mn) == 1){
      if (mn == 1){
        if (dim_a == 7) printf("%d ! %d !\n\n",rand_max(),rand_max());
        else printf("C'est bien ! Vous avancez de 4 cases !\n\n");
        *avance = 4;
      } else if (mn == 0){
        if (dim_a == 7) printf("%d (%d) ! %d !\n\n",rand_max(),rand_max(),rand_max());
        else printf("C'est pas bien (je l'ai fait aussi) ! Vous reculez de 10 cases !\n\n");
        *recule = 10;
      } else {
        if (dim_a == 7) printf("%d, %d !\n\n",rand_max(),rand_max());
        else printf("Je comprends pas, donc vous reculez de 4 cases !\n\n");
        *recule = 4;
      }
    } else {
      if (dim_a == 7) printf("%d ! %d ! %d !\n\n",rand_max(),rand_max(),rand_max());
      else printf("N'importe quoi ! Reculez ! 4 cases plus loin !\n\n");
      *recule = 4;
    }
    while(getchar()!='\n');
  } else if (cases == 37){ // Case 37
    if (dim_a == 1){
      printRick();
      printf("You have been rickrolled !\n\n");
      printf("Vous reculez de 4 cases.\n\n");
      *recule = 4;
    } else if (rand_max() % 50 == 0 && dim_a > -6 && dim_a < 6) { 
      printf("Vous êtes téléporté quelque part ???\n\n");
      dim = -6;
    } else { // Affichage argent actuel (réel + dons)
      if(dim_a == 7) printf("%d %f %d !\n\n",rand_max(),points_give,rand_max());
      else printf("Vous gagnez %f point(s) !\n\n",points_give);
      points_count += points_give;
    }
  } else if (cases == 38){ // Case 38 (calculs fort sympathiques)
    if (dim_a == 1 || dim_a == 2 || dim_a <= -4) printf("Calculer (38*3)/4 [en C]\n");
    else if (dim_a == 7) printf("((0^0+0^0+0^0)!+0^0)*((0^0+0^0)^(0^0+0^0))\n");
    else if (dim_a == 0 || dim_a >= 6) printf("Calculer ((0^0+0^0+0^0)!+0^0)*((0^0+0^0)^(0^0+0^0))\n");
    else if (dim_a >= 3) printf("Calculer -7i * 4i\n");
    else if (dim_a >= -3 && dim_a <= -1) printf("Calculer -ln(e^7)*(-4)\n");
    int calcul_1;
    if (scanf("%d",&calcul_1) == 1 && calcul_1 == 28){
      printf("Bravo ! Vous reculez donc de 10 cases !\n\n");
    } else {
      printf("La réponse était 28 ! Donc vous reculez de 10 cases !\n\n");
    }
    *recule = 10;
    while(getchar()!='\n');
  } else if (cases == 39){ // Case 39
    dim = changementDim(dim_a);
    if (dim == dim_a){
      dim = case1(ligne,colonne,case_rand,recule,avance,dim_a,pas);
    }
  } else if (cases == 40){ // Case 40
    if (!mdp[2]){
      if (dim_a == 7) printf("%d n° 3: 380649\n",rand_max());
      else printf("Mot de passe n° 3 : 380649\n");
      printf("PS : Vous pouvez le noter.\n\n");
      mdp[2]=true;
    } else if (!mdp[5]){
      if (dim_a == 7) printf("%d n° 6 : %d\n",rand_max(),codes_mdp[2]);
      else printf("Mot de passe n° 6 : %d\n",codes_mdp[2]);
      printf("PS : Vous pouvez le noter.\n\n");
      mdp[5]=true;
    } else if (dim_a == 7) {
      if (!decouverteBC) printf("??? %d %f %d !\n\n",rand_max(),100*money_give,rand_max());
      else printf("%d %f %d (-5) !\n\n",rand_max(),100*money_give,rand_max());
      argent.c += 100*money_give;
    } else {
      if (!decouverteJE) printf("??? gagne %f d'argent !\n\n",100*money_give);
      else printf("Jean-Eudes gagne %f d'argent !\n\n",100*money_give);
      argent.i += 100*money_give;
    }
  } else if (cases == 41){ // Case 41
    dim = changementDim(dim_a);
    if (dim == dim_a){
      dim = case1(ligne,colonne,case_rand,recule,avance,dim_a,pas);
    }
  } else if (cases == 42){ // Case 42
    if (dim_a == 1){
      bool rrr_2 = RRR(3,4);
      if (rrr_2){
        printf("Vous reculez de 3 cases !\n\n");
        *recule = 3;
      } else {
        printf("Vous avancez de 2 cases (Vous avez évité Rick) !\n\n");
        *avance = 2;
      }
    } else {
      machineASous(10,dim_a);
    }
  } else if (cases == 43){ // Case 43 (hors programme)
    if (dim_a <= -1 && dim_a >= -3){
      printf("Hors programme, attention !\n");
      printf("Il vous recule de 3 cases.\n\n");
      *recule = 3;
    } else if (dim_a == 0) {
      printf("Attention au hors programme !\n");
      printf("Vous reculez de 0 case.\n\n");
    } else if (dim_a == 7) {
      printf("%d !\n",rand_max());
      printf("%d.\n\n",rand_max());
    } else if (dim_a >= 3) {
      printf("Attention au hors programme !\n");
      printf("Vous reculez de i case.\n\n");
    } else if (dim_a == 2) {
      printf("Attention au hors programme !\n");
      printf("Vous êtes téléporté en dimension négative parce que.\n\n");
      dim = -1;
    } else {
      printf("Attention au hors programme !\n");
      printf("Vous reculez de 3 cases.\n\n");
      *recule = 3;
    }
  } else if (cases == 44){ // Case 44
    MalusTP(35,dim_a);
    if (dim_a == 1){
      printf("Vous reculez de 3 cases !\n\n");
      *recule = 3;
    }
  } else if (cases == 45){ // Case 45
    if (dim_a == 1){
      int PouF_2;
      printf("Pile ou Face (Pile : 0, Face : 1) : ");
      if (scanf("%d",&PouF_2) == 1){}
      if (dim_a == -1){
        printf("Gagné ! Vous reculez de 4 cases - !\n\n");
        *avance = 4;
      } else {
        printf("Perdu ! Vous reculez de 4 cases !\n\n");
        *recule = 4;
      }
      while (getchar()!='\n');
    } else if (dim_a == 2 || dim_a <= -6 || dim_a >= 6) {
      cartesTirage(rand_max()%1001,(rand_max()%10)-5); // Cartes à collectionner de toute dimension (dimension non initialisée)
    } else {
      machineASous(5,dim_a); 
    }
  } else if (cases == 46){ // Case 46
    dim = changementDim(dim_a);
    if (dim == dim_a){
      dim = case1(ligne,colonne,case_rand,recule,avance,dim_a,pas);
    }
  } else if (cases == 47){ // Case 47 (prison)
    Prison(40,dim_a);
  } else if (cases == 48){ // Case 48
    if (dim_a == 1){
      int morp = MorpionDebile();
      for(int i = 0; i < 3; i += 1){
        for(int j = 0; j < 3; j += 1){
          tab_morp[i][j] = 0;
        }
      }
      if (morp == 1){
        printf("Bravo ! Vous avez d'une case !\n\n");
        *avance = 1;
      } else if (morp == 2) {
        printf("Perdu ! Vous reculez de 5 cases !\n\n");
        *recule = 5;
      } else{
        printf("Match nul ! Vous reculez de 2 cases !\n\n");
        *recule = 2;
      }
    } else {
      dim = utilisationCarteSecrete(dim_a);
    }
  } else if (cases == 49){ // Case 49
    if (dim_a == 1){
      printRick();
      printf("You have been rickrolled !\n\n");
      printf("Vous reculez de 4 cases.\n\n");
      *recule = 4;
    } else if (rand_max() % 50 == 0 && dim_a > -6 && dim_a < 6) { 
      printf("Vous êtes téléporté quelque part ???\n\n");
      dim = -6;
    } else { // Affichage argent actuel (réel + dons)
      if (dim_a == 7) printf("%d : *1.1/pts %d\n\n",rand_max(),rand_max());
      else printf("Amélioration : +10 pourcents par don de points\n\n");
      points_give *= 1.1;
    }
  } else if (cases == 50){ // Case 50 (complexité n^3)
    NCube(4,dim_a);
  } else if (cases == 51){ // Case 51 (professeur C)
    if (dim_a == 1){
      printf("Le professeur C. en randonnée\n\n");
      printf("Le professeur C. est parti faire de la randonnée en montagne. Pour chaque parcours, à la fin de chaque montée de chaque descente, il a noté l'altitude du lieu (en mètres) et le temps écoulé depuis le départ (en minutes). Tout ceci est consigné dans une liste de couples en Python. [Ah du Python]\n");
      printf("Q18. Quelles contraintes a-t-on sur les données de la liste pour que le parcours soit cohérent ?\n");
      printf("Q19. (Question à traiter en dernier). Ecrire une fonction  def parcoursValide(Parcours:[(int,int)]) -> bool  qui vérifie si une liste de couples correspond bien à un parcours.\n");
      printf("Q20. Ecrire une fonction  def maxmin(Parcours:[(int,int)]) -> (int,int)  qui détermine pour un parcours donné l'altitude du point le plus haut et du point le plus bas (en un seul parcours de liste)\n");
      printf("Q21. Ecrire une fonction  def deniv(Parcours:[(int,int)]) -> (int,int)  qui calcule pour un parcours donné le dénivelé positif (somme des dénivelés des montées), et le dénivelé négatif (somme des dénivelés des descentes) (\u2605 appel professeur) [Ben voyons]\n");
      printf("Q22. Ecrire une fonction  def parcoursInverse(Parcours:[(int,int)]) -> [(int,int)]  qui renvoie les données du parcours s'il avait été fait dans le sens inverse\n\n");
      if (professeurC == false) printf("Vous reculez de 3 cases (le professeur C. a mystérieusement disparu) !\n\n");
      else printf("Vous reculez de 3 cases !\n\n");
      *recule = 3;
    } else if (dim_a == 2){
      printf("I - Principe général\n");
      printf("L'exécution d'un programme consiste à obéir à des instructions et à évaluer des expressions dans un ordre précis, et en ayant accès à une zone mémoire.\n\n");
    } else if (dim_a == 7){
      printf("%d.\n\n",rand_max());
    } else if (dim_a >= 3){
      printf("On a vu qu'un appel à [fibo(n:int) -> int] était très inefficace car engendrait de très nombreux appels récursifs, souvent réalisés avec la valeur n.\n\n");
    } else if (dim_a == 0){
      printf("On rappelle qu'un entier supérieur ou égal à 2 est premier si et seulement si il n'est divisible que par 1 et par lui-même.\n");
      printf("Q1. Écrire la fonction estPremier(n) qui prend un entier n (n >= 2) en argument, et qui renvoie 1 si n est premier et 0 sinon. On appliquera directement la définition des nombres premiers donnée ci-dessous.\n\n");
    } else if (dim_a >= -3 && dim_a <= -1){
      printf("Les fonctions malloc (allouer un espace mémoire dans le tas) et free (rendre l'espace alloué au tas, ou libérer la mémoire) donnent au programme un peu de souplesse quant à l'utilisation de la mémoire.\n");
      printf("Il devient possible de réserver un espace mémoire de taille connue à l'exécution (allocation 'dynamique') et cet espace mémoire survit à un appel de fonction (durée de vie gérée par le programmeur).\n");
      printf("Ne pas oublier #include <stdlib.h>\n\n");
    } else if (dim_a >= 6) {
      printf("Implication réciproque : claire\nImplication directe : claire par contraposition\n\n");
    } else if (dim_a == -8) {
      printf("Le plus drôle, c'est de farmer 500 000 bitcoins pour sortir !\n\n");
    } else if (dim_a <= -6) {
      printf("Les pistons sont cylindriques, parce que c'est pas cher !\n\n");
    } else if (dim_a <= -4) {
      printf("La partie émergée, c'est ce qu'on aime bien quand on tripote un ordinateur. (Citation que j'ai gardée datant peut-être de décembre)\n\n");
    }
  } else if (cases == 52){ // Case 52
    float BJ_1;
    if (dim_a == 1 || dim_a >= 6){
      if (dim_a == 7) printf("%d BJ %d cm ? (5 %d) ",rand_max(),rand_max(),rand_max());
      else printf("Quelle est la taille du BJ en cm ? (à 5 cm près) ");
      while (scanf("%f",&BJ_1) != 1){
        if (dim_a == 7) printf("%d, %d : ",rand_max(),rand_max());
        else printf("Erreur de saisie, recommencez : ");
        while(getchar()!='\n');
      }
      while(getchar()!='\n');
      if (fabs(BJ_1-60.8) <= 5){
        printf("Bravo ! Vous avancez de 2 cases !\n");
        *avance = 2;
      } else {
        printf("Perdu ! Vous reculez de 3 cases !\n");
        *recule = 3;
      }
    } else if (dim_a == 2){
      printf("Quelle est la taille du BBB en cm ? (à 5 cm près) ");
      while (scanf("%f",&BJ_1) != 1){
        printf("Erreur de saisie, recommencez : ");
        while(getchar()!='\n');
      }
      while(getchar()!='\n');
      if (fabs(BJ_1-92.9) <= 5){
        printf("Bravo ! Vous avancez de 2 cases !\n");
        *avance = 2;
      } else {
        printf("Perdu ! Vous reculez de 3 cases !\n");
        *recule = 3;
      }
    } else if (dim_a >= -3 && dim_a <= -1){
      printf("Quelle est la taille du BBB en BJ ? (à 1/10 BJ près) ");
      while (scanf("%f",&BJ_1) != 1){
        printf("Erreur de saisie, recommencez : ");
        while(getchar()!='\n');
      }
      while(getchar()!='\n');
      if (fabs(BJ_1-(92.9/60.8)) <= 0.1){
        printf("Bravo ! Vous avancez de 2 cases !\n");
        *avance = 2;
      } else {
        printf("Perdu ! Vous reculez de 3 cases !\n");
        *recule = 3;
      }
    } else if (dim_a == 0){
      printf("Quelle est la taille du BJ en BBB ? (à 1/10 BBB près) ");
      while (scanf("%f",&BJ_1) != 1){
        printf("Erreur de saisie, recommencez : ");
        while(getchar()!='\n');
      }
      while(getchar()!='\n');
      if (fabs(BJ_1-(60.8/92.9)) <= 0.1){
        printf("Bravo ! Vous avancez de 2 cases !\n");
        *avance = 2;
      } else {
        printf("Perdu ! Vous reculez de 3 cases !\n");
        *recule = 3;
      }
    } else if (dim_a == 7){
      printf("%d ? (%d)\n\n",rand_max(),rand_max());
    } else if (dim_a >= 3){
      printf("Combien vaut 1 m en BJ ? (à 1/10 BJ près) ");
      while (scanf("%f",&BJ_1) != 1){
        printf("Erreur de saisie, recommencez : ");
        while(getchar()!='\n');
      }
      while(getchar()!='\n');
      if (fabs(BJ_1-(1/0.608)) <= 0.1){
        printf("Bravo ! Vous avancez de 2 cases !\n");
        *avance = 2;
      } else {
        printf("Perdu ! Vous reculez de 3 cases !\n");
        *recule = 3;
      }
    } else if (dim_a <= -4){
      printf("Combien vaut 1 m en BBB? (à 1/10 BBB près) ");
      while (scanf("%f",&BJ_1) != 1){
        printf("Erreur de saisie, recommencez : ");
        while(getchar()!='\n');
      }
      while(getchar()!='\n');
      if (fabs(BJ_1-(1/0.929)) <= 0.1){
        printf("Bravo ! Vous avancez de 2 cases !\n");
        *avance = 2;
      } else {
        printf("Perdu ! Vous reculez de 3 cases !\n");
        *recule = 3;
      }
    }

  } else if (cases == 53){ // Case 53
    MalusTP(40,dim_a);
    if (dim_a == 1){
      printf("Vous reculez de 4 cases !\n\n");
      *recule = 4;
    }
  } else if (cases == 54){ // Case 54 (case ++)
    if (dim_a <= -1 && dim_a >= -3){
      printf("++ vous a utilisé !\n");
      printf("-3 cases vous reculent !\n\n");
      *avance = 3;
    } else if (dim_a == 7) { 
      printf("%d %d...\n",rand_max(),rand_max());
      printf("%d %d %d %d!\n\n",rand_max(),rand_max(),rand_max(),rand_max());
    } else if (dim_a >= 6){
      printf("Attaque déception très forte...\n");
      printf("Vous perdez 8 points !\n\n");
      *recule = 8;
    } else if (dim_a == -8){
      printf("Vous avez tenté de déterminer votre position !\n");
      printf("Vous perdez rien du tout!\n\n");
      *recule = 3;
    } else if (dim_a <= -6){
      printf("Vous avez utilisé votre bon vieux pignouf-crémaillère !\n");
      printf("Vous perdez 1.1 point !\n\n");
      *recule = 3;
    } else if (dim_a <= -4){
      printf("Vous avez utilisé ++ !\n");
      printf("Vous perdez 3 points (cases) !\n\n");
      *recule = 3;
    } else if (dim_a >= 3){
      printf("Vous avez pensé à utiliser ++ !\n");
      printf("Vous reculez de 3i cases !\n\n");
      *recule = 3;
    } else {
      printf("Vous avez utilisé ++ (slide 82, 1ère fois, diapo 8) !\n");
      printf("Vous reculez de 3 cases !\n\n");
      *recule = 3;
    }
  } else if (cases == 55){ // Case 55 (case ++)
    if (dim_a <= -1 && dim_a >= -3){
      printf("++ vous a utilisé !\n");
      printf("-3 cases vous reculent !\n\n");
      *avance = 3;
    } else if (dim_a == 7) { 
      printf("%d %d...\n",rand_max(),rand_max());
      printf("%d %d %d %d!\n\n",rand_max(),rand_max(),rand_max(),rand_max());
    } else if (dim_a >= 6){
      printf("C'est ce qu'on pense.\n");
      printf("Vous perdez 20 points !\n\n");
      *recule = 20;
    } else if (dim_a == -8){
      printf("Vous avez tenté de déterminer votre position !\n");
      printf("Vous perdez rien du tout!\n\n");
      *recule = 3;
    } else if (dim_a <= -6){
      printf("Vous avez utilisé votre bon gros vieux pignouf-crémaillère !\n");
      printf("Vous perdez 1.1 point !\n\n");
      *recule = 3;
    } else if (dim_a <= -4){
      printf("Vous avez utilisé ++ !\n");
      printf("Vous perdez 3 points (cases) !\n\n");
      *recule = 3;
    } else if (dim_a >= 3){
      printf("Vous avez pensé à utiliser ++ !\n");
      printf("Vous reculez de 3i cases !\n\n");
      *recule = 3;
    } else {
      printf("Vous avez utilisé ++ (slide 82, 2e fois, diapo 8) !\n");
      printf("Vous reculez de 3 cases !\n\n");
      *recule = 3;
    }
  } else if (cases == 56){ // Case 56 (calcul qui donne 3)
    if (dim_a == 1) printf("Quelle est la valeur de pi selon la bible ? ");
    else if (dim_a == 2) printf("Quelle est la valeur de e selon l'approximation approximative ? ");
    else if (dim_a >= -3 && dim_a <= -1) printf("Quelle est la valeur de (-1)/(-exp(-1)) selon l'approximation approximative ? ");
    else if (dim_a == 7) printf("%d ? ",rand_max());
    else if (dim_a == 0 || dim_a >= 6) printf("Quelle est la valeur de ((0^0+0^0)!)^(ln(0^0+0^0+0^0)/ln(0!+0!)) ? ");
    else if (dim_a >= 3) printf("Quelle est la valeur de (9i)^4/2187 ? ");
    else if (dim_a <= -4) printf("Quelle est la valeur de la somme des k de 0 à 2 ? ");
    float pi_1;
    if (scanf("%f",&pi_1) == 1 && pi_1 == 3.0){
      printf("Bravo, vous avancez de 4 cases !\n\n");
      *avance = 4;
    } else {
      printf("Perdu ! La valeur était 3 ! Vous reculez de 3 cases !\n\n");
      *recule = 3;
    }
    while(getchar()!='\n');
  } else if (cases == 57){
    if (dim_a == 1) printf("Que fait cette fonction mystère qui fait des choses obscures que vous devez comprendre ?\n");
    else if (dim_a == 2) printf("Que fait cette fonction mystère qui crée des erreurs ?\n");
    else if (dim_a >= -3 && dim_a <= -1){ 
      if (!professeurC) printf("Que fait le professeur C. pour sa fonction mystère (PS : il est retenu prisonnier) ?\n");
      else printf("Que fait le professeur C. pour sa fonction mystère (PS : depuis que vous l'avez sauvé, il peut recommencer...) ?\n");
    } else if (dim_a == 0) printf("Que fait cette fonction mystère qui ne fait rien en apparence mais qui fait quelque chose comme par magie ?\n");
    else if (dim_a >= 3) printf("Une liste ? HEIN ? Cela n'existe pas !\n");
    else if (dim_a == 7) printf("%d\n",rand_max());
    else if (dim_a >= 6) printf("CLAIR\n");
    else if (dim_a <= -4) printf("Que fait cette fonction mystère créée par une personne mystère qui est maître de cette dimension ?\n");
    printf("PS : On sait tous que le professeur C. a créé les fonctions mystères avant sa randonnée...\n");
    printf("PS 2 : Vous reculez d'une case !\n\n");
    *recule = 1;
  } else if (cases == 58){ // Case 58
    if (dim_a == 1){
      if(mastermind(12,10)){
        printf("Bravo, vous avancez de 3 cases !\n\n");
        *avance = 3;
      } else {
        printf("Bon c'était dur, mais vous reculez de 2 cases !\n\n");
        *recule = 2;
      }
    } else {
      chaudFroid(dim_a);
    }
    
  } else if (cases == 59){ // Case 59
    dim = changementDim(dim_a);
    if (dim == dim_a){
      dim = case1(ligne,colonne,case_rand,recule,avance,dim_a,pas);
    }
  } else if (cases == 60){ // Case 60
    dim = changementDim(dim_a);
    if (dim == dim_a){
      dim = case1(ligne,colonne,case_rand,recule,avance,dim_a,pas);
    }
  } else if (cases == 61){ // Case 61
    dim = changementDim(dim_a);
    if (dim == dim_a){
      dim = case1(ligne,colonne,case_rand,recule,avance,dim_a,pas);
    }
  } else if (cases == 62){ // Case 62
    MalusTP(45,dim_a);
    if (dim_a == 1){
      printf("Vous reculez de 4 cases !\n\n");
      *recule = 4;   
    }
    dim = changementDim(dim_a);
    if (dim == dim_a){
      dim = case1(ligne,colonne,case_rand,recule,avance,dim_a,pas);
    }
  } else if (cases == 63){ // Case 63 (FIN ou départ)
    if (dim_a == 2){
      printf("Vous revenez au départ...\n\n");
      dim = 1;
      *ligne = 0; *colonne = 0;
      ligne_1 = 0; colonne_1 = 0;
      count_dep += 1;
    } else if (dim_a == 0){ // Dimension nulle (mots de passe)
      int mdp_1 = 0, mdp_2 = 0, mdp_3 = 0;
      printf("Mot de passe n° 1 : ");
      if(scanf("%d",&mdp_1) == 1 && mdp_1 == 14076){
        while(getchar()!='\n');
        printf("Mot de passe n° 2 : ");
        if(scanf("%d",&mdp_2) == 1 && mdp_2 == 176634){
          while(getchar()!='\n');
          printf("Mot de passe n° 3 : ");
          if(scanf("%d",&mdp_3) == 1 && mdp_3 == 380649){
            printf("Vous revenez en dimension classique !\n\n");
            dim = 1;
            ligne_1 = 5; colonne_1 = 3; // Case 56
          } else {
            printf("Vous revenez au départ !\n\n");
            dim = 1;
            *ligne = 0; *colonne = 0;
            ligne_1 = 0; colonne_1 = 0;
            count_dep += 1;
          }
        } else {
          printf("Vous revenez au départ !\n\n");
          dim = 1;
          *ligne = 0; *colonne = 0;
          ligne_1 = 0; colonne_1 = 0;
          count_dep += 1;
        }
      } else {
        printf("Vous revenez au départ !\n\n");
        dim = 1;
        *ligne = 0; *colonne = 0;
        ligne_1 = 0; colonne_1 = 0;
        count_dep += 1;
      }
      while(getchar()!='\n');
    } else if (dim_a == -1){
      printf("Vous revenez au départ !\n");
      dim = 1;
      *ligne = 0; *colonne = 0;
      ligne_1 = 0; colonne_1 = 0;
      count_dep += 1;
    } else if (dim_a == -7){
      int mdp_1 = 0, mdp_2 = 0, mdp_3 = 0;
      printf("Mot de passe n° 4 : ");
      if(scanf("%d",&mdp_1) == 1 && mdp_1 == codes_mdp[0]){
        while(getchar()!='\n');
        printf("Mot de passe n° 5 : ");
        if(scanf("%d",&mdp_2) == 1 && mdp_2 == codes_mdp[1]){
          while(getchar()!='\n');
          printf("Mot de passe n° 6 : ");
          if(scanf("%d",&mdp_3) == 1 && mdp_3 == codes_mdp[2]){
            printf("Vous revenez en dimension classique, case 62 !\n\n");
            dim = 1;
            ligne_1 = 4; colonne_1 = 4; // Case 62
          } else {
            printf("Vous revenez au départ !\n\n");
            dim = 1;
            *ligne = 0; *colonne = 0;
            ligne_1 = 0; colonne_1 = 0;
            count_dep += 1;
          }
        } else {
          printf("Vous revenez au départ !\n\n");
          dim = 1;
          *ligne = 0; *colonne = 0;
          ligne_1 = 0; colonne_1 = 0;
          count_dep += 1;
        }
      } else {
        printf("Vous revenez au départ !\n\n");
        dim = 1;
        *ligne = 0; *colonne = 0;
        ligne_1 = 0; colonne_1 = 0;
        count_dep += 1;
      }
      while(getchar()!='\n');
    } else if (dim_a != 1){
      dim = case1(ligne,colonne,case_rand,recule,avance,dim_a,pas);
    }
  } else if (cases == 69){ // Case 69
    ParolesRick();
  } else if ((dim_a == -1 && cases < 0)) {
    dim = case1(ligne,colonne,rand_max() % 100,recule,avance,dim_a,pas);
  } else { // Autres cases
    dim = case1(ligne,colonne,case_rand,recule,avance,dim_a,pas);
  }
  if (rand_max() % 10 == 0){
    dim = utilisationCarteSecrete(dim);
  }
  if (rand_max() % chance == 0){
    dim = minageBitcoin(dim);
  }
  return dim;
}

// DIMENSIONS -2, -1, 2

void remplirTab(){
  for(int i = 0; i < TAILLE; i += 1){
    for(int j = 0; j < TAILLE; j += 1){
      tab2[i][j] = caseRandom();
    }
  }
}

int caseRandom2(){
  return -(rand_max() % 9)-1; // Entre -9 et -1
}

void remplirTab2(){
  for(int i = 0; i < TAILLE; i += 1){
    for(int j = 0; j < TAILLE; j += 1){
      tab4[i][j] = caseRandom2();
    }
  }
}

int caseRandom3(){
  return (rand_max() % 109)-9; // Entre -9 et 99
}
void remplirTab3(){
  for(int i = 0; i < TAILLE; i += 1){
    for(int j = 0; j < TAILLE; j += 1){
      tab5[i][j] = caseRandom3();
    }
  }
}

int evaluePolynome(int tab[],int val, int taille){
  int evalue = 0;
  for (int i = 0; i < taille; i += 1){
    evalue += tab[i] * pow(val,i);
  }
  return evalue;
}

int main() {
  codes_mdp_tot[0] = 14076;
  codes_mdp_tot[1] = 176634;
  codes_mdp_tot[2] = 380649;
  srand(time(NULL));
  time_t t1, t2;
  int dim = 1;
  // Dimension classique
  char dir_1 = 'D';
  char dir_1_rec = 'G';
  int recule_1 = 0;
  int avance_1 = 0;
  // Dimension non initialisée
  int ligne_2 = 0;
  int colonne_2 = 0;
  int recule_2 = 0;
  int avance_2 = 0;
  // Dimension nulle
  int ligne_0 = 0;
  int colonne_0 = 0;
  int recule_0 = 0;
  int avance_0 = 0;
  // Dimension négative
  int ligne_neg = 0;
  int colonne_neg = 0;
  int recule_neg = 0;
  int avance_neg = 0;
  // Backrooms
  int recule_back = 0;
  int avance_back = 0;
  // Imaginaire
  int ligne_i = 0;
  int colonne_i = 0;
  int recule_i = 0;
  int avance_i = 0;
  // Carcenac
  int ligne_c = 0;
  int colonne_c = 0;
  char dir_c = 'D';
  char dir_c_rec = 'G';
  int recule_c = 0;
  int avance_c = 0;
  // Chasse
  bool chasseJacob = false;
  int countJacob = 0;
  int countPlayer = 0;
  bool chasseJE = false;
  int countJE = 0;
  bool chasseCA = false;
  int countCA = 0;

  remplirTab();
  remplirTab2();
  remplirTab3();

  // Test
  ligne_1 = 0; colonne_1 = 0;
  ligne_c = 0; colonne_c = 0;
  count_dep = 0;
  dim = 1;
  count_lancers = 0;
  professeurC = false;

  bool victoire = false;
  bool premier_lancer = true;
  char de = '\0';

  for(int i = 0; i < 3; i += 1){
    codes_mdp[i] = rand_max();
    codes_mdp_tot[3+i] = codes_mdp[i];
  }
  while (!victoire){
    // Affichage de la grille
    if (dim == 1){
      afficheGrille(tab,8,ligne_1,colonne_1);
      printf("*-* Dimension classique - Case %d *-*\n\n",tab[ligne_1][colonne_1]);
    } else if (dim == 2){
      afficheGrille(tab2,8,ligne_2,colonne_2);
      printf("*-* Dimension non initilialisée - Case %d *-*\n\n",tab2[ligne_2][colonne_2]);
    } else if (dim == 0) {
      afficheGrille(tab3,8,ligne_0,colonne_0);
      printf("*-* Dimension nulle - Case %d *-*\n\n",tab3[ligne_0][colonne_0]);
    } else if (dim == -1){
      afficheGrille(tab4,8,ligne_neg,colonne_neg);
      printf("*-* Dimension négative - Case %d *-*\n\n",tab4[ligne_neg][colonne_neg]);
    } else if (dim == -2){
      afficheGrille(tab4,8,8,8);
      printf("  ??   ??   ??   ??   ??   ??   ??   ??\n\n");
      printf("*-* Backrooms *-*\n\n");
    } else if (dim == -3){
      printf("  ||   ||   ||   ||   ||   ||   ||   ||\n\n");
      afficheGrille(tab4,8,8,8);
      printf("  ||   ||   ||   ||   ||   ||   ||   ||\n\n");
      printf("*-* Prison de M. Jacob *-*\n\n");
    } else if (dim == 3){
      afficheGrilleIm(tab5,8,ligne_i,colonne_i);
      printf("*-* Dimension imaginaire - Case %di *-*\n\n",tab5[ligne_2][colonne_2]);
    } else if (dim == 4){
      afficheGrilleIm(tab5,8,8,8);
      printf("  ???   ???   ???   ???   ???   ???   ???   ???\n\n");
      printf("*-* Backrooms de Jean-Eudes *-*\n\n");
    } else if (dim == -4){
      afficheGrille(tab2,8,ligne_c,colonne_c);
      printf("*-* Dimension Carcenac - Case %d *-*\n\n",tab2[ligne_c][colonne_c]);
    } else if (dim == -5){
      afficheGrille(tab2,8,8,8);
      printf("  ??   ??   ??   ??   ??   ??   ??   ??\n\n");
      printf("*-* Backrooms de M. Carcenac *-*\n\n");
    } else if (dim == -6){
      printf("*-*-*\n| ? |\n*-*-*\n\n");
      printf("*-* ?? *-*\n\n");
    } else if (dim == -7){
      afficheGrilleFaure(tab,8,8,8);
      printf("*-* Backrooms de M. Faure *-*\n\n");
    } else if (dim == 6){
      afficheGrilleIm(tab5,8,8,8);
      printf("  ???   ???   ???   ???   ???   ???   ???   ???\n\n");
      printf("*-* Backrooms de M. Jansou *-*\n\n");
    } else if (dim == 7){
      printf("*-*-*\n| ? |\n*-*-*\n\n");
      printf("*-* %d *-*\n\n",rand_max());
    } else if (dim == -8){
      printf("*-*-*\n| ? |\n*-*-*\n\n");
      printf("*-* Dimension de Schrödinger *-*\n\n");
    } else if (dim == 5){
      printf("*-**-*\n| XX |\n*-**-*\n\n");
      printf("*-* Dimension de redirection *-*\n\n");
    } 
    // Explication des règles
    if (!decouverteDC && dim == 1){
      printf("Bienvenue dans le Jeu de l'Oie revisité (appelé aussi Les Sept 1/2 Dimensions de l'Enfer) !\n");
      printf("L'objectif est très simple : atteindre la case 63 !\n");
      printf("Cependant, de nombreux obstacles ralentiront votre route !\n");
      printf("Vous pouvez également emprunter fortuitement des portails qui vous emmèneront dans des dimensions parallèles !\n");
      printf("Si vous vous retrouvez dans ces endroits (ce qui est fort probable), votre objectif sera de revenir dans la dimension classique, malgré le désordre qui y règne !\n");
      printf("Pour autant, si vous revenez au départ, les probabilités pour les portails seront modifiées en votre faveur !\n");
      printf("Conseil : notez les mots de passe que vous rencontrerez !\n");
      printf("Bon jeu, bonne chance !\n\n");
      printf("PS : vous pouvez appuyer sur e au lancer de dé pour avoir des informations sur votre partie en cours !\n\n");
      decouverteDC = true;
    } else if (!decouverteDNI && dim == 2){
      printf("Bienvenue dans la dimension non initialisée !\n");
      printf("Comme vous pourrez le constater, les mouvements y sont aléatoires, tout comme le numéro des cases !\n");
      printf("Votre objectif est de sortir de cette dimension par les portails, malgré les nombreux travaux qui bloquent une grande partie de ceux-ci !\n\n");
      decouverteDNI = true;
    } else if (!decouverteDN && dim == 0){
      printf("Bienvenue dans la dimension nulle !\n");
      printf("Comme vous pouvez le voir, cette dimension ne comporte que des cases 00.\n");
      printf("Ces cases sont aléatoires.\n");
      printf("Votre objectif : trouver tous les mots de passe pour les entrer lorsque vous tombez sur une case demandant un mot de passe, sous peine de revenir au début.\nCependant, des portails peuvent vous ramener à la dimension non initialisée.\n\n");
      decouverteDN = true;
    } else if (!decouverteDSN && dim == -1 && decouverteDNI){
      printf("Bienvenue dans la dimension négative !\n");
      printf("Il s'agit d'une dimension similaire à la dimension non initialisée mais avec quelques modifications.\n");
      printf("Votre objectif : sortir par un portail !\n\n");
      decouverteDSN = true;
    } else if (!decouverteDSN && dim == -1 && !decouverteDNI){
      printf("Bienvenue dans la dimension négative !\n");
      printf("Comme vous pourrez le constater, les mouvements y sont aléatoires, tout comme le numéro des cases !\n");
      printf("Votre objectif est de sortir de cette dimension par les portails, malgré les nombreux travaux qui bloquent une grande partie de ces portails !\n\n");
      printf("Votre objectif : sortir par un portail !\n\n");
      decouverteDSN = true;
    } else if (!decouverteBR && dim == -2){
      printf("Bienvenue dans les Backrooms !\n");
      printf("Vous vous situez dans une dimension avec un espace totalement tordu, donc il est impossible de vous localiser !\n");
      printf("Votre objectif : sortir par la chance, sans tomber sur un policier un peu particulier.\n\n");
      decouverteBR = true;
    } else if (!decouvertePJ && dim == -3){
      printf("Bienvenue dans la prison de M. Jacob !\n");
      printf("Le temps s'est arrêté, la dimension négative ne change plus.\n");
      printf("Vous avez été jugé criminel par une erreur judiciaire (je crois) et emmené dans cette prison !\n");
      printf("Votre objectif : s'évader avec le professeur C.\n\n");
      decouvertePJ = true;
    } else if (!decouverteIM && dim == 3){
      printf("Bienvenue dans la dimension imaginaire !\n");
      printf("Cette dimension contient des imaginaires purs.\n");
      printf("Votre objectif : gagner.\n\n");
      decouverteIM = true;
    } else if (!decouverteJE && dim == 4){
      printf("Bienvenue dans les Backrooms de Jean-Eudes !\n");
      printf("Vous avez été attrapé par Jean-Eudes et vous avez été emmené dans son monde licornesque.\n");
      printf("Votre objectif : lui faire un câlin.\n\n");
      decouverteJE = true;
    } else if (!decouverteCA && dim == -4){
      printf("Bienvenue dans la dimension de M. Carcenac !\n");
      printf("Cette dimension retrouve les mouvements de la dimension classique... mais avec des numéros aléatoires.\n");
      printf("Votre objectif : gagner.\n\n");
      decouverteCA = true;
    } else if (!decouverteBC && dim == -5){
      printf("Bienvenue dans les Backrooms de M. Carcenac !\n");
      printf("Vous avez été attrapé et emmené dans une dimension avec une possibilité de gagner grâce à la fin alternative !\n");
      printf("Votre objectif : gagner avec la fin alternative.\n\n");
      decouverteBC = true;
    } else if (!decouverteDE && dim == -6){
      printf("Bienvenue dans un endroit sombre, lugubre, non identifiable...\n");
      printf("Bref, tous les adjectifs sont bons pour décrire cet endroit inconnu (tout ce que je peux dire c'est que dans le code c'est la dimension -6).\n");
      printf("Objectif : découvrir où on est et sortir de là.\n\n");
      decouverteDE = true;
    } else if (!decouverteFA && dim == -7){
      printf("Bienvenue dans les backrooms de M. Faure !\n");
      printf("Comme il le dit si bien : c'est pas cher !\n");
      printf("Votre objectif : gagner enfin je crois\n\n");
      decouverteFA = true;
    } else if (!decouverteJA && dim == 6){
      printf("Bienvenue dans les backrooms de M. Jansou !\n");
      printf("Ici, tout est clair !\n");
      printf("Votre objectif : je sais pas\n\n");
      decouverteJA = true;
    } else if (!decouverteTR && dim == 7){
      printf("Bienvenue dans le fond du trou !\n");
      printf("L'objectif est très simple : retourner dans les backrooms de M. Jansou.\n");
      printf("Attention ! Les lettres vont se transform%d%d...\n\n",rand_max(),rand_max());
      decouverteTR = true;
    } else if (!decouverteBO && dim == -8){
      printf("Bienvenue dans la dimension de Schrödinger (on doit être près de la dimension Faure) !\n");
      printf("L'objectif est très simple : avancer technologiquement pour connaître votre état quantique pour savoir si vous avez gagné !\n");
      printf("Vous minez du bitcoin à chaque lancer de dé !\n\n");
      decouverteBO = true;
    } else if (!decouverteRE && dim == 5){
      printf("Bienvenue dans la dimension de redirection !\n");
      printf("Vous vous trouvez actuellement devant le guichet d'attente pour l'entrée aux Backrooms (l'attente est très longue...).\n");
      printf("Vous pouvez cependant être ramené en dimension négative !\n\n");
      printf("Objectif : supporter les cases d'attente et sortir de là !\n\n");
      decouverteRE = true;
    }
    // Lancer de dé
    if (dim == 7) printf("%d d %d : ",rand_max(),rand_max());
    else printf("Entrez d pour lancer le dé : ");
    while (scanf("%c",&de) != 1 || (de != 'd' && de != 'e')){
      while (getchar() != '\n');
      if (dim == 7) printf("%d d %d : ",rand_max(),rand_max());
      else printf("Entrez d pour lancer le dé : ");
    }
    while (getchar() != '\n');
    if (premier_lancer){
      t1 = time(NULL);
      premier_lancer = false;
    }
    if (de == 'e'){
      printf("Dimension : %d\n",dim);
      printf("Retours au départ : %d\n",count_dep);
      printf("Case en dimension classique : %d\n",tab[ligne_1][colonne_1]);
      printf("Lancers de dé : %d\n",count_lancers+1);
      printf("Professeur C sauvé : %d\n",professeurC);
      printf("Multiplication d'argent : %f\n",money_give);
      printf("Multiplication de points : %f\n",points_give);
      printf("Secondes en moins : %f\n",second_less);
      printf("Argent : %f,%f,%f,%f\n",argent.r,argent.i,argent.j,argent.c);
      printf("Argent sauvegardé : %f,%f,%f,%f\n",argent_save.r,argent_save.i,argent_save.j,argent_save.c);
      printf("Points : %f\n",points_count);
      if (bitcoins > 0) {
        printf("Bitcoins : %d\n",bitcoins);
        printf("Pioche : +%d par minage\n",pioche);
        printf("Chance de miner un bitcoin : 1/%d\n",chance);
      }
      if (decouverteRE) printf("Points d'attente (Redirection) : %d\n",points_attente);
      printf("Cartes : ");
      for(int i = 0; i < 30; i += 1){
        if (cartes[i] > 0){
          if (i < 25) printf("| %d ",i+1);
          else printf("| S%d ",i-24);
        }
      }
      printf("|\n");
      printf("Temps de jeu : %ld\n",time(NULL)-t1);
      printf("Mots de passe : ");
      for (int i = 0; i < 6; i += 1){
        if (mdp[i]) printf("| %d ",i+1);
      }
      printf("|\n");
    }
    printf("\n");
    count_lancers += 1;
    if (dim == 1){ // Dimension clasique
      int pas_11 = lancer();
      avancer(pas_11,&ligne_1,&colonne_1,&dir_1,&dir_1_rec,false);
      afficheGrille(tab,8,ligne_1,colonne_1);
      printf("*-* Dimension classique - Case %d *-*\n",tab[ligne_1][colonne_1]);
      printf("Vous avez avancé de %d case(s)\n\n",pas_11);
      dim = case1(&ligne_1,&colonne_1,tab[ligne_1][colonne_1],&recule_1,&avance_1,1,pas_11); // Case (action)
      avancer(avance_1,&ligne_1,&colonne_1,&dir_1,&dir_1_rec,false);
      reculer(recule_1,&ligne_1,&colonne_1,&dir_1,&dir_1_rec,false);
      if (rand_max() % (6 + 2*count_dep) == 0){ // Téléportation aléatoire
        printf("Vous passez en dimension non initialisée (Pas de chance) !\n\n");
        dim = 2;
      }
    } else if (dim == 2){ // Dimension non initialisée
      int pas_12 = lancer();
      avancerCh(pas_12,&ligne_2,&colonne_2,false);
      afficheGrille(tab2,8,ligne_2,colonne_2);
      printf("*-* Dimension non initialisée - Case %d *-*\n",tab2[ligne_2][colonne_2]);
      printf("Vous avez avancé de %d case(s)\n\n",pas_12);
      dim = case1(&ligne_2,&colonne_2,tab2[ligne_2][colonne_2],&recule_2,&avance_2,2,pas_12); // Case (action)
      avancerCh(avance_2,&ligne_2,&colonne_2,false);
      avancerCh(recule_2,&ligne_2,&colonne_2,false);
      remplirTab(); // Changement de la grille aléatoirement
      if (dim != 2){
        // Réinitialisation de la position du joueur en cas de sortie de la dimension
        ligne_2 = 0; colonne_2 = 0;
      }
    } else if (dim == 0){
      int pas_10 = lancer();
      avancerCh(pas_10,&ligne_0,&colonne_0,false);
      afficheGrille(tab3,8,ligne_0,colonne_0);
      printf("*-* Dimension nulle - Case %d *-*\n",tab3[ligne_0][colonne_0]);
      printf("Vous avez avancé de %d case(s)\n\n",pas_10);
      dim = case1(&ligne_0,&colonne_0,tab3[ligne_0][colonne_0],&recule_0,&avance_0,0,pas_10);
      avancerCh(avance_0,&ligne_0,&colonne_0,false);
      avancerCh(recule_0,&ligne_0,&colonne_0,false);
      if (dim != 0){
        ligne_0 = 0; colonne_0 = 0;
      }
    } else if (dim == -1){
      int pas_1n = lancer();
      avancerCh(pas_1n,&ligne_neg,&colonne_neg,false);
      afficheGrille(tab4,8,ligne_neg,colonne_neg);
      printf("*-* Dimension négative - Case %d *-*\n",tab4[ligne_neg][colonne_neg]);
      printf("Vous avez avancé de %d case(s)\n\n",pas_1n);
      dim = case1(&ligne_neg,&colonne_neg,tab4[ligne_neg][colonne_neg],&recule_neg,&avance_neg,-1,pas_1n);
      avancerCh(avance_neg,&ligne_neg,&colonne_neg,false);
      avancerCh(recule_neg,&ligne_neg,&colonne_neg,false);
      remplirTab2();
      if (dim != -1){
        ligne_neg = 0; colonne_neg = 0;
      }
    } else if (dim == -2){
      int pas_1b = lancer();
      int pas_2b = lancer();
      afficheGrille(tab4,8,8,8); // Affichage de la grille sans la position du joueur (trop loin en bas à droite)
      printf("  ??   ??   ??   ??   ??   ??   ??   ??\n\n");
      printf("*-* Backrooms - Case ?? (%d) *-*\n",rand_max());
      printf("Vous avez avancé de %d case(s)\n\n",pas_1b);
      // Chasse par M. Jacob
      if (pas_1b == 6 && pas_2b >= 3 && chasseJacob == false){
        printf("Vous sortez des Backrooms !\n\n");
        dim = -1;
      } else if (rand_max() % 10 == 0 && chasseJacob == false){
        if (!professeurC) printf("Vous êtes chassé par M. Jacob ! Vous devez lui échapper pendant 10 tours sous peine de finir dans sa prison où vous pouvez retrouver un personnage familier ! (Il avance de 2 cases par tour)\n\n");
        else printf("Vous êtes de nouveau chassé par M. Jacob ! Vous devez lui échapper pendant 10 tours sous peine de finir dans sa prison. (Il avance de 2 cases par tour)\n\n");
        chasseJacob = true;
      } else if (chasseJacob == true){
        countJacob += 2;
        countPlayer += pas_1b;
        if (countPlayer <= countJacob){
          if (!professeurC) printf("Vous avez été rattrapé par M. Jacob ! Vous entrez dans sa prison ! Vous retrouvez le professeur C. qui a disparu en randonnée !\n\n");
          else printf("Vous avez été rattrapé par M. Jacob !\n\n");
          chasseJacob = false;
          dim = -3;
          countJacob = 0;
          countPlayer = 0;
        } else if (countJacob == 20){
          printf("Vous avez échappé à M. Jacob ! Vous continuez à chercher la sortie des Backrooms !\n\n");
          chasseJacob = false;
          countJacob = 0;
          countPlayer = 0;
        } else {
          printf("Vous êtes à %d cases d'avance de M. Jacob\n\n",countPlayer-countJacob);
        }
      } else {
        dim = case1(&ligne_neg,&colonne_neg,0,&recule_back,&avance_back,-2,pas_1b);
      }
      remplirTab2();
    } else if (dim == -3){
      int pas_1j = lancer();
      int pas_2j = lancer();
      printf("  ||   ||   ||   ||   ||   ||   ||   ||\n\n");
      afficheGrille(tab4,8,8,8);
      printf("  ||   ||   ||   ||   ||   ||   ||   ||\n\n");
      printf("*-* Prison de M. Jacob - Case ?? (%d) *-*\n",rand_max());
      printf("Vous avez avancé de %d case(s)\n\n",pas_1j);
      if (pas_1j == 6 && pas_2j == 6){
        if (!professeurC) printf("Vous sortez de la prison de M. Jacob ! Vous avez sauvé le professeur C. !\n\n");
        else printf("Vous sortez de la prison de M. Jacob !\n\n");
        professeurC = true;
        dim = -2;
      } else if (rand_max() % 200 == 0) {
        if (!professeurC) printf("Vous sortez de la prison de M. Jacob ! Vous avez sauvé le professeur C. !\n\n");
        else printf("Vous sortez de la prison de M. Jacob !\n\n");
        professeurC = true;
        dim = 6;
      } else {
        dim = case1(&ligne_neg,&colonne_neg,0,&recule_back,&avance_back,-3,pas_1j);
      }
    } else if (dim == 3){
      int pas_1i = lancer();
      avancerCh(pas_1i,&ligne_i,&colonne_i,false);
      afficheGrilleIm(tab5,8,ligne_i,colonne_i);
      printf("*-* Dimension imaginaire - Case %di *-*\n",tab5[ligne_i][colonne_i]);
      printf("Vous avez avancé de %d case(s)\n\n",pas_1i);
      // Chasse par Jean-Eudes
      if (rand_max() % 63 == 0 && chasseJE == false){
        printf("Vous êtes chassé par Jean-Eudes ! Vous devez lui échapper pendant 10 tours sous peine de finir dans les backrooms licornesques. (Il avance de 4 cases par tour)\n\n");
        chasseJE = true;
      } else if (chasseJE == true){
        countJE += 4;
        countPlayer += pas_1i;
        if (countPlayer <= countJE){
          printf("Vous avez été rattrapé par Jean-Eudes !\n\n");
          chasseJE = false;
          dim = 4;
          countJE = 0;
          countPlayer = 0;
        } else if (countJE == 40){
          printf("Vous avez échappé à Jean-Eudes !\n\n");
          chasseJE = false;
          countJE = 0;
          countPlayer = 0;
        } else {
          printf("Vous êtes à %d cases d'avance de Jean-Eudes\n\n",countPlayer-countJE);
        }
      } else {
        dim = case1(&ligne_i,&colonne_i,rand_max()%63,&recule_i,&avance_i,3,pas_1i);
      }
      avancerCh(avance_i,&ligne_i,&colonne_i,false);
      avancerCh(recule_i,&ligne_i,&colonne_i,false);
      if (dim != 3){
        ligne_i = 7; colonne_i = 7;
      }
    } else if (dim == 4){
      int pas_1je = lancer();
      int pas_2je = lancer();
      int rep = 0;
      printf("*-* Backrooms de Jean-Eudes - Case ?? (%d) *-*\n",rand_max());
      printf("Vous avez avancé de %d case(s)\n\n",pas_1je);
      if (pas_1je == 6 && pas_2je == 6){
        printf("Acceptez-vous de faire un câlin à Jean-Eudes ?\n");
        printf("1 - Oui\n");
        printf("2 - Non\n");
        while(scanf("%d",&rep) != 1){
          printf("Vous acceptez ? ");
          while(getchar()!='\n');
        }
        while(getchar()!='\n');
        if (rep == 1){
          printf("Vous faites un câlin à Jean-Eudes...\n\n");
          printf("Vous retournez en dimension classique, case 62\n\n");
          ligne_1 = 4; colonne_1 = 4;
          dim = 1;
          count_dep += 1;
        } else {
          printf("Vous allez dans la prison de M. Jacob, dans les Backrooms, et votre progression dans la dimension classique est réinitialisée (car vous pas gentil).\n\n");
          dim = -3;
          ligne_1 = 0; colonne_1 = 0;
        }
      } else {
        dim = case1(&ligne_i,&colonne_i,0,&recule_i,&avance_i,4,pas_1je);
      }
    } else if (dim == -4){
      int pas_1c = lancer();
      avancer(pas_1c,&ligne_c,&colonne_c,&dir_c,&dir_c_rec,false);
      afficheGrille(tab2,8,ligne_c,colonne_c);
      printf("*-* Dimension Carcenac - Case %d *-*\n",tab2[ligne_c][colonne_c]);
      printf("Vous avez avancé de %d case(s)\n\n",pas_1c);
      // Chasse par M. Carcenac
      if (rand_max() % 63 == 0 && chasseCA == false){
        printf("Vous êtes chassé par M. Carcenac ! Vous devez lui échapper pendant 10 tours sous peine de finir dans les backrooms de Carcenac. (Il avance de 4 cases par tour)\n\n");
        chasseCA = true;
      } else if (chasseCA == true){
        countCA += 4;
        countPlayer += pas_1c;
        if (countPlayer <= countCA){
          printf("Vous avez été rattrapé par M. Carcenac !\n\n");
          chasseCA = false;
          dim = -5;
          countCA = 0;
          countPlayer = 0;
        } else if (countCA == 40){
          printf("Vous avez échappé à M. Carcenac !\n\n");
          chasseCA = false;
          countCA = 0;
          countPlayer = 0;
        } else {
          printf("Vous êtes à %d cases d'avance de M. Carcenac\n\n",countPlayer-countCA);
        }
      } else {
        dim = case1(&ligne_c,&colonne_c,tab2[ligne_c][colonne_c],&recule_c,&avance_c,-4,pas_1c);
      }
      avancer(avance_c,&ligne_c,&colonne_c,&dir_c,&dir_c_rec,false);
      reculer(recule_c,&ligne_c,&colonne_c,&dir_c,&dir_c_rec,false);
    } else if (dim == -5){
      int pas_1ca = lancer();
      int pas_2ca = lancer();
      printf("*-* Backrooms de M. Carcenac - Case ?? (%d) *-*\n",rand_max());
      printf("Vous avez avancé de %d case(s)\n\n",pas_1ca);
      if (pas_1ca == 6 && pas_2ca == 6){
        if (professeurC){
          printf("Bon allez, on va faire un peu de hors programme !\n");
          printf("BRAVO ! Vous avez gagné (hors programme Carcenac) (fin n°2) ! (Désolé pour ce jeu d'horreur)\n\n");
          victoire = true;
        } else {
          printf("Vous retournez en dimension classique, case 62 !\n\n");
          dim = 1;
          ligne_1 = 4; colonne_1 = 4;
        }
      } else if (pas_1ca == 1 && pas_2ca == 1){
        printf("Vous retournez en dimension Carcenac !\n\n");
        dim = -4;
      } else {
        dim = case1(&ligne_c,&colonne_c,0,&recule_c,&avance_c,-5,pas_1ca);
      }
    } else if (dim == -6){
      int pas_1de = lancer();
      int pas_2de = lancer();
      printf("*-* Quelque part - Case ?? (%d) *-*\n",rand_max());
      printf("Vous avez avancé de %d case(s) (de toute façon ça sert à rien)\n\n",pas_1de);
      if (pas_1de == 6 && pas_2de == 6){
          printf("Vous êtes chassé par quelqu'un... il a l'air de venir d'un endroit contenant une kyrielle de machines !\n");
          printf("Ah bah il vous a attrapé...\n");
          dim = -7;
      } else {
          dim = case1(&ligne_c,&colonne_c,0,&recule_c,&avance_c,-6,pas_1de);
      }
    } else if (dim == -7){
        int pas_1fa = lancer();
        int pas_2fa = lancer();
        int fa_rand = rand_max() % 100;
        printf("*-* Backrooms de M. Faure - Case ?? (%d) *-*\n",rand_max());
        printf("Vous avez avancé de %d case(s) (de toute façon ça sert à rien et c'est pas cher)\n\n",pas_1fa);
        if (fa_rand == 0){
            if (professeurC){
                printf("C'est la fin du cours (même s'il est 11h50 mais ça a sonné donc ça va) ! Vous sortez du programme et vous éteignez !\n\n");
                printf("BRAVO ! Vous avez gagné (fin n°6) (fin M. Faure) ! (Désolé pour ce jeu d'horreur)\n\n");
                victoire = true;
            } else {
                printf("Vous retournez en dimension classique (case 62) !\n\n");
                colonne_1 = 4; ligne_1 = 4;
                dim = 1;
            }
        } else if (fa_rand == 1 || fa_rand == 7) { 
          printf("Vous êtes téléporté dans les backrooms de M. Jansou !\n\n");
          dim = 6;
        } else {
          dim = case1(&ligne_c,&colonne_c,0,&recule_c,&avance_c,-7,pas_1fa);
        }
    } else if (dim == 6){
      int pas_1ja = lancer();
      int ja_rand = rand_max() % 100;
      printf("*-* Backrooms de M. Jansou - Case ?? (%d) *-*\n",rand_max());
      printf("Vous avez avancé de %d case(s)\n\n",pas_1ja);
      if (ja_rand == 0){
        if (professeurC){
          printf("C'est fini ! (Dans quelques instants, le TP d'informatique)\n\n");
          dim = -5;
        } else {
          int polynome[4]; // Degrés croissants
          int reponse[4];
          polynome[0] = 7;
          for(int i = 1; i < 4; i += 1){
            polynome[i] = (rand_max() % 11) - 10;
          }
          printf("Trouver un polynôme de degré au plus 3 (écrire : 'a,b,c,d' avec a,b,c,d entiers relatifs) tel que :\n");
          printf("P(-2) = %d\n",evaluePolynome(polynome,-2,4));
          printf("P(0) = 7\n");
          printf("P(1) = %d\n",evaluePolynome(polynome,1,4));
          printf("P(2) = %d\n",evaluePolynome(polynome,2,4));
          while(scanf("%d,%d,%d,%d",&reponse[0],&reponse[1],&reponse[2],&reponse[3]) != 4){
            while(getchar()!='\n');
          }
          while(getchar()!='\n');
          if ((reponse[0] == polynome[3]) && (reponse[1] == polynome[2]) && (reponse[2] == polynome[1]) && (reponse[3] == polynome[0])){
            printf("C'est la bonne réponse ! Vous êtes téléporté dans les backrooms de M. Carcenac !\n\n");
            dim = -5;
          } else {
            printf("Mauvaise réponse... Vous êtes téléporté dans le fond du trou...\n\n");
            dim = 7;
          }
        }
      } else if (ja_rand == 1) { 
        // Vers Backrooms Faure, Carcenac, tout court, Jean-Eudes, Reste
        int ja_rand_dim = rand_max() % 7;
        printf("Téléportation dans des Backrooms aléatoires :\n\n");
        if (ja_rand_dim == 0) dim = -7;
        else if (ja_rand_dim == 1) dim = -5;
        else if (ja_rand_dim == 2) dim = -2;
        else if (ja_rand_dim == 3) dim = -3;
        else if (ja_rand_dim == 4) dim = 4;
        else if (ja_rand_dim == 5) dim = 7;
        else if (ja_rand_dim == 6) {
          dim = 6;
          decouverteJA = false;
        }
      } else if (ja_rand == 2 || ja_rand == 3) { 
        printf("Vous tombez au fond du trou...\n\n");
        dim = 7;
      } else {
        dim = case1(&ligne_c,&colonne_c,0,&recule_c,&avance_c,6,pas_1ja);
      }
    } else if (dim == 7){
      int pas_1tr = lancer();
      int pas_2tr = lancer();
      printf("*-* %d - %d ?? (%d) *-*\n",rand_max(),rand_max(),rand_max());
      printf("%d %d %d\n\n",rand_max(),pas_1tr,rand_max());
      if (pas_1tr == 6 && pas_2tr == 6){
          printf("Vous revenez dans les backrooms de M. Jansou !\n\n");
          dim = 6;
      } else if (pas_1tr == 1 && pas_2tr == 1 && rand_max()%4 == 0 && !professeurC){
          printf("BRAVO ! Vous avez gagné (fin n°7) (fond du trou) ! (Désolé pour ce jeu d'horreur)\n\n");
          victoire = true;
      } else {
          dim = case1(&ligne_c,&colonne_c,0,&recule_c,&avance_c,7,pas_1tr);
      }
    } else if (dim == -8){
      int pas_1bo = lancer();
      printf("*-* Dimension de Schrödinger - Case ?? (%d) *-*\n",rand_max());
      printf("Vous avez avancé de %d case(s) (de toute façon ça sert à rien)\n\n",pas_1bo);
      dim = case1(&ligne_c,&colonne_c,0,&recule_c,&avance_c,-8,pas_1bo);
    } else if (dim == 5){
      printf("*-* Dimension de redirection *-*\n");
      int jeu = rand_max() % 3;
      if (jeu == 0){
        int t_attente = (rand_max() % (100 - 5*count_dep)) + 1;
        points_attente += t_attente;
        if (points_attente >= 300*(count_attente+1)){
          printf("Les probabilités sont augmentées ! (équivalent à un retour au départ)\n\n");
          count_attente += 1;
          count_dep += 1;
        }
        if (points_attente >= 3600){
          dim = 13;
        }
        MalusTP(t_attente,5); // De 1 à 100 secondes
      } else if (jeu == 1) {
        chaudFroid(5);
      } else {
        int justePrix3;
        bool jp_3 = JustePrix(5,13,2000,&justePrix3);
        int rand_prix = rand_max() % 4;
        if (jp_3){
          if (rand_prix == 0){
            printf("Vous gagnez %f d'argent !\n\n",justePrix3*money_give);
            argent.r += justePrix3*money_give;
          } else if (rand_prix == 1){
            if (decouvertePJ) printf("M. Jacob gagne %f d'argent !\n\n",justePrix3*money_give);
            else printf("??? gagne %f d'argent !\n\n",justePrix3*money_give);
            argent.j += justePrix3*money_give;
          } else if (rand_prix == 2){
            if (decouverteJE) printf("Jean-Eudes gagne %f d'argent !\n\n",justePrix3*money_give);
            else printf("??? gagne %f d'argent !\n\n",justePrix3*money_give);
            argent.i += justePrix3*money_give;
          } else {
            if (decouverteBC) printf("M. Carcenac gagne %f d'argent !\n\n",justePrix3*money_give);
            else printf("??? gagne %f d'argent !\n\n",justePrix3*money_give);
            argent.c += justePrix3*money_give;
          }
        } else {
          printf("Vous avez perdu...\n\n");
          MalusTP(100,5);
        }
      }
      if (rand_max() % 20 == 0){
        cartesTirage(rand_max()%1001,(rand_max()%10)-5);
      }
      if (rand_max() % 10 == 0){
        dim = utilisationCarteSecrete(5);
      }
      if (rand_max() % 10 == 0){
        dim = changementDim2();
      }
    }
    if (dim == 10){
      printf("BRAVO ! Vous avez gagné (fin n°5) (carte secrète) ! (Désolé pour ce jeu d'horreur)\n\n");
      victoire = true;
    } else if (dim == 11){
      printf("Bon allez, on va faire un peu de hors programme !\n");
      printf("BRAVO ! Vous avez gagné (500 points et backrooms de M. Carcenac) (fin n°8) ! (Désolé pour ce jeu d'horreur)\n\n");
      victoire = true;
    } else if (dim == 12){
      printf("BRAVO ! Vous avez acquis 500 000 bitcoins pour rechercher une technologie et vous avez gagné !\n");
      printf("Désolé pour ce jeu d'horreur (fin n°9) !\n\n");
      victoire = true;
    } else if (dim == 13){
      printf("BRAVO ! Vous avez gagné (fin n°10) (1 h d'attente en dimension de redirection) ! (Désolé pour ce jeu d'horreur)\n\n");
    }

    avance_0 = 0; recule_0 = 0;
    avance_1 = 0; recule_1 = 0;
    avance_2 = 0; recule_2 = 0;
    avance_neg = 0; recule_neg = 0;
    avance_back = 0; recule_back = 0;
    avance_i = 0; recule_i = 0;
    avance_c = 0; recule_c = 0;
    if (ligne_1 == 4 && colonne_1 == 3){
      victoire = true;
      printf("BRAVO ! Vous avez gagné (fin n°1) ! (Désolé pour ce jeu d'horreur)\n\n");
      if (professeurC) printf("Objectif secondaire accompli : Sauver le professeur C !\n");
    }
    if ((argent.r + argent_save.r) >= 10000 && (argent.i + argent_save.i) >= 10000 && (argent.j + argent_save.j) >= 10000 && (argent.c + argent_save.c) >= 10000){
      printf("BRAVO ! Vous avez gagné (fin n°3) (10000 d'argent pour chacun) ! (Désolé pour ce jeu d'horreur)\n\n");
      victoire = true;
    }
    // Vérification : toutes les cartes sont collectionnées (sauf les secrètes)
    bool all_cartes = true;
    for (int i = 0; i < 25; i += 1){
      if (cartes[i] == 0) all_cartes = false;
    }
    if (all_cartes){
      printf("BRAVO ! Vous avez gagné (fin n°4) (toutes les cartes collectionnées sauf secrètes) ! (Désolé pour ce jeu d'horreur)\n\n");
    }
  }
  t2 = time(NULL);
  // Statistiques de fin de jeu
  int tf_heures = (t2-t1)/3600;
  int tf_minutes = ((t2-t1)%3600)/60;
  int tf_secondes = ((t2-t1)%3600)%60;
  printf("Votre temps de jeu : %d heures %d minutes %d secondes\n",tf_heures,tf_minutes,tf_secondes);
  printf("Nombre de lancers de dé : %d\n",count_lancers);
  printf("Retours au départ : %d\n",count_dep);
  printf("Argent : (%f,%f,%f,%f)\n",argent.r+argent_save.r,argent.i+argent_save.i,argent.j+argent_save.j,argent.c+argent_save.c);
  printf("Argent non sauvegardé : %f,%f,%f,%f\n",argent.r,argent.i,argent.j,argent.c);
  printf("Argent sauvegardé : %f,%f,%f,%f\n",argent_save.r,argent_save.i,argent_save.j,argent_save.c);
  printf("Dimension : %d\n",dim);
  printf("Case en dimension classique : %d\n",tab[ligne_1][colonne_1]);
  printf("Professeur C sauvé : %d\n",professeurC);
  printf("Multiplication d'argent : %f\n",money_give);
  printf("Multiplication de points : %f\n",points_give);
  printf("Secondes en moins : %f\n",second_less);
  printf("Points : %f\n",points_count);
  if (bitcoins > 0) {
    printf("Bitcoins : %d\n",bitcoins);
    printf("Pioche : +%d par minage\n",pioche);
    printf("Chance de miner un bitcoin : 1/%d\n",chance);
  }
  if (decouverteRE) printf("Points d'attente (Redirection) : %d\n",points_attente);
  printf("Cartes : ");
  for(int i = 0; i < 30; i += 1){
    if (cartes[i] > 0){
      if (i < 25) printf("| %d ",i+1);
      else printf("| S%d ",i-24);
    }
  }
  printf("|\n");
  printf("Mots de passe : ");
  for (int i = 0; i < 6; i += 1){
    if (mdp[i]) printf("| %d : %d ",i+1,codes_mdp_tot[i]);
  }
  printf("|\n");
  return 0;
}