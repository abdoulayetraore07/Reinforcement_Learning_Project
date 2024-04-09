#include "mazeEnv.h"
#include "functions.h"
#include "math.h"

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
int nombre_actions = 4 ;
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


int main( int argc, char* argv[] ) {
     
     mazeEnv_make(maze.txt);        /* Creation du labyrinthe */
     mazeEnv_reset();               /*Initialiser la cellule courante avec la cellule de depart
     nblignes_Q = rows*cols ;

     Q = malloc(nombre_actions * sizeof(double));        /* Creation du tableau Q */
     for(int i=0; i<nombre_actions; i++) {
         Q[i] = malloc(nblignes_Q * sizeof(double));
     }
     
     for (int i=0; i<nombre_actions; ++i ) {               /* Initialisation du tableau Q */
          for ( int j=0; j<nblignes_Q; ++j ) {
              double temp = random
              Q[i][j]= 
          }     
     }                                           
     
     
     
     























     
}
    
