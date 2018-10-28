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

#ifndef __MKPSOLUTION_H__
#define __MKPSOLUTION_H__

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

/*
 * create an empty solution for a MKP problem
 *
 * no items selected
 */
// Define a structure of ACO
typedef struct _ant{
    solution *antSolution;
    double *heuristicInfo;
    double *selectProbability;
} ant;

solution *create_empty_solution(problem *p);

solution *create_greedy_solution(problem *p);
/*
 * duplicate a solution
 */
solution *clone_solution(solution *s, problem *p);
solution *clone_solution(solution *s, solution *ns, problem *p);
/*
 * eliminate a solution
 */
void destroy_solution(solution *s);
void generateAntSolution(float *pheromone, ant* ant, problem *p,solution* commonSa, solution* commonSTemp,solution* commonFTemp);
/*
 * print a solution
 */
void print_solution(solution *s);

/*
 * return the objective function value of a given solution
 */
int compute_objective_function(solution *s, problem *p);

/*
 * check if a solution violates any constraint and update
 * the internal variables of the solution
 */
void check_violated_constraints(solution *s, problem *p);
void create_ant_solution(int num_ant, solution *finalSolution, problem *p,double timeLimit,ant** antSet,solution *commonSa,solution *commonSTemp,solution *commonFTemp);
void calculate_HI(double *heuristicInfo, solution *currentSolution, problem*p);

/*
 * 0 if no, 1 if yes
 */

bool emptyExistingSolution(problem *p,solution *s);

int is_feasible_solution(solution *s);

/*
 * create a random solution (CH1 in the first implementation exercise)
 * by shuffling the n items and trying to add in such order,
 * maintaining the feasibility of the solution
 */
solution *create_random_solution(problem *p);
void create_random_solution(problem *p,solution *s);
solution *create_toyoda_solution(problem *p);
#endif

