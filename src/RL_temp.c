#include <stdio.h>
#include "mazeEnv.h"
#include "functions.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>
// 
char** mazeEnv;
int** visited;
int rows;
int cols;
int start_row;
int start_col;
int state_row;
int state_col;
int goal_row;
int goal_col;
int nb_actions = 4 ;
int nblignes_Q ;
double alpha=0.1;
double gamma_perso=0.1;
double** Q;
double epsilon=0.2;
char police ;
int done = 0 ;


void alloc_mazeEnv(){                                           /* Creation du labyrinthe */
     mazeEnv = malloc(rows * sizeof(char*));  

     for(int i=0; i<rows; i++) {
         mazeEnv[i] = malloc(cols * sizeof(char*));
     }
}

void mazeEnv_make(char* file_name){                             /* Fonction de creation du labyrinthe à partir du nom du fichier */
     char c;
     char rows_s[3] ={'\0'}; 
     char cols_s[3] ={'\0'};
     int rows_i = 0;
     int cols_i = 0;
     int swap = 0;

     FILE* file = fopen(file_name, "r");
     if (file == NULL){
        printf("Pointeur nul");
     }

     if (file) {
         /* lire la premiere ligne avant EOF */
         while( (c=getc(file)) != EOF) {
               if(c== '\n'){
                      break;
               } else if (c==',') {
                      swap = 1;
               } else if (!swap) {
                      rows_s[rows_i]=c;
                      rows_i++;
               } else {
                      cols_s[cols_i]= c;
                      cols_i++;
               }
         }
     }

     /* convertir le string en nombre */
     rows = atoi(rows_s);
     cols = atoi(cols_s);

     alloc_mazeEnv();

     for (int i=0; i<rows; i++){
         for (int j=0; j < cols; j++){
             c = getc(file);

             if (c=='\n'){
                 c = getc(file);
             } else if (c == 's'){
               start_row = i;
               start_col = j;
             } else if ( c == 'g'){
               goal_row = i;
               goal_col = j;
             }

             mazeEnv[i][j] = c;
         }
     }

     fclose(file);
}

void mazeEnv_reset(){                                           /* Fonction d'initialisation de la cellule courante avec la cellule de depart */
     state_row = start_row;
     state_col = start_col;
     }

void alloc_visited()                                            /* Initialisation des différents terrains rencontrés pendant l'exploration */
{
        visited = malloc(rows * sizeof(int*));
        int i;
        for (i = 0; i < rows; ++i){
                visited[i] = malloc(cols * sizeof(int*));
        }
}

void init_visited()                                             /* Fonction d'initialisation des différents terrains rencontrés pendant l'exploration */
{
        alloc_visited();

        int i, j;
        for (i = 0; i < rows; ++i) {
                for (j = 0; j < cols; ++j) {
                        if (mazeEnv[i][j] == '+') {
                                visited[i][j] = wall;
                        } else if (mazeEnv[i][j] == 'g') {
                                visited[i][j] = goal;
                        } else {
                                visited[i][j] = unknown;
                        }
                }
        }
}

double rewarder (int x1, int y1, int x2, int y2 ) {             /* Fonction de récompense en fonction de la distance eucludienne */
     int reward_max=0.1;
     double norme=sqrt( (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) ) ;
     double rewards = (1/norme+1 + 0.25 )*reward_max ;   
     return rewards;
}

int min (int a, int b) {                                        /* Fonction min */
     if (a>b) {
          return b;
     } else {
          return a;
     }
}

int max (int a, int b) {                                        /* Fonction max */
     if (a>b) {
          return a;
     } else {
          return b;
     }
}

envOutput mazeEnv_step(action a){                               /* Fonction d'attribution de la récompense et du nouvel état */
    int rewards = 0;
    envOutput stepOut;
    int state_row_new=state_row;
    int state_col_new=state_col;
     
    if (a==up){
       state_row_new= max(0,state_row -1);
    }else if (a==down){
       state_row_new = min(rows,state_row +1);
    }else if (a==right){
       state_col_new= min(cols,state_col +1);
    }else if (a==left){
       state_col_new = max(0,state_col -1);
    }

   if ( visited[state_row_new][state_col_new]==wall ) {
        rewards = -1000000; 
        state_row_new=state_row;
        state_col_new=state_col; 
        printf("Mur\n\n");
   } else {
        rewards = rewarder (state_row_new,state_col_new,goal_row,goal_col) ;
        printf("Non Mur\n\n");
   }
     
    if((state_row == goal_row) && (state_col == goal_col)){
       done   = 1;
    }

    stepOut.reward = rewards ;
    stepOut.done   = done;
    stepOut.new_col = state_col_new;
    stepOut.new_row = state_row_new; 

   return stepOut;
}

struct policy {                                                 /* Struct pour retourner la valeur de Q_max et et l'action choisie grace aux polices */
     double Q_max;
     enum action current_act;
};

struct policy choice_policy_eps(int state_row,int state_col) {  /* Fonction choice_policy par epsilon_greedy */

     double Q_max= Q[state_row*cols + state_col][0];
     enum action current_act = up ;
     for ( int j=1; j<nb_actions; j++ ) {    
          if (Q_max < Q[state_row*cols + state_col][j]) { 
               Q_max= Q[state_row*cols + state_col][j];
               current_act = (enum action) (j) ; 
          }        
     }   
     int alea_1=rand() % 100 ;
     int borne= 100*epsilon ;
     if (alea_1<borne) {
          int alea_2= rand()% 4 ;
          current_act= (enum action) (alea_2) ;
          Q_max= Q[state_row*cols + state_col][current_act]; 
     }
     
     struct policy retour= {Q_max,current_act} ;
     return retour;

}

struct policy choice_policy_bolt(int state_row,int state_col) { /* Fonction choice_policy par exploration de boltzman */
     
     double somme_expo = 0;
     for (int j=0; j< nb_actions ; j++ ) {
          somme_expo+= exp(Q[state_row*cols + state_col][j]);
     }

     double proba_up = exp(Q[state_row*cols + state_col][0]) / somme_expo ;       /* Probabilité de choisir action up */
     double proba_down = exp(Q[state_row*cols + state_col][1]) / somme_expo ;     /* Probabilité de choisir action down */
     double proba_left = exp(Q[state_row*cols + state_col][2]) / somme_expo ;     /* Probabilité de choisir action left */
     double proba_right = exp(Q[state_row*cols + state_col][3]) / somme_expo ;    /* Probabilité de choisir action right */  

     int alea=rand() % 100 ;                                                      /* Choix d'un nombre aléatoire entre 0 et 99 */
     enum action current_act = 0 ;                     
     double Q_act = 0;
     

     if ( alea < proba_up*100 ) {                                                 /* Choix de l'action et Q correspondant en fonction de la probabilité */
          current_act = up ;
          Q_act= Q[state_row*cols + state_col][current_act]; 
     } else {
          if ( alea < ( proba_up + proba_down ) *100 ) { 
               current_act = down ;
               Q_act= Q[state_row*cols + state_col][current_act]; 
          } else {
               if ( alea < ( proba_up + proba_down + proba_left ) *100 ) {
                    current_act = left ;
                    Q_act= Q[state_row*cols + state_col][current_act] ; 

               } else {
                    current_act = right ;
                    Q_act= Q[state_row*cols + state_col][current_act] ;    
               }
          }
     }
               
     struct policy retour= {Q_act,current_act} ;
     return retour;

}

void mazeEnv_render_pos(){                                      /* Fonction d'affichage du labyrinthe, de l'état actuel et de la trajectoire */
     mazeEnv[state_row][state_col] = 'o';
     for (int i=0; i<rows; i++) {
         for (int j=0; j< cols; j++){
             printf("%c ", mazeEnv[i][j]);
         }
         printf("\n");
     }
     printf("\n");
}


int main( int argc, char* argv[] ) {
     
     srand( time( NULL ) );                                     

     mazeEnv_make("maze.txt");                                  /* Creation du labyrinthe */
     nblignes_Q = rows*cols ;
     int limit_Q= 2 ;                                          /* Limite des valeurs de Q */

     Q = malloc(nblignes_Q * sizeof(double));                   /* Creation du tableau Q */
     for(int i=0; i<nblignes_Q; i++) {
         Q[i] = malloc(nb_actions * sizeof(double));
     }
     
     for (int i=0; i<nblignes_Q; i++ ) {                        /* Initialisation du tableau Q */
          for ( int j=0; j<nb_actions; j++ ) {                           
              int temp = rand() % (2*limit_Q) ;                   /* Nombre aléatoire pour remplir Q entre -limit_Q et limit_Q */
              Q[i][j]= temp-limit_Q ;
          }     
     }    
  
     int start_row_Q= start_row*cols + start_col ;              /*  Numero cellule du start dans le labyrinthe */
     int goal_row_Q= goal_row*cols + goal_col ;                 /*  Numero cellule du goal dans le labyrinthe */
    
     for (int j=0; j<nb_actions; j++ )  {                       /* Initialisation du tableau Q dans le cas terminal */
          Q[goal_row_Q][j]=0;  
     }

     

     init_visited() ;                                           /* Initialisation du tableau visited */



      /*** Partie training ***/


     mazeEnv_reset();                                           /* Initialiser la cellule courante avec la cellule de depart */

     int reponse = 0;   
     do {      
          printf("Tapez le chiffre 1 pour la police epsilon_greedy ou 2 pour l'exploration de Boltzman\n ") ;
          scanf("%d",&reponse) ;
     } while ( reponse != 1 && reponse != 2) ;

     clock_t start, end; 
     double elapsed; 
  
     start = clock();                                            /* Lancement de la mesure pour connaître le délai d'éxecution de la boucle */
     printf("Le temps de départ est : %.2f\n ", start ) ;

     if (reponse == 1 ) {

          while ( done != 1 ) {
               printf("-----------------------------------------------\n\n\n");  
               mazeEnv_render_pos()  ;                               /* Affichage de l'etat actuel */  

               struct policy state = choice_policy_eps(state_row,state_col) ; /* Choix de l'action en fonction de la police et de Q pour l'état courant */
               envOutput stepOut=mazeEnv_step(state.current_act) ;   /* Observation rewards and new_state */
          
               double rewards = stepOut.reward ;                     /* Récuperation de la récompense */
               int state_row_new = stepOut.new_row  ;                /* Récuperation du nouvel_état */
               int state_col_new = stepOut.new_col ;
               double Q_max=state.Q_max ;                            /* Récuperation de la valeur de Q pour l'action choisie précedemment pour l'état courant */

               struct policy state_new = choice_policy_eps(state_row_new,state_col_new);  /* Choix de l'action en fonction de la police et de Q pour le nouvel état  */
          
               double Q_max_new=state_new.Q_max ;                    /* Récuperation de la valeur de Q pour l'action choisie précedemment pour le nouvel état */
         
               Q[state_row*cols + state_col][state.current_act] +=  alpha*( rewards + gamma_perso*Q_max_new - Q_max ) ;  /* Modification de la valeur de Q */
               state_row= state_row_new ;
               state_col= state_col_new ; 
     }

     }
     else {

          while ( done != 1 ) {
               printf("-----------------------------------------------\n\n\n");  
               mazeEnv_render_pos()  ;                               /* Affichage de l'etat actuel */  

               struct policy state = choice_policy_bolt(state_row,state_col) ; /* Choix de l'action en fonction de la police et de Q pour l'état courant */
               envOutput stepOut=mazeEnv_step(state.current_act) ;   /* Observation rewards and new_state */
          
               double rewards = stepOut.reward ;                     /* Récuperation de la récompense */
               int state_row_new = stepOut.new_row  ;                /* Récuperation du nouvel_état */
               int state_col_new = stepOut.new_col ;
               double Q_max=state.Q_max ;                            /* Récuperation de la valeur de Q pour l'action choisie précedemment pour l'état courant */

               struct policy state_new = choice_policy_bolt(state_row_new,state_col_new);  /* Choix de l'action en fonction de la police et de Q pour le nouvel état  */
          
               double Q_max_new=state_new.Q_max ;                    /* Récuperation de la valeur de Q pour l'action choisie précedemment pour le nouvel état */
         
               Q[state_row*cols + state_col][state.current_act] +=  alpha*( rewards + gamma_perso*Q_max_new - Q_max ) ;  /* Modification de la valeur de Q */
               state_row= state_row_new ;
               state_col= state_col_new ;  
     }

     }

     end = clock();                                    /* Arrêt de la mesure     */ 
     elapsed = ((double)end - start) / CLOCKS_PER_SEC; /* Conversion en secondes  */  

     printf("Bravo, vous avez atteint la sortie en : %.2f secondes. \n\n", elapsed ) ;
     
     
     return 0 ;
     
}
