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

#include "util.h"
#include <iostream>
using namespace std;
void set_seed(int seed) {
  srand(seed);
}



int *create_shuffled(int n) {
  int i,
      *v = (int *)malloc(n * sizeof(int));
  for (i = 0 ; i < n ; i++) v[i] = i;
  
  shuffle_int(v, n);
  
  return(v);
}

void shuffle_int(int *v, int n) {
  int i,  j, tmp;
  for (i = n-1 ; i >= 1 ; i--) {
    j = rand() % i;
    tmp = v[i];
    v[i] = v[j];
    v[j] = tmp;
  }
}

params *read_params(int argc, char *argv[]) {
  int i;

  // check in mkpdata.h what fields there are

  params *pars = (params *)malloc(sizeof(params));

  pars->instance_file = argv[1];
  pars->seed = 0;
  for (i = 2 ; i < argc ; i++) {
    if (strcmp(argv[i], "--seed") == 0) {
      i++;
      pars->seed = atoi(argv[i]);
    }
  }
  
  return(pars);
}

long factorial(long number)
{   if(number<=1)
    return 1;
else
    return number*factorial(number-1);
}

double lnchoose(int n, int m)
{
    
    if (m > n)
        
    {
        
        return 0;
        
    }
    if (m < n/2.0)
    {
        m = n-m;
    }
    
    double s1 = 0;
    for (int i=m+1; i<=n; i++)
    {
        s1 += log((double)i);
    }
    
    double s2 = 0;
    int ub = n-m;
    for (int i=2; i<=ub; i++)
    {
        s2 += log((double)i);
    }
    
    return s1-s2;
}

int combinator(int n, int m)
{
    
    if (m > n)
        
    {
        
        return 0;
        
    }
    return (int)exp(lnchoose(n, m));
}
