#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <limits.h>

int nb_actions = 9 ;     // Neuf choix maximum possibles en toute généralité
int nblignes_Q = 19683 ; // Le nombre total de combinaisons possibles pour un plateau de 3x3 avec trois états possibles pour chaque case est donné par 19683 car chaque case peut être dans l'un des trois états indépendamment des autres cases.
double alpha=0.1;
double gamma_perso = 0.95;
double** Q1;
double** Q2;
double epsilon = 1.0;       // Epsilon depart
double epsilon_final = 0.1; // Epsilon final
int nb_max_moves = 100000 ;
int nb_training = 5000000 ;
int nb_normalise = 75 ;
int* grille ;
int state ;
int new_state ;


/* Creer une grille vide */
int* creer_grille() {
   int* grille = malloc(sizeof(int)*9);
   return grille;
}

void init_grille() {
    for (int i=0; i<9; ++i) {
       grille[i] = 0;
   }
}

/* Afficher la grille. */
void afficher() {
    printf("\n");
    printf("-------------\n");
    printf("|");
    for (int i=0; i<9; ++i) {
            if (grille[i] == 0) {
                printf(" . |");
            } else if (grille[i] == 1) {
                printf(" X |");
            } else{
                printf(" O |");
            }
            if ((i+1)%3==0 && i!=8 ) {
                printf("\n");
                printf("|");
            }
    }
    printf("\n-------------\n");
}

/* Placer un symbole pour le joueur à la case
   donnée par le chiffre. */
void placer(int chiffre, int joueur) {
    grille[chiffre-1] = joueur;
}

/* Placer un symbole dans une case aléatoire
   pour joueur donné, en veillant de ne pas
   placer sur une case occupée. */
void placer_alea(int joueur) {
   int libre = 0;
   int i;
   do {
       // choisir i entre 1 et 9
        i = rand()%(9)+1;
       if (grille[i-1]==0) {
           placer(i,joueur);
           libre = 1;
       }
   } while (!libre);
}

/* Donne 1 si le joueur a gagné, et 0 sinon. */
int a_gagne(int joueur) {
    int gagne = 0;
    int test;
    // On teste toutes les colonnes,
    // puis toutes les lignes,
    // puis les deux diagonales.
    // C'est gagné si une ligne/colonne/diagonale
    // n'a que des cases occupées par le joueur.
    
   // verticale



   for (int i=0;i<3;++i) {
          test = 1;
       // tester la i+1-ième colonne
       for (int j=0;j<3;++j) {
           if (grille[i+3*j] != joueur) {
              test = 0;
           }
       }
       gagne = gagne || test;
   }
   // horizontale
   for (int i=0;i<3;++i) {
          test = 1;
       // tester la i+1-ième ligne
       for (int j=0;j<3;++j) {
           if (grille[3*i+j] != joueur) {
              test = 0;
           }
       }
       gagne = gagne || test;
   }
   // diagonale gauche
   test = 1;
   for (int i=0;i<3;++i) {
           if (grille[3*i+i] != joueur) {
              test = 0;
           }
       }
       gagne = gagne || test;
   // diagonale droite
   test = 1;
   for (int i=0;i<3;++i) {
           if (grille[3*(2-i)+i] != joueur) {
              test = 0;
           }
       }
       gagne = gagne || test;
   return gagne;
}

/* Donne 1 si la grille est pleine, et 0 sinon. */
int est_plein() {
    // On cherche une case vide.
    // Si trouvé, la grille n'est pas pleine.
    for (int i=0;i<9;++i) {
        if (grille[i]==0) {
            return 0; // case vide, grille n'est pas pleine
        }
    }
    return 1; // aucune case vide, grille est pleine
}

int is_busy( int chiffre ) {        // Fonction permettant de savoir si la case choisie est vide ou pas.
    if (grille[chiffre-1]==0) {
            return 0; // case vide 
    }
    return 1 ; 
}

int choice_policy_eps_global( int etat ) {  /* Fonction choice_policy par epsilon_greedy avec Q1 + Q2  */

    /* Fonction retournant la valeur maximale de Q dans l'état s et l'action correspondant */
    int action ;
    int alea_1=rand() % 100 ;
    int borne= 100*epsilon ;                                   /* Multiplication par 100 pour se ramener entre 0 et 100 pour faciliter le tirage aléatoire de nombre */

    if (alea_1<borne) {     
        int alea_2 ;                                   /* Choix d'une action aléatoire avec probabilite epsilon */
        do {
             alea_2= rand()% 9 + 1 ;     
        } while (is_busy(alea_2)!=0) ;  
        action=alea_2 ;   
    } else {
        int Q_max= INT_MIN ;
        for (int i = 0; i < 9; i++) {
            if ( is_busy(i+1)==0  && (Q1[etat][i] + Q2[etat][i]) > Q_max ) {
                Q_max = Q1[etat][i] + Q2[etat][i];
                action = i+1;
            }
        }
    }
    return action ;
    
    return 0;

}

int choice_policy_eps_single( int etat, double** Q) {  /* Fonction choice_policy par epsilon_greedy avec Q1 ou Q2  */

    /* Fonction retournant la valeur maximale de Q dans l'état s et l'action correspondant */
    int action ;
    int alea_1=rand() % 100 ;
    int borne= 100*epsilon ;                                   /* Multiplication par 100 pour se ramener entre 0 et 100 pour faciliter le tirage aléatoire de nombre */

    if (alea_1<borne) {     
        int alea_2 ;                                   /* Choix d'une action aléatoire avec probabilite epsilon */
        do {
             alea_2= rand()% 9 + 1 ;     
        } while (is_busy(alea_2)!=0) ;  
        action=alea_2 ;   
    } else {
        int Q_max= INT_MIN ;
        for (int i = 0; i < 9; i++) {
            if ( is_busy(i+1)==0  && Q[etat][i] > Q_max ) {
                Q_max = Q[etat][i] ;
                action = i+1;
            }
        }
    }
    return action ;
    
    return 0;

}

int convert_grille_etat( ) {              // La base 3 est utilisée parce que chaque case du plateau peut avoir trois états distincts (vide, X, O)
    int etat  = 0;
    for (int i = 0; i < 9; i++) {
            etat = etat*3 + grille[i] ;
    }
    return etat;
}

double maxi_Q ( int new_state, double** Q ) {     /* Fonction retournant la veleur de maximale de Q dans l'état s et l'action correspondant */
                                       
    double Q_max= INT_MIN ;
    int libre = 0 ;
    for (int i = 0; i < 9; i++) {
            if ( is_busy(i+1)==0 && Q[new_state][i] > Q_max) {
                Q_max = Q[new_state][i] ;
                libre = 1 ;
            }
    }
    if (libre == 0) {                           // Si la grille est pleine dans l'état new_state retourner une valeur de Q aléatoire ;
        int i = rand()%9 ;
        Q_max= Q[new_state][i] ;
    }
    return Q_max ;
}

void sauvegarder_Q(char* filename, double** Q) {
    FILE* fichier = fopen(filename, "w");
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < nblignes_Q; i++) {
        for (int j = 0; j < nb_actions; j++) {
            fprintf(fichier, "%lf ", Q[i][j]);
        }
        fprintf(fichier, "\n");
    }
    fclose(fichier);
}

void charger_Q(char* filename, double** Q) {
    FILE* fichier = fopen(filename, "r");
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < nblignes_Q; i++) {
        for (int j = 0; j < nb_actions; j++) {
            if (fscanf(fichier, "%lf", &Q[i][j]) != 1) {
                perror("Erreur lors de la lecture du fichier");
                exit(EXIT_FAILURE);
            }
        }
    }
    fclose(fichier);
}

void init_Q(double** Q_table) {
    for (int i = 0; i < nblignes_Q; i++) {
        for (int j = 0; j < nb_actions; j++) {
            Q_table[i][j] = 0.0;
        }
    }
}

void mise_a_jour_Q(int action, int reward ) {
    /*Mise à jour de Q1 ou Q2 avec une probabilité 0.5*/
                int action_future = 0 ;
                int alea_1= rand() % 100 +1 ;

                if (alea_1<=50) {
                    int Q_max = maxi_Q(new_state,Q1) ;
                    for (int j=0; j<9 ; j++) {
                        if (Q1[new_state][j]==Q_max) {
                            action_future = j+1;
                            break ;
                        }
                    }
                    Q1[state][action-1] += alpha * ( reward + gamma_perso * Q2[new_state][action_future-1] - Q1[state][action-1]);
                } else {
                    int Q_max = maxi_Q(new_state,Q2) ;
                    for (int j=0; j<9 ; j++) {
                        if (Q2[new_state][j]==Q_max) {
                            action_future = j+1;
                            break ;
                        }
                    }
                    Q2[state][action-1] += alpha * ( reward + gamma_perso * Q1[new_state][action_future-1] - Q2[state][action-1]);
                }

}
/* Programme principal */

int main() {
        
    // On crée une grille vide
    grille = creer_grille();

    Q1 = malloc(nblignes_Q * sizeof(double*));                    /* Creation du tableau Q */
    Q2 = malloc(nblignes_Q * sizeof(double*));
    for (int i = 0; i < nblignes_Q; i++) {
        Q1[i] = malloc(nb_actions * sizeof(double));
        Q2[i] = malloc(nb_actions * sizeof(double));
    }
    
    charger_Q("data/Q1_values.txt", Q1);
    charger_Q("data/Q2_values.txt", Q2);                                 /* Initialisation des tableaux  Q1 et Q2 */


    /* PARTIE TRAINING */
    

    printf("\n\nDebut training\n" ); 

    for ( int j=1; j<=nb_training ; j++ ) {                    // Boucle pour faire les épisodes  

        int action ;
        init_grille() ;
        state = convert_grille_etat() ;
        int finie = 0;                                             // Variable booléenne représentant si la partie est finie ou pas
        int reward = 0 ;

        /// Réduction de epsilon sur les épisodes
        epsilon = epsilon - (1.0 - epsilon_final) / nb_training;
        if (epsilon < epsilon_final) {
            epsilon = epsilon_final;
        } 

        while (!finie) {
            srand( time( NULL ) );
            action =  choice_policy_eps_global( state ) ;
            placer( action, 1 );                                        // Joueur = 1 represente RL et Joueur = 2 represente choix_aléatoire ou humain ;

            if (a_gagne(1)) {
                reward = 10000 ;
                new_state = convert_grille_etat() ;
                finie = 1 ; 
                mise_a_jour_Q(action, reward ) ;
            } else {
                if (!est_plein()) {
                    placer_alea(2);
                    if (a_gagne(2)) {
                        reward = -10000 ;
                        finie = 1 ;
                    }
                    else {
                        reward= -1 ;
                    }
                } else {
                    finie = 1 ;
                    reward = 0 ;
                }
                new_state = convert_grille_etat() ;
                mise_a_jour_Q(action, reward ) ;
            }
            state = new_state ;
           
            // c'est fini si un des deux joueurs a gagné,
            // ou la grille est pleine
        }
        // afficher la grille à la fin du jeu,
        // et afficher qui a gagné ou si match nul (case remplie)
        // printf("Épisode %d/%d terminée \n",j, nb_training );  /////////////////////////////////////////////
        
    }

    sauvegarder_Q("data/Q1_values.txt", Q1);
    sauvegarder_Q("data/Q2_values.txt", Q2);   // On sauvegarde les valeurs de Q vu que un bon entrainement demande beaucoup de temps compare au cas du labyrinthe ;

    /* PARTIE POUR PERMETTRE A UN HUMAIN DE JOUER APRES LE TRAINING */

    int nb_parties_jouées = 0 ;      // Variables pour définir le nombre de parties qu'un humain pourra faire apres le training ;
    int continu_jeu = 1 ;           
    int partie_win = 0 ;

    
    printf("\n\n----------Training terminée----------\n" ) ; 
    printf("\nTapez 0 pour rejouer ou un autre chiffre sinon : " ) ; 
    scanf("%d",&continu_jeu);
    
    

    while ( continu_jeu == 0  )   {
        
        ++nb_parties_jouées;

        int action ;
        int action_humain ;
        int reward = 0 ;
        init_grille();
        state = convert_grille_etat() ;
        int finie = 0;                                             // Variable booléenne représentant si la partie est finie ou pas
   
        int tours = 0;                                             // Nombre de tours joués

        while (!finie) {
        
            tours = tours + 1;
            action =  choice_policy_eps_global( state ) ;
            placer( action, 1 );                                        // Joueur = 1 represente IA et Joueur = 2 represente choix_aléatoire ou humain ;
            afficher();                                                 // afficher la grille
            if (a_gagne(1)) {
                reward = 10000000 ;
                new_state = convert_grille_etat() ;
                finie = 1 ; 
                mise_a_jour_Q(action, reward ) ;
            } else {
                if (!est_plein()) {                                     // Cas grille non pleine ; 
                    int nb_essai=0;
                    do {                                                // Choix de l'action de l'humain sur une case non_occupée ;
                        ++nb_essai ;
                        if (nb_essai>1) {
                            printf("Alors comme ça, on veut tricher... Je te surveille petit_bonhomme\n\n");
                        }
                        printf("Entrez un chiffre pour placer un pion dans la case : ");
                        scanf("%d",&action_humain);
                    } while (is_busy(action_humain)!=0 || action_humain<0 || action_humain> 9 ) ;

                    // placer le symbole du joueur humain
                    placer(action_humain,2);
                    afficher();                                                 // afficher la grille

                    if (a_gagne(2)) {
                        finie = 1 ;
                        reward = -10000000 ;
                    } else {
                        reward=-10000 ;
                    }

                } else {                                                 // Cas grille pleine ;
                    finie = 1 ;
                    reward = 0 ;
                }
                new_state = convert_grille_etat() ;
            }
            state = new_state ;
            mise_a_jour_Q(action, reward ) ;
           
            // c'est fini si un des deux joueurs a gagné,
            // ou la grille est pleine
        }
        
        afficher();
        if (a_gagne(1)) {
            printf("\nOups, l'humanité est menacée là !!!  IA a gagné en %d tours.\n",tours);
        }
        else if (a_gagne(2)) {
            printf("\nBravo Superman( Woman ), vous faites l'honneur des Hommes, vous avez gagné en %d tours.\n",tours);
            ++ partie_win ;

        } else {
            printf("\nPas si loin, Match nul en %d tours.\n",tours);
        }
        
        sauvegarder_Q("data/Q1_values.txt", Q1);   // On sauvegarde les valeurs de Q vu que un bon entrainement demande beaucoup de temps compare au cas du labyrinthe ;
        sauvegarder_Q("data/Q2_values.txt", Q2);

        printf("\nParties jouées : %d\nParties gagnées : %d \n\n",nb_parties_jouées, partie_win) ; // Partie statistique et rejouer ; 
        printf("Tapez 0 pour rejouer ou un autre chiffre sinon\n" ) ; 
        scanf("%d",&continu_jeu);

    }

    printf("\nJeu terminé\n\n" ) ; 
    
    
    for (int i = 0; i < nblignes_Q; i++) {
        free(Q1[i]);
        free(Q2[i]);
    }
    free(Q1);
    free(Q2);
    free(grille); 
                                 


    }

    return 0 ;

}
