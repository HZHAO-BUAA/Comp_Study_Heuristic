//
//  ameliorateSolution.hpp
//  HO_Imp_TD
//
//  Created by hanqing_zhao on 2018/4/6.
//  Copyright © 2018年 hanqing_zhao. All rights reserved.
//

#ifndef ameliorateSolution_hpp
#define ameliorateSolution_hpp
#include <stack>
#include "mkpdata.h"
#include "mkpsolution.h"
#include "util.h"
#include "mkpproblem.h"
using namespace std;

void amelirate_solution_FI(problem *p, solution *s, solution *sFinale, int mode);

void ameliorate_recursive(problem *p, solution *s,solution *sa, solution *sFinale,solution *sTemp, int mode, int facon,int k);
void amelirate_solution_BI(problem *p, solution *s,solution *sFinale, int mode);
void ameliorateSolutionVND(int k,solution *s,problem *p,solution *sa, solution *sFinale, int mode);
void combine(int *arr,int start,int *result, stack<int> &resultStack,int index,int n,int arr_len);

float *calculatePUValue(problem *p, solution *s);
#endif /* ameliorateSolution_hpp */
