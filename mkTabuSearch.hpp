//
//  mkTabuSearch.hpp
//  HO_proj_part2
//
//  Created by hanqing_zhao on 2018/5/6.
//  Copyright © 2018年 hanqing_zhao. All rights reserved.
//

#ifndef mkTabuSearch_hpp
#define mkTabuSearch_hpp

#include <stdio.h>
#include <math.h>
#include <limits.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "mkpdata.h"
#include "mkpneighbourhood.h"
#include "util.h"
#include "mkpproblem.h"


typedef struct _tabuList{
    int tabuLength;
    int *tabuFrequency;
    int *tabuLongtermFrequency;
    int **selectionHistory;
    
} tabuList;

void initTabuVector(problem*p,tabuList *tabuList1,int tabuLength);
void create_tabu_solution(solution *finalSolution,problem *p,int tabuLgh,double timeLimit);
void surrogateConstraints(solution *targetSolution,problem *p,double *surrogateCtrRes);
#endif /* mkTabuSearch_hpp */
