#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_types.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>
#include <stdarg.h>
#include <math.h>
#define WIDTH 512
#define HEIGHT 600 
#define LC 96 //taille carre

//gcc -o projet2048 projet2048.c `sdl-config --libs --cflags`
//SDL///////////////////////////////
void bouclePartie();
char boucleMenu();
void dessine(SDL_Surface * f);
//declaration de variables pour stoquer les coordonees des images 
SDL_Rect position;
SDL_Rect positionTemp;
SDL_Rect positionEcran;
SDL_Rect positionScore;
SDL_Rect positionGameover;
//declaration de variables pour chaque image
SDL_Surface * ecran = NULL;
SDL_Surface * carre1=NULL;
SDL_Surface * carre2=NULL;
SDL_Surface * carre3=NULL;
SDL_Surface * carre4=NULL;
SDL_Surface * carre5=NULL;
SDL_Surface * carre6=NULL;
SDL_Surface * carre7=NULL;
SDL_Surface * carre8=NULL;
SDL_Surface * carre9=NULL;
SDL_Surface * carre10=NULL;
SDL_Surface * carre11=NULL;
SDL_Surface * carre12=NULL;
SDL_Surface * carre13=NULL;
SDL_Surface * carre14=NULL;
SDL_Surface * carre15=NULL;
SDL_Surface * carre16=NULL;
SDL_Surface * score1=NULL;
SDL_Surface * score2=NULL;
SDL_Surface * score3=NULL;
SDL_Surface * score4=NULL;
SDL_Surface * score5=NULL;
SDL_Surface * score6=NULL;
SDL_Surface * ecranGameover=NULL;
SDL_Surface * fondecran=NULL;

//Structure pour tableau et score de la partie
struct partie{
	int ** T; // Tableau 
	int ** V; //Tableau pour les coordonnees des espaces vides
	int S; //points
	int v; //Quantite d'espaces vides
	char ** C;//chemin pour les images des valeurs
	int * B;//blockage verifier si n'a plus de mouvement = gameover
};

//AficherTableau actuel et score de la partie
struct partie afficherTableau(struct partie P){
	int i, j, w;
	
	printf("\n\nPoints: %d\n\n", P.S);
	for(i=0;i<4;i++){
		for(j=0;j<4;j++){
			if(P.T[i][j]!=0){
				if(P.T[i][j]<10){printf(" ");}; //aligner les lignes
				printf("  %d", P.T[i][j]);
			}else{
				printf("   X"); //afficher X aux cases vides pour faciliter visualisation
			}
			if(j==3){printf("\n\n");} //sauter ligne
		}
	}
	printf("\n");


	//IMPLEMENTATION SDL
	//concatenation des valeurs de chaque espace avec son image (chemin img/VALEUR.bmp)
	w=0;
	char temp[20];
	for(i=0;i<4;i++){
		for(j=0;j<4;j++){
				sprintf(temp, "img/%d.bmp", P.T[i][j]);
				strcpy(&P.C[w][0],temp);//sauvegarder chemin dans te tableau C(chemin)
				w++;
		}
	}
	fondecran=SDL_LoadBMP("img/fond1.bmp");

	//Envoyer le chemin des image de chaque carre
	carre1=SDL_LoadBMP(&P.C[0][0]);
	carre2=SDL_LoadBMP(&P.C[1][0]);
	carre3=SDL_LoadBMP(&P.C[2][0]);
	carre4=SDL_LoadBMP(&P.C[3][0]);
	carre5=SDL_LoadBMP(&P.C[4][0]);
	carre6=SDL_LoadBMP(&P.C[5][0]);
	carre7=SDL_LoadBMP(&P.C[6][0]);
	carre8=SDL_LoadBMP(&P.C[7][0]);
	carre9=SDL_LoadBMP(&P.C[8][0]);
	carre10=SDL_LoadBMP(&P.C[9][0]);
	carre11=SDL_LoadBMP(&P.C[10][0]);
	carre12=SDL_LoadBMP(&P.C[11][0]);
	carre13=SDL_LoadBMP(&P.C[12][0]);
	carre14=SDL_LoadBMP(&P.C[13][0]);
	carre15=SDL_LoadBMP(&P.C[14][0]);
	carre16=SDL_LoadBMP(&P.C[15][0]);

	//Separer points ex: 136 => 1/3/6, ensuite faire concatenation avec le chemin de son image pour afficher au SDL
	
	//ps: Il n'y a pas de bibliotheque SDL_ttf dans les machine virtuels vdiportail
	int points[6]={0,0,0,0,0,0};
	int valeur = P.S;
	int digit;
	i = 0;
	while(valeur > 0) {
		digit = valeur % 10;
		points[i] = digit;
		valeur = valeur / 10;
		i++;
	}
	for(i=0;i<6;i++){
		sprintf(temp, "img/s%d.bmp", points[i]);
		strcpy(&P.C[i][0],temp);
	}
	score1=SDL_LoadBMP(&P.C[0][0]);
	score2=SDL_LoadBMP(&P.C[1][0]);
	score3=SDL_LoadBMP(&P.C[2][0]);
	score4=SDL_LoadBMP(&P.C[3][0]);
	score5=SDL_LoadBMP(&P.C[4][0]);
	score6=SDL_LoadBMP(&P.C[5][0]);
	return P;
}

int ** tableauTemp(){
	int i, j;
	//creer tableau 4x4 temporaire pour les inversions
	int ** T = malloc(sizeof(int*)*4);
	if(T == NULL){exit(-1);}
	for(i=0;i<4;i++){
		T[i] = malloc(sizeof(int)*4);
		if(T[i]==NULL){exit(-1);}
	}
	return T;
}

//fonction pour liberer la memoire utilise par des tableaux
void freeTableau(int ** T){
	if(T!=NULL){
		int i;
		for(i=0;i<4;i++){
			if(T[i]!=NULL){
				free(T[i]);
			}
		}free(T);
	}
}

//Fonction pour inverser valeurs du tableau
int ** inverseT(int ** T){ 
	int i, j;
	int ** TT = tableauTemp();
	for(i=0;i<4;i++){
		for(j=3;j>=0;j--){
			TT[i][j] = T[i][3-j];
		}
	}
	freeTableau(T);
	T = TT;
	return TT;
}

//Fonction pour tourner tableau en 90 degre
int ** rotationT(int ** T){
	int i, j;
	int ** TT = tableauTemp();
	for(i=0;i<4;i++){
		for(j=3;j>=0;j--){
			TT[i][j] = T[3-j][i];
		}
	}
	freeTableau(T);
	T = TT;
	return TT;
}

//rotation inverse pour tourner tableau en -90 degre
int ** derotationT(int ** T){
	int i, j;
	int ** TT = tableauTemp();
	for(i=0;i<4;i++){
		for(j=3;j>=0;j--){
			TT[j][i] = T[i][3-j];
		}
	}
	freeTableau(T);
	T = TT;
	return TT;
}

//Fonction pour savoir le nombre de espaces vides et stocker ses coordonnees
struct partie trouverVides(struct partie P){
	int i, j;
	P.v=0;
	for(i=0;i<4;i++){
		for(j=0;j<4;j++){
			if(P.T[i][j]==0){ //si case == 0 stocker coord dans tableau P.V
				P.V[P.v][0]=i;
				P.V[P.v][1]=j;
				P.v++;//compteur espaces vides P.v
			}
		}
	}
	return P;
}

//Fonction pour ajouter 2 ou 4 dans des espaces aleatoires
struct partie ajouterValeur(struct partie P, int mode){
	int z, valeur;
	if(mode==1){ //mode de initialisation add valeur 2
		valeur=2;
	}else if(mode==2){ //mode jeu add valeur aleatoire 2 ou 4
		z=rand() % 2;
		if(z==0){
			valeur=2;
		}else{
			valeur=4;
		}
	}
	
	P=trouverVides(P);
	srand(time(NULL));
	z = rand() % P.v; //valeur aleatoire entre 0 et P.v (P.v = numero de espaces vides)
	P.T[P.V[z][0]][P.V[z][1]]=valeur; //prends coordonnees en P.V pour add le nouveau valeur
	P.v = P.v - 1;
	return P;
}

//Fonction base pour les mouvement (la base des mouvements c'est vers la droite)
struct partie action(struct partie P, int dir){
	int i, j, k;
	int block = 0;
	
	P.B[dir]=0;//s'il n'a pas de mouvent dans cette direction avertir
	
	for(i=0;i<4;i++){
			block = 0; //block
			for(j=3;j>=0;j--){
				if(j!=0){
					if(P.T[i][j]!=0){ //si espace n'est pas vide
						block = 0;//liberer block
						if(P.T[i][j-1]==0){ //si espace a cote est vide
							for(k=j-1;k>=0;k--){ //boucle pour trouver prochain espace non vide 
								if(P.T[i][k]!=0){ 
									if(P.T[i][k]==P.T[i][j]){ //s'ils on la meme valeur, fusionner
										P.T[i][j]+=P.T[i][k]; 
										P.S+=P.T[i][j];//Faire somme SCORE
										P.T[i][k]=0;
										k=-1;
										block = 1;//block somme suplementaire
										P.B[dir]=1;//avertir qu'il a eu un mouvement
									}else{ //sinon sortir de la boucle
										break;
									}
								}
							}
						}else if(P.T[i][j-1]==P.T[i][j]){
							P.T[i][j]+=P.T[i][j-1];
							P.S+=P.T[i][j];//Faire somme SCORE
							P.T[i][j-1]=0;
							block = 1;//block somme suplementaire
							P.B[dir]=1;//avertir qu'il a eu un mouvement
						}
					}else if(P.T[i][j]==0){
						for(k=j-1;k>=0;k--){
							if(P.T[i][k]!=0){
								P.T[i][j]=P.T[i][k];
								P.T[i][k]=0;
								P.B[dir]=1;//avertir qu'il a eu un mouvement
								if(j!=3){
									if(block!=1){//verifier si valeur est deja resultat d'une somme
										if(P.T[i][j+1]==P.T[i][j]){
											P.T[i][j+1]+=P.T[i][j];
											P.S+=P.T[i][j+1];//Faire somme SCORE
											P.T[i][j]=0;
											j++; //revenir dans l'espace qu'est vide maintenant
											block = 1;//block somme suplementaire
											P.B[dir]=1;//avertir qu'il a eu un mouvement
										}
									}
								}
								k=-1;
							}
						}
					}
				}
			}
		}
		return P;
}

//Fonction pour declancher un mouvement
struct partie mouvement(struct partie P, char x){
	if(x=='d'){
		P = action(P,0);
	}else if(x=='g'){
		P.T = inverseT(P.T); //inverser tableau pour utiliser fonction base mouvement a gauche
		P = action(P,1);
		P.T = inverseT(P.T); //desinverser tableau
	}else if(x=='h'){
		P.T = rotationT(P.T);
		P = action(P,2);
		P.T = derotationT(P.T);
	}else if(x=='b'){
		P.T = derotationT(P.T);
		P = action(P,3);
		P.T = rotationT(P.T);
	}
	//voir s'il y a encore des mouvements disponibles
	P=trouverVides(P);
	if(P.v==0){
		if(P.v==0 && P.B[0]==0 && P.B[1]==0 && P.B[2]==0 && P.B[3]==0){
			printf("\n\nGAME OVER!\n\nPoints: %d\n",P.S);
			ecranGameover=SDL_LoadBMP("img/gameover1.bmp");
			return P;
		}else{
			return P;
		}
	}
	
	P=ajouterValeur(P,2); //ajouder valeur a un espace vide aleatoire
	return P;
}

//Fonction pour ouvrir sauvegarde
struct partie ouvrirPartie(struct partie P){
	int n=0, r, i, j;
	
	//ouvrir fichier .txt et lire les donnees de la partie sauvergade
	FILE * F = fopen("data.txt", "r");
	if(F==NULL){
		printf("Error: il n'y a pas de partie sauvegarde encore!");
		P.v=999999; //avertir qu'il n'y a pas de sauvegarde
		return P;
	}
	
	int intLu;
    
    //Prendre premier int = score/points de la partie
    fscanf(F, "%d", &intLu);
	P.S = intLu;

    //Lire fichier et stocker les donnees
    r=0;
    n=0;
	while((fscanf(F, "%d", &intLu)) == 1){
        P.T[n][r]=intLu;
        r++;
        if(r==4){r=0;n++;}
	}
	return P; //retourner tableau avec les donnees de la partie sauvergade
	
}

//fonction pour sauvegarder les donnees du tableau et le score dans un fichier 
void sauvegardePartie(struct partie P){
	int i, j;
	FILE * F = fopen("data.txt", "w"); //ouvrir ou creer fichier data.txt
	if(F == NULL) {    
		perror("Error sauvegarde partie.") ;
		exit(-1) ;
	}
	
	fprintf(F,"%d",P.S); //fprintf des points
	
	for(i=0;i<4;i++){
		for(j=0;j<4;j++){
			fprintf(F," %d",P.T[i][j]); //fprintf des valeurs du tableau
		}
	}
	fclose(F);
	printf("\nPartie sauvegardee!\n");
}
void fermerJeu(){
	printf("[\nAu revoir! Ate logo!\n");
	exit(-1);
}
//Fonction pour inicialiser la partie
struct partie inicialiser(struct partie P){
	printf("\n/////////////////////////////////////////////\nBem-vindo: 2048\n/////////////////////////////////////////////\nRafael Baptista Benaion et Liza Toumi - L2 Info - ProgAvance\n\n\n");
	
	int i, j;
	char x;
	
	//creer tableau 4x4 base pour la partie
	P.T = malloc(sizeof(int*)*4);
	if(P.T == NULL){exit(-1);}
	for(i=0;i<4;i++){
		P.T[i] = malloc(sizeof(int)*4);
		if(P.T[i]==NULL){exit(-1);}
	}
	//SDL tableau pour garder les chemins des images
	P.C = malloc(sizeof(char*)*16);
	if(P.C == NULL){exit(-1);}
	for(i=0;i<16;i++){
		P.C[i] = malloc(200);
		if(P.C[i]==NULL){exit(-1);}
	}
	//creer tableau 16x2 pour stocker les espaces vides
	P.V = malloc(sizeof(int*)*16);
	if(P.V == NULL){exit(-1);}
	for(i=0;i<16;i++){
		P.V[i] = malloc(sizeof(int)*2);
		if(P.V[i]==NULL){exit(-1);}
	}
	//creer tableau pour verifier s'il n'y a plus de mouvement == game over
	P.B = malloc(sizeof(int)*4);
	if(P.B == NULL){exit(-1);}
	P.B[0]=1;
	P.B[1]=1;
	P.B[2]=1;
	P.B[3]=1;

	//SDL///////////////////////////////////////////////////////////////////////////////////////////
	
  	fondecran=SDL_LoadBMP("img/fondMenu.bmp"); //definir image de fond

	// Initialisation SDL
    if(SDL_Init(SDL_INIT_VIDEO) !=0){
        fprintf(stderr,"\nUnable to initialize SDL: %s\n", 
        SDL_GetError());
        exit(EXIT_FAILURE);
    }
    if((ecran = SDL_SetVideoMode(WIDTH,HEIGHT,32, SDL_HWSURFACE))==NULL){ 
        fprintf(stderr,"Erreur VideoMode %s\n",SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // Légende de la fenêtre
	SDL_WM_SetIcon(SDL_LoadBMP("img/2.bmp"), NULL); 
    SDL_WM_SetCaption("Jeu 2048 - Rafael Benaion et Liza Toumi",NULL); 
    SDL_FillRect(ecran,NULL,SDL_MapRGB(ecran->format, 0,0,0));
    SDL_Flip(ecran);
    dessine(ecran); //appeler fonction dessine pour afficher
    SDL_Flip(ecran);

	int l=1;
	while(l==1){ //boucle pour revenir au menu en cas d'error dans ouvrirPartie(P);

		x = boucleMenu();
		if(x=='n'){ //nouvelle partie
			//inicialiser valeurs 0
			for(i=0;i<4;i++){
				for(j=0;j<4;j++){
					P.T[i][j] = 0;
				}
			}
			P=ajouterValeur(P, 1);
			P=ajouterValeur(P ,1);
			P.S = 0;
			return P;
		}else if(x=='c'){ //ouvrir ancienne partie
			P = ouvrirPartie(P);
			if(P.v!=999999){
				return P;
			}else if(P.v==999999){
				l=1;
				fondecran=SDL_LoadBMP("img/fondMenuErreur.bmp");
				dessine(ecran);
			}	
		}else if(x=='f'){ //sortir du jeu
			fermerJeu();
		}else{
			printf("\nOops! Error: 404\n"); //error de choix
			fermerJeu();
		}
	}
}

int main(int argc, char ** argv){

	int p=1;
	char x;
	
	//definir la position des images
	srand(time(NULL));
    position.x=61;
    position.y=107;
    positionEcran.x=0;
    positionEcran.y=0;
	positionScore.x=165;
	positionScore.y=0;

	//creer partie base
	struct partie P = inicialiser(P);
	P = afficherTableau(P);
	
    fondecran=SDL_LoadBMP("img/fond1.bmp"); //changer fond pour la partie
    dessine(ecran);

	bouclePartie(P);//appeller boucle pour le commands de la partie
    SDL_Quit();
        return EXIT_SUCCESS;
	
	//Boucle pour le terminal (avant SDL)
	//attends mouvement joueur 
	while(p==1){
		//options de commande
		printf("\n[s]sauvergade\n[f]fermer\n[g][h][b][d]:");
		scanf(" %c", &x);
		
		if(x=='g' || x=='h' || x=='b' || x=='d'){ //check choix lettre deplacement
			//declancher mouvement
			P=mouvement(P, x);
			afficherTableau(P);
		}else if(x=='f'){
			fermerJeu();
		}else if(x=='z'){ // ONLY FOR TEST ENLEVER APRES
			P.T=inverseT(P.T);
			afficherTableau(P);
			printf("invertido");
		}else if(x=='s'){
			sauvegardePartie(P);
			afficherTableau(P);
			continue;
		}else{
			printf("Oops! Mauvaise choix.\n");
			continue;
		}
	}
}

//Fonctions SDL:
void bouclePartie(struct partie P){ //boucle pour le controle des commandes de la partie
    int cont=1;
    SDL_Event event;
    while(cont){
        SDL_WaitEvent(&event);
        switch(event.type){
			case SDL_MOUSEBUTTONUP://choix par souris click
				//coordonees de chaques button
				if(220 < event.button.x && 
				event.button.x < 220 + 161 &&
				540 < event.button.y && 
				event.button.y < 540 + 43){
					sauvegardePartie(P);
					P=afficherTableau(P);
					fondecran=SDL_LoadBMP("img/sauvegarde1.bmp");
					dessine(ecran);
                    break;
				}
				else if(390 < event.button.x && 
				event.button.x < 390 + 55 &&
				540 < event.button.y && 
				event.button.y < 540 + 44){
					cont=0;
                    break;
				}
                break;
            case SDL_QUIT:
            cont = 0;
            case SDL_KEYDOWN://choix par clavier
            switch(event.key.keysym.sym){
                case SDLK_UP:
					//appeller fonction de mouvement
					P=mouvement(P, 'h');
					//appeller fonction de affichage
					P=afficherTableau(P);
					//afficher changement SDL
                    dessine(ecran);
                    break;
                case SDLK_DOWN:
					P=mouvement(P, 'b');
					P=afficherTableau(P);
                    dessine(ecran);
                    break;
                case SDLK_LEFT :
					P=mouvement(P, 'g');
					P=afficherTableau(P);
                    dessine(ecran);
                    break;
                case SDLK_RIGHT:
                    P=mouvement(P, 'd');
					P=afficherTableau(P);
                    dessine(ecran);
                    break;
                case 's':
                    sauvegardePartie(P);
					P=afficherTableau(P);
					fondecran=SDL_LoadBMP("img/sauvegarde1.bmp"); //changer l'image de sauvegarde
					dessine(ecran);
                    break;
				case 'f':
                    cont=0;
                    break;
			}
        }
    }
}

//Boucle de controle pour le menu principal
char boucleMenu(){
	char x;
    int cont=1;
    SDL_Event event;
    while(cont){
        SDL_WaitEvent(&event);
        switch(event.type){
			case SDL_MOUSEBUTTONUP: //choix par souris click
				//coordonees de chaques button
				if(65 < event.button.x && 
					event.button.x < 65 + 161 &&
					542 < event.button.y && 
					event.button.y < 542 + 43){
					x='n';
                    return x;
                    break;
				}
				if(285 < event.button.x && 
					event.button.x < 285 + 161 &&
					542 < event.button.y && 
					event.button.y < 542 + 43){
					x='c';
                    return x;
					break;
				}
                break;
            case SDL_QUIT:
            cont = 0;
            case SDL_KEYDOWN: //choix par clavier
            switch(event.key.keysym.sym){
                case 'n':
					x='n';
                    return x;
                    break;
				case 'c':
					x='c';
                    return x;
                    break;
            }
        }
    }
}

void dessine(SDL_Surface * s){
    SDL_FillRect(ecran,NULL,SDL_MapRGB(ecran->format, 0,0,0));
	//Afficher image fond ecran
    SDL_BlitSurface(fondecran,NULL,s,&positionEcran);
    
	//Afficher images (valeurs) de chaque carre et definir leurs positions
	SDL_BlitSurface(carre1,NULL,s,&position);
	positionTemp.x=position.x+98;
	positionTemp.y=position.y;
    SDL_BlitSurface(carre2,NULL,s,&positionTemp);
	positionTemp.x=position.x+(2*98);
	SDL_BlitSurface(carre3,NULL,s,&positionTemp);
	positionTemp.x=position.x+(3*98);
    SDL_BlitSurface(carre4,NULL,s,&positionTemp);

	positionTemp.x=position.x;
	positionTemp.y=position.y+98;
	SDL_BlitSurface(carre5,NULL,s,&positionTemp);
	positionTemp.x=position.x+98;
    SDL_BlitSurface(carre6,NULL,s,&positionTemp);
	positionTemp.x=position.x+(2*98);
	SDL_BlitSurface(carre7,NULL,s,&positionTemp);
	positionTemp.x=position.x+(3*98);
    SDL_BlitSurface(carre8,NULL,s,&positionTemp);

	positionTemp.x=position.x;
	positionTemp.y=position.y+(2*98);
	SDL_BlitSurface(carre9,NULL,s,&positionTemp);
	positionTemp.x=position.x+98;
    SDL_BlitSurface(carre10,NULL,s,&positionTemp);
	positionTemp.x=position.x+(2*98);
	SDL_BlitSurface(carre11,NULL,s,&positionTemp);
	positionTemp.x=position.x+(3*98);
    SDL_BlitSurface(carre12,NULL,s,&positionTemp);


	positionTemp.x=position.x;
	positionTemp.y=position.y+(3*98);
	SDL_BlitSurface(carre13,NULL,s,&positionTemp);
	positionTemp.x=position.x+98;
    SDL_BlitSurface(carre14,NULL,s,&positionTemp);
	positionTemp.x=position.x+(2*98);
	SDL_BlitSurface(carre15,NULL,s,&positionTemp);
	positionTemp.x=position.x+(3*98);
	SDL_BlitSurface(carre16,NULL,s,&positionTemp);

	//Afficher les images des points (score) et leurs positions:
	SDL_BlitSurface(score6,NULL,s,&positionScore);
	positionTemp.y=positionScore.y;
	positionTemp.x=positionScore.x+30;
	SDL_BlitSurface(score5,NULL,s,&positionTemp);
	positionTemp.x=positionScore.x+(2*30);
	SDL_BlitSurface(score4,NULL,s,&positionTemp);
	positionTemp.x=positionScore.x+(3*30);
	SDL_BlitSurface(score3,NULL,s,&positionTemp);
	positionTemp.x=positionScore.x+(4*30);
	SDL_BlitSurface(score2,NULL,s,&positionTemp);
	positionTemp.x=positionScore.x+(5*30);
	SDL_BlitSurface(score1,NULL,s,&positionTemp);

	positionGameover.x=0;
	positionGameover.y=100;
	SDL_BlitSurface(ecranGameover,NULL,s,&positionGameover);

    SDL_Flip(s);
}