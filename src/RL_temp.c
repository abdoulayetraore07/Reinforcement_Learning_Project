#include "mazeEnv.h"
#include "functions.h"
#include <math.h>

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

double reward (int x1, int y1, int x2, int y2 ) {   
     int reward_max=10000;
     double norme=sqrt( (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) ) 
     double reward = (1/norme+1)*10000 + reward_max*0.25 ;   
}

envOutput mazeEnv_step(action a){
    int reward = 0;
    int done = 0;
    envOutput stepOut;

    if (a==up){
       state_row = max(0,state_row -1);
    }else if (a==down){
       state_row = min(rows,state_row +1);
    }else if (a==right){
       state_col = min(cols,state_col +1);
    }else if (a==left){
       state_col = max(0,state_col -1);
    }
    
    if((state_row == goal_row) && (state_col == goal_col)){
       done   = 1;
    }

    if 

    stepOut.reward = reward;
    stepOut.done   = done;
    stepOut.new_col = state_col;
    stepOut.new_row = state_row; 

   return stepOut;
}

int main( int argc, char* argv[] ) {
     
     mazeEnv_make("maze.txt");        /* Creation du labyrinthe */
     nblignes_Q = rows*cols ;

     Q = malloc(nblignes_Q * sizeof(double));        /* Creation du tableau Q */
     for(int i=0; i<nblignes_Q; i++) {
         Q[i] = malloc(nb_actions * sizeof(double));
     }
     
     for (int i=0; i<nblignes_Q; ++i ) {   /* Initialisation du tableau Q */
          for ( int j=0; j<nb_actions; ++j ) {
              int temp = rand() % 2*nblignes_Q ;
              Q[i][j]= (double)temp - nblignes_Q ;
          }     
     }    
     
     for (int j=0; j<nb_actions; ++j )  {    /* Initialisation du tableau Q dans le cas terminal */
          Q[start_row][j]=0;  
     }

     init_visited()      /* Initialisation du tableau visited */


      /* Partie training */
     
     mazeEnv_reset();               /*Initialiser la cellule courante avec la cellule de depart

     while (state_row != goal_row || state_col = goal_col)  {
     
     double Q_max= Q[state_row][0];
     enum action current_act = up ;
     
     for ( int j=1; j<nb_actions; ++j ) {    /* Choix de l'action telle que Q est maximum  */
          if (Q_max<Q[state_row][j]) { 
               Q_max= Q[state_row][j];
               current_act = (enum action) (j) ; 
          }        
     }

     envOutput stepOut=mazeEnv_step(current_act);
     
     


     }
     
          

     

     























     
}
    
