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

#include "mkpsolution.h"
#include<algorithm>
#include<iostream>
#include "toyoda.h"
#include <math.h>
#include "ameliorateSolution.hpp"
using namespace std;

solution *create_empty_solution(problem *p) {
  int n = p->n;
  int m = p->m;

  solution *s = (solution *)malloc(sizeof(solution));

  // see mkpdata.h for what the fields contain
  s->n = n;
  s->m = m;
  s->sol = (int *)calloc(n, sizeof(int));
  s->value = 0;

  s->num_selected = 0;
  s->selected = (int *)calloc(n, sizeof(int));
  s->num_discarded = n;
  s->discarded = (int *)calloc(n, sizeof(int));
  for (int i = 0 ; i < n ; i++) s->discarded[i] = i;

  s->num_violated = 0;
  s->violated = (int *)calloc(m, sizeof(int));
  s->resources_used = (int *)calloc(m, sizeof(int));
    
  return (s);
}

bool emptyExistingSolution(problem *p,solution *s){
    int n = p->n;
    int m = p->m;
    /*s->n = n;
    s->m = m;
    s->value = 0;
    s->num_selected = 0;
    s->num_discarded = n;
    s->num_violated = 0;
    for (int i = 0 ; i < n ; i++) s->sol[i] = 0;
    for (int i = 0 ; i < n ; i++) s->discarded[i] = i;
    for (int i = 0 ; i < m ; i++) s->resources_used[i] = 0;
    for (int i = 0 ; i < m ; i++) s->violated[i] = 0;*/
    s->n = n;
    s->m = m;
    if (s->sol != NULL){
        for (int i = 0 ; i < n ; i++) s->sol[i] = 0;
    }
    
    s->value = 0;
    
    s->num_selected = 0;
    if (s->selected != NULL){
    for (int i = 0 ; i < n ; i++) s->selected[i] = 0;
    }
    s->num_discarded = n;
    if (s->discarded != NULL){
    for (int i = 0 ; i < n ; i++) s->discarded[i] = i;
    }
    
    s->num_violated = 0;
    if (s->violated != NULL){
    for (int i = 0 ; i < m ; i++) s->violated[i] = 0;
    }
    if (s->resources_used != NULL){
    for (int i = 0 ; i < m ; i++) s->resources_used[i] = 0;
    }
    return true;
}


solution *clone_solution(solution *s, problem *p) {
  solution *ns = create_empty_solution(p);
  for (int i = 0 ; i < s->n ; i++) {
    ns->sol[i] = s->sol[i];
    ns->selected[i] = s->selected[i];
    ns->discarded[i] = s->discarded[i];
  }
  ns->num_selected = s->num_selected;
  ns->num_discarded = s->num_discarded;
  ns->value = s->value;
  for (int i = 0 ; i < s->m ; i++) {
    ns->violated[i] = s->violated[i];
    ns->resources_used[i] = s->resources_used[i];
  }
  ns->num_violated = s->num_violated;
  return(ns);
}

solution *clone_solution(solution *s, solution *ns, problem *p) {
    //solution *ns = create_empty_solution(p);
    for (int i = 0 ; i < s->n ; i++) {
        ns->sol[i] = s->sol[i];
        ns->selected[i] = s->selected[i];
        ns->discarded[i] = s->discarded[i];
    }
    ns->num_selected = s->num_selected;
    ns->num_discarded = s->num_discarded;
    ns->value = s->value;
    for (int i = 0 ; i < s->m ; i++) {
        ns->violated[i] = s->violated[i];
        ns->resources_used[i] = s->resources_used[i];
    }
    ns->num_violated = s->num_violated;
    return(ns);
}

void destroy_solution(solution *s) {
  if (s->sol != NULL) free(s->sol);
  if (s->selected != NULL) free(s->selected);
  if (s->discarded != NULL) free(s->discarded);
  if (s->violated != NULL) free(s->violated);
  if (s->resources_used != NULL) free(s->resources_used);
  free(s);
}

void print_solution(solution *s) {
  if (s->num_selected == 0) printf("No items selected in the solution!\n");
  else {
    printf("Items in solution:\n[ ");
    for (int i = 0 ; i < s->n ; i++) {
      if (s->sol[i] == 1) printf("%d ",i);
    }
    printf("]\nSolution value: %d\n", s->value);
  }

  if (s->num_violated == 0) {
    printf("This is a feasible solution.\n");
  } else {
    printf("Violated constraints:\n");
    for (int i = 0 ; i < s->m ; i++) {
       if (s->violated[i] == 1) printf("%d ",i);
    }
    printf("\n");
  }

  printf("[ ");
  for (int i = 0 ; i < s->num_selected ; i++) {
      printf("%d ",s->selected[i]);
  }
  printf("] :: [ ");
  for (int i = 0 ; i < s->num_discarded ; i++) {
    printf("%d ",s->discarded[i]);
  }
  printf("]\n\n");
}


int compute_objective_function(solution *s, problem *p) {
  int i, c = 0;
  for (i = 0 ; i < s->n ; i++) {
    c = c + s->sol[i] * p->profits[i];
  }
  return(c);
}


void check_violated_constraints(solution *s, problem *p) {
 // check how many elements in s->resources_used exceed the maximum capacity
 // allowed by the constraints
 int i, nv = 0;
 for (i = 0 ; i < p->m ; i++) {
   if (s->resources_used[i] > p->capacities[i]) {
     nv++;
     s->violated[i] = 1;
   } else {
     s->violated[i] = 0;
   }
 }
 s->num_violated = nv;
}


int is_feasible_solution(solution *s) {
  return (s->num_violated == 0);
}


solution *create_random_solution(problem *p) {
  // create an array of n shuffled items
  // scan the array and try to add one by one
  // add only if the i-th item does not violate any constraint
  solution *s = create_empty_solution(p);
  int *v = create_shuffled(p->n);
  int i, viol;
  for (i = 0 ; i < p->n ; i++) {
    viol = check_and_add_item(s, v[i], p);
  }
  // viol = 0
  //free(v);

  return(s);
}
//creat dynamique heuristic information
void calculate_HI(double *heuristicInfo, solution *currentSolution, problem*p){
    int idx=0,idy=0,idz=0;
    double denominateur1,denominateur2;
    for(idx=0;idx<p->n;idx++){
        denominateur1=0;
        for(idy=0;idy<p->m;idy++){
            denominateur2=0;
            for(idz=0;idz<currentSolution->num_selected;idz++){
                denominateur2+=(double)p->constraints[idy][currentSolution->selected[idz]];
            }
            
            denominateur2=(((double)p->capacities[idy])-denominateur2)+1;
  
            denominateur1+=((double)p->constraints[idy][idx])/denominateur2;
        }
        heuristicInfo[idx]=((double)p->profits[idx])/denominateur1;
    }
}

void generateAntSolution(double *pheromone, ant* ant, problem *p,solution* commonSa, solution* commonSTemp,solution *commonFTemp){
    int idx=0,itemAdd;

    double alpha=1,beta=20,denominateur,sumSelect;
    //empty ant solution
    emptyExistingSolution(p, ant->antSolution);
    emptyExistingSolution(p, commonSa);
    emptyExistingSolution(p, commonFTemp);
    emptyExistingSolution(p, commonSTemp);
    /*int tabuItems[p->n];
    for(idx=0;idx<p->n;idx++){
        tabuItems[idx]=0;
    }*/
    //for all items
    bool noMoreAdd=false;
    //cout<<"add items: "<<endl;
    while(!noMoreAdd){
        double selectThreshold=rand()%100/(double)101;
        sumSelect=0;
        //initialize all selective proba to 0
        for(idx=0;idx<p->n;idx++)ant->selectProbability[idx]=0;
        //refresh heuristic info
        calculate_HI(ant->heuristicInfo, ant->antSolution, p);
        //calculate selective probability of each unselected item
        denominateur=0;
        noMoreAdd=true;
        int numFeasible=0;
        for(idx=0;idx<ant->antSolution->num_discarded;idx++){
            //if item does not violate constraint
            
            if(check_item(ant->antSolution, ant->antSolution->discarded[idx], p))
            {
                numFeasible+=1;
                noMoreAdd=false;
                ant->selectProbability[ant->antSolution->discarded[idx]]=pow(pheromone[ant->antSolution->discarded[idx]],alpha)*pow(ant->heuristicInfo[ant->antSolution->discarded[idx]],beta);

                denominateur+=ant->selectProbability[ant->antSolution->discarded[idx]];
                /* if(isnan(pheromone[ant->antSolution->discarded[idx]])==1){
                    cout<<"pheromone: "<< pheromone[ant->antSolution->discarded[idx]]<<endl;
                    cout<<"hi item:"<<ant->antSolution->discarded[idx]<<endl;
                }*/
            }
            
        }
        //cout<<"num frasible: "<< numFeasible <<endl;
        //cout<<"denominateur: "<< denominateur <<endl;
        //regularize selective proba
        for(idx=0;idx<p->n;idx++){
            if(denominateur!=0){
                ant->selectProbability[idx]=ant->selectProbability[idx]/denominateur;
            }
            
           
        }
        
        
        idx=0;

        
        while(sumSelect<=selectThreshold & idx<=p->n){
            sumSelect+=ant->selectProbability[idx];
            idx++;
        }
        idx=idx-1;
        
        itemAdd=idx;
        
        //cout<<"try to add item: "<<idx<<endl;
        check_and_add_item(ant->antSolution, itemAdd, p);
    }
    //BI greedy method amelioration
    //cout<<"recursive amelioration: "<<endl;
    ameliorate_recursive(p, ant->antSolution,commonSa,commonFTemp,commonSTemp, 2, 1, 0);
    emptyExistingSolution(p, ant->antSolution);
    clone_solution(commonFTemp, ant->antSolution, p);
}

double gFunction(solution *targetSolution,problem *p){
    double g=0;
    int idx;
    for(idx=0;idx<targetSolution->num_discarded;idx++){
        g+=p->profits[targetSolution->discarded[idx]];
    }
    g=1.0/g;
    return g;
}


void create_ant_solution(int num_ant, solution *finalSolution, problem *p,double timeLimit,ant** antSet,solution *commonSa,solution *commonSTemp,solution *commonFTemp){
    
    int idx=0,idy=0,localMaxAnt;
    
    //Pbest parameter
    double Pbest=0.5,rho=0.95,globalMaxValue=-1,localMaxValue,tauxMax,tauxMin,phAug;
    double epsilon=(1-pow(Pbest,1.0/(double)p->n))/((((double)p->n)/2-1)*pow(Pbest,1.0/(double)p->n));
    //pheromone trail for each object
    double pheromone[p->n];
    for(idx=0;idx<p->n;idx++)pheromone[idx]=0;

    //for each itration
        clock_t startC,stopC;
        startC= clock();
        stopC=clock()-startC;
        
    while(stopC<timeLimit*1000000){
        localMaxValue=0;
        localMaxAnt=0;
        //select ant with global max solution
        //cout<<"select global max"<<endl;
        for(idy=0;idy<num_ant;idy++){
            if(antSet[idy]->antSolution->value>localMaxValue){
                localMaxValue=antSet[idy]->antSolution->value;
                localMaxAnt=idy;
            }
        }
        //cout<<"local max value: "<<localMaxValue<<endl;
        //if local max is a global max
        if(localMaxValue>globalMaxValue){
            globalMaxValue=localMaxValue;
            cout<<"New global best solution, value: "<<globalMaxValue<<endl;
            clone_solution(antSet[localMaxAnt]->antSolution,finalSolution, p);
        }

        //renew pheromone trail
        //cout<<"renew pheromone trail"<<endl;
        tauxMax=gFunction(finalSolution, p)/(1-rho);
        tauxMin=epsilon*tauxMax;
        phAug=gFunction(finalSolution, p);
        for(idy=0;idy<antSet[localMaxAnt]->antSolution->num_selected;idy++){
            pheromone[antSet[localMaxAnt]->antSolution->selected[idy]]=rho*pheromone[antSet[localMaxAnt]->antSolution->selected[idy]]+phAug;
            /*if(isnan(pheromone[antSet[localMaxAnt]->antSolution->selected[idy]])==1){
                cout<<"pheromone: "<<pheromone[antSet[localMaxAnt]->antSolution->selected[idy]]<<endl;
                cout<<"phaug: "<<phAug<<endl;
            }*/
        }
        
        for(idy=0;idy<p->n;idy++){
            if(pheromone[idy]>tauxMax)pheromone[idy]=tauxMax;
            if(pheromone[idy]<tauxMin)pheromone[idy]=tauxMin;
            /*if(isnan(pheromone[idy])==1){
                cout<<"pheromone: "<<pheromone[idy]<<endl;
                cout<<"tauxmax: "<<tauxMax<<endl;
                cout<<"tauxmin: "<<tauxMin<<endl;
            }*/

        }

        //update solution of each ant
        for(idy=0;idy<num_ant;idy++){
            //cout<<"generate solution of ant: "<<idy<<endl;
            generateAntSolution(pheromone, antSet[idy], p,commonSa,commonSTemp,commonFTemp);
        }
        
        stopC=clock()-startC;
    }
    //when process therminated, select best solution
    //cout<<"select global max"<<endl;
    localMaxValue=0;
    localMaxAnt=0;
    for(idy=0;idy<num_ant;idy++){
        if(antSet[idy]->antSolution->value>localMaxValue){
            localMaxValue=antSet[idy]->antSolution->value;
            localMaxAnt=idy;
        }
    }
    //if local max is a global max
    if(localMaxValue>globalMaxValue){
        globalMaxValue=localMaxValue;
        cout<<"New global best solution, value: "<<globalMaxValue<<endl;
        clone_solution(antSet[localMaxAnt]->antSolution,finalSolution, p);
    }
}

void create_random_solution(problem *p,solution *s) {
    // create an array of n shuffled items
    // scan the array and try to add one by one
    // add only if the i-th item does not violate any constraint
    //solution *s = create_empty_solution(p);
    emptyExistingSolution(p, s);
    int *v = create_shuffled(p->n);
    int i, viol;
    for (i = 0 ; i < p->n ; i++) {
        viol = check_and_add_item(s, v[i], p);
    }
    // viol = 0
    free(v);
    
    //return(s);
}

solution *create_greedy_solution(problem *p){
    solution *s=create_empty_solution(p);
    // create a listed of selected items, set them to zero
    int i,j,voil, nextSelection=0;
    int *checkedChoix;
    bool findProfit;
    checkedChoix=(int*)malloc(p->n * sizeof(int));
    for(i=0;i<p->n;i++)checkedChoix[i]=0;
    //copy profit matrix into temprofit
    int *temProfit=(int *)malloc(p->n * sizeof(int));
    
    for (i=0;i<p->n;i++){
        temProfit[i]=p->profits[i];
    }
    std::sort(temProfit,temProfit+p->n);
    
    for(i=0; i<p->n; i++){
        findProfit=false;
        for(j=0; j<p->n; j++){
            if(p->profits[j]==temProfit[p->n-1-i] & checkedChoix[j]==0 & !findProfit){
                nextSelection=j;
                if(!findProfit){
                    checkedChoix[j]=1;
                    findProfit=true;
                }
            }
        }
        voil = check_and_add_item(s,nextSelection,p);
    }
    return(s);
}
float *calculatePUValue(problem *p, solution *s){
    float **normalizedContraints=normalize_contraints(p->constraints,p->capacities, p->m, p->n);
    int i,j;
    float *puValues, euclidU=0;
    if(s->num_selected==0){
        float vValues[p->n];
        //vValues=(float*)malloc(p->n * sizeof(float));
        puValues=(float*)malloc(p->n * sizeof(float));
        for(i=0;i<p->n;i++){
            vValues[i]=0;
            for(j=0;j<p->m;j++){
                vValues[i]+=normalizedContraints[j][i];
            }
            
            puValues[i]=(float)p->profits[i]/vValues[i];
        }
        return puValues;
    }
    else{
        float uValues[p->m],vValues[p->n];
        //uValues=(float*)malloc(p->m * sizeof(float));
        //vValues=(float*)malloc(p->n * sizeof(float));
        puValues=(float*)malloc(p->n * sizeof(float));
        for(i=0;i<p->m;i++)uValues[i]=0;
        
        for(i=0;i<s->num_selected;i++){
            for(j=0;j<p->m;j++){
                uValues[j]+=normalizedContraints[j][s->selected[i]];
            }
        }
        euclidU=0;
        for(j=0;j<p->m;j++) euclidU+=pow(uValues[j],2);
        euclidU=sqrt(euclidU);
        for(i=0;i<p->n;i++){
            vValues[i]=0;
            for(j=0;j<p->m;j++){
                vValues[i]+=normalizedContraints[j][i]*uValues[j]/euclidU;
            }
            puValues[i]=(float)p->profits[i]/vValues[i];
        }
        return puValues;
    }
}

solution *create_toyoda_solution(problem *p){
    solution *s=create_empty_solution(p);
    float **normalizedContraints=normalize_contraints(p->constraints,p->capacities, p->m, p->n);
    float *uValues, *vValues, *puValues, *puValues2, euclidU=0;
    int *checkedChoix;
    int i,j,countSolution, nextSelection=0, voil;
    bool emptySolution=true;
    uValues=(float*)malloc(p->m * sizeof(float));
    vValues=(float*)malloc(p->n * sizeof(float));
    puValues=(float*)malloc(p->n * sizeof(float));
    
    checkedChoix=(int*)malloc(p->n * sizeof(int));
    //initialize checkedChoix
    for(i=0;i<p->n;i++)checkedChoix[i]=0;
    //initialize uValues
    for(i=0;i<p->m;i++)uValues[i]=0;
    //initialize V values
    for(countSolution=0;countSolution<p->n; countSolution++){
        if(emptySolution){
            //calculate v and pu without using u
            for(i=0;i<p->n;i++){
                vValues[i]=0;
                for(j=0;j<p->m;j++){
                    vValues[i]+=normalizedContraints[j][i];
                }
                
                puValues[i]=(float)p->profits[i]/vValues[i];
                //cout<<puValues[i]<<" ";
            }
            //cout<<endl;
        }
        else{
            //calculate v and pu using u
            for(i=0;i<p->n;i++){
                vValues[i]=0;
                for(j=0;j<p->m;j++){
                    vValues[i]+=normalizedContraints[j][i]*uValues[j]/euclidU;
                }
                puValues[i]=(float)p->profits[i]/vValues[i];
                //cout<<puValues[i]<<" ";
            }
            //cout<<endl;
        }
        //determine next treatement
        float largestPU=0;
        for(i=0;i<p->n;i++){
            if(puValues[i]>largestPU & checkedChoix[i]==0){
                largestPU=puValues[i];
                nextSelection=i;
            }
        }
        /*
        puValues2=calculatePUValue(p, s);
        cout<<"pu values from end to end func: ";
        for(i=0;i<p->n;i++){
            cout<<puValues2[i]<<" ";
        }
        cout<<endl;
         */
        checkedChoix[nextSelection]=1;
        //cout<<"Try to add: "<<nextSelection<<endl;
        voil=check_and_add_item(s, nextSelection, p);
        //if item correctly added into solution
        if(voil==0){
            emptySolution=false;
            euclidU=0;
            for(i=0;i<p->m;i++){
                uValues[i]+=normalizedContraints[i][nextSelection];
                euclidU+=pow(uValues[i],2);
            }
            euclidU=sqrt(euclidU);
        }
        
    }

    return s;
}


