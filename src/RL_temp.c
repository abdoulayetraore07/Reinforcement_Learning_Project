#include "mazeEnv.h"
#include "functions.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>

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
int** Q;
int nb_iterations_max = 100 ;


void alloc_mazeEnv(){
     mazeEnv = malloc(rows * sizeof(char*));

     for(int i=0; i<rows; i++) {
         mazeEnv[i] = malloc(cols * sizeof(char*));
     }
}

void mazeEnv_make(char* file_name){
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


void mazeEnv_reset(){             
     state_row = start_row;
     state_col = start_col;
     }

void alloc_visited()
{
        visited = malloc(rows * sizeof(int*));
        int i;
        for (i = 0; i < rows; ++i){
                visited[i] = malloc(cols * sizeof(int*));
        }
}

void init_visited()
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


double rewarder (int x1, int y1, int x2, int y2 ) {   
     int reward_max=10000;
     double norme=sqrt( (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) ) ;
     double rewards = (1/norme+1)*10000 + reward_max*0.25 ;   
     return rewards;
}

int min (int a, int b) {
     if (a>b) {
          return b;
     } else {
          return a;
     }
}

int max (int a, int b) {
     if (a>b) {
          return a;
     } else {
          return b;
     }
}

envOutput mazeEnv_step(action a){
    int rewards = 0;
    int done = 0;
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
        rewards = -10000; 
        state_row_new=state_row;
        state_col_new=state_col;  
   } else {
        rewards = rewarder (state_row,state_col,start_row,start_col) ;
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

struct policy {
     double Q_max;
     enum action current_act;
};


struct policy choice_policy(int state_row,int state_col) {  /* Permettant de retourner Q_max et action correspondant */
     double Q_max= Q[state_row*cols + state_col][0];
     enum action current_act = up ;
     for ( int j=1; j<nb_actions; ++j ) {    
          if (Q_max < Q[state_row][j]) { 
               Q_max= Q[state_row][j];
               current_act = (enum action) (j) ; 
          }        
     }   
     struct policy retour= {Q_max,current_act} ;
     return retour;
}


void mazeEnv_render_pos(){
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

     mazeEnv_make("maze.txt");        /* Creation du labyrinthe */
     nblignes_Q = rows*cols ;

     Q = malloc(nblignes_Q * sizeof(double));        /* Creation du tableau Q */
     for(int i=0; i<nblignes_Q; i++) {
         Q[i] = malloc(nb_actions * sizeof(double));
     }
     
     for (int i=0; i<nblignes_Q; ++i ) {   /* Initialisation du tableau Q */
          for ( int j=0; j<nb_actions; ++j ) {
              int temp = rand() % (2*nblignes_Q) ;
              Q[i][j]= temp-nblignes_Q ;
          }     
     }    
  
     int start_row_Q= start_row*cols + start_col ;  /*  Numero cellule du start dans le labyrinthe */
     int goal_row_Q= goal_row*cols + goal_col ;  /*  Numero cellule du goal dans le labyrinthe */
    
     for (int j=0; j<nb_actions; ++j )  {    /* Initialisation du tableau Q dans le cas terminal */
          Q[goal_row_Q][j]=0;  
     }

     init_visited() ;    /* Initialisation du tableau visited */
          
      /*** Partie training ***/


     mazeEnv_reset();               /*Initialiser la cellule courante avec la cellule de depart */

     int nb_iterations=0; 

     while (( state_row != goal_row || state_col != goal_col) && nb_iterations<nb_iterations_max ) {

          mazeEnv_render_pos()  ;   /*Affichage de l'etat actuel */

          struct policy state = choice_policy(state_row,state_col) ; /* Choix de l'action telle que Q est maximum  */
          envOutput stepOut=mazeEnv_step(state.current_act) ;   /* Observation rewards and new_state */
          
          double rewards = stepOut.reward ;
          int state_row_new = stepOut.new_row  ;          
          int state_col_new = stepOut.new_col ;

          struct policy state_new = choice_policy(state_row_new,state_col_new);  /* Choix de l'action telle que Q est maximum pour le nouvel état  */
          double Q_max=state.Q_max ;
          double Q_max_new=state_new.Q_max ;
         
          Q[state_row*cols + state_col][state.current_act] +=  alpha*( rewards + gamma_perso*Q_max_new - Q_max ) ;
          state_row= state_row_new ;
          state_col= state_col_new ;
          ++nb_iterations ;
       
     }

     return 0;
     
     
}
    
