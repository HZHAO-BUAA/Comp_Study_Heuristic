/***************************************************************************
 *   Copyright (C) 2018 Alberto Franzin                                    *
 *   alberto.franzin@ulb.ac.be                                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "mkpio.h"

FILE *open_file(char *filename) {
  FILE *input_file;
  printf("\nOpening instance file %s\n\n",filename);
  if( (input_file = fopen(filename, "r")) == NULL) {
     fprintf(stderr, "error opening input file %s\n",filename);
     exit(1);
   }

  return (input_file);
}

void close_file(FILE *input_file) {
  fclose(input_file);
}

int *read_problem_data(FILE *input_file){
  int *problem_data = (int *) malloc(3 * sizeof(int));
  fscanf(input_file, "%d", &problem_data[0]);
  fscanf(input_file, "%d", &problem_data[1]);
  fscanf(input_file, "%d", &problem_data[2]);
  //printf("%d %d %d\n", problem_data[0], problem_data[1], problem_data[2]);
  return(problem_data);
}

int *read_profits(FILE *input_file, int n) {

  int i;
  int *profits = (int *) malloc(n * sizeof(int));
  
  for (i = 0 ; i < n ; i++) {
    fscanf(input_file, "%d", &profits[i]);
  }

  /*for (i = 0 ; i < n ; i++) {
    printf("%d ", profits[i]);
  }
  printf("\n\n");*/

  return(profits);

}


int **read_constraints(FILE *input_file, int m, int n) {

  int i,j;
  int **constraints = (int **) malloc(m * sizeof(int *));
  for (i = 0 ; i < m ; i++) {
    constraints[i] = (int *) malloc(n * sizeof(int));
  }

  for (i = 0 ; i < m ; i++) {
    for (j = 0 ; j < n ; j++) {
      fscanf(input_file, "%d", &constraints[i][j]);
    }
  }

  /*for (i = 0 ; i < m ; i++) {
    for (j = 0 ; j < n ; j++) {
      printf("%d ", constraints[i][j]);
    }
    printf("\n");
  }
  printf("\n");*/

  return(constraints);

}

int *read_capacities(FILE *input_file, int m) {

  int i;
  int *capacities = (int *) malloc(m * sizeof(int));

  for (i = 0 ; i < m ; i++) {
    fscanf(input_file, "%d", &capacities[i]);
  }

  return(capacities);

}


