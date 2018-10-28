//
//  toyoda.cpp
//  HO_Imp_TD
//
//  Created by hanqing_zhao on 2018/4/2.
//  Copyright © 2018年 hanqing_zhao. All rights reserved.
//

#include <stdio.h>
#include <math.h>
#include <limits.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "toyoda.h"
#include <iostream>

using namespace std;
float **normalize_contraints(int **originalContraints,int *capacities, int m, int n){
    int i,j;
    float **newConstraints = (float **) malloc(m * sizeof(float *));
    for (i = 0 ; i < m ; i++) {
        newConstraints[i] = (float *) malloc(n * sizeof(float));
        for(j=0; j<n; j++){
            newConstraints[i][j]=(float)originalContraints[i][j]/(float)capacities[i];
        }
        
    }
    return newConstraints;
}
