//
//  mkTabuSearch.cpp
//  HO_proj_part2
//
//  Created by hanqing_zhao on 2018/5/6.
//  Copyright © 2018年 hanqing_zhao. All rights reserved.
//

#include "mkTabuSearch.hpp"
#include "mkpsolution.h"
#include<algorithm>
#include<iostream>
#include "toyoda.h"
#include <time.h>
#include "ameliorateSolution.hpp"

void initTabuVector(problem*p,tabuList *tabuList1,int tabuLength){
    tabuList1->tabuFrequency=(int*)calloc(p->n,sizeof(int));
    tabuList1->tabuLongtermFrequency=(int*)calloc(p->n,sizeof(int));
    tabuList1->tabuLength=tabuLength;
    int idx,idy;
    for(idx=0;idx<p->n;idx++)tabuList1->tabuFrequency[idx]=0;
    for(idx=0;idx<p->n;idx++)tabuList1->tabuLongtermFrequency[idx]=0;
    
    tabuList1->selectionHistory = (int **) malloc(tabuLength * sizeof(int *));
    for (idx = 0 ; idx < tabuLength ; idx++) {
        tabuList1->selectionHistory[idx] = (int *) malloc(p->n * sizeof(int));
        for(idy=0;idy<p->n;idy++){
            tabuList1->selectionHistory[idx][idy]=0;
        }
    }
}

//renew tabu records for each newly generated solution
void renewTabuVector(solution *newSolution,problem *p, tabuList *tabuList1){
    int idx,idy;
    for(idx=0;idx<newSolution->num_selected;idx++){
        tabuList1->tabuFrequency[newSolution->selected[idx]]+=1;
        tabuList1->tabuLongtermFrequency[newSolution->selected[idx]]+=1;
    }
    for(idx=0;idx<p->n;idx++){
        tabuList1->tabuFrequency[idx]-=tabuList1->selectionHistory[tabuList1->tabuLength-1][idx];
    }
    for(idx=0;idx<tabuList1->tabuLength-1;idx++){
        for(idy=0;idy<p->n;idy++){
            tabuList1->selectionHistory[tabuList1->tabuLength-1-idx][idy]=tabuList1->selectionHistory[tabuList1->tabuLength-2-idx][idy];
        }
        
    }
    for(idx=0;idx<p->n;idx++){
        tabuList1->selectionHistory[0][idx]=0;
    }
    for(idx=0;idx<newSolution->num_selected;idx++){
        tabuList1->selectionHistory[0][newSolution->selected[idx]]+=1;
    }
}

void calculatePenality(solution *targetSolution, tabuList *tabuList1, problem *p, double* penalityVector,int itr){
    double PenR=0,C=100000,PenF=0;
    int idx;
    double surrogateCtr[p->n];
    //calculate initial ratio save to penality vector
    surrogateConstraints(targetSolution, p, surrogateCtr);
    //find maximum ratio
    for(idx=0;idx<p->n;idx++){
        if(surrogateCtr[idx]>PenR)PenR=surrogateCtr[idx];
    }
    PenF=PenR/(C*(double)itr);
    for(idx=0;idx<p->n;idx++){
        //penalityVector[idx]=PenR*(double)tabuList1->tabuFrequency[idx];
        penalityVector[idx]=PenF*(double)tabuList1->tabuLongtermFrequency[idx];
    }
    
}

//generate selective priority of each item give a target solution
void surrogateConstraints(solution *targetSolution,problem *p,double *surrogateCtrRes){
    int idx,idy;
    double bPrime[p->m],wValues[p->m];
    //for each constraint
    for(idx=0;idx<p->m;idx++){
        //init each b' to initial b
        bPrime[idx]=p->capacities[idx];
        for(idy=0;idy<targetSolution->num_selected;idy++){
            bPrime[idx]=bPrime[idx]-(double)p->constraints[idx][targetSolution->selected[idy]];
        }
        if(bPrime[idx]>0){
            wValues[idx]=1.0/bPrime[idx];
        }
        else{
            wValues[idx]=2.0-bPrime[idx];
        }
    }
    for(idx=0;idx<p->n;idx++){
        surrogateCtrRes[idx]=0;
        for(idy=0;idy<p->m;idy++){
            surrogateCtrRes[idx]+=wValues[idy]*p->constraints[idy][idx];
        }
        surrogateCtrRes[idx]=p->profits[idx]/surrogateCtrRes[idx];
    }
}

int findMaxItem(int* selectedMark,double *priorityValue,solution *targetSolution){
    int idx,maxItem=-1;
    double maxValue=-10000000000;
    for(idx=0;idx<targetSolution->num_discarded;idx++){
        if(selectedMark[targetSolution->discarded[idx]]==0 & priorityValue[targetSolution->discarded[idx]]>maxValue){
            maxItem=targetSolution->discarded[idx];
            maxValue=priorityValue[targetSolution->discarded[idx]];
        }
    }
    //cout<<"max value: "<<maxValue<<endl;
    return maxItem;
}

int findMinItem(double *priorityValue,solution *targetSolution){
    int idx,minItem=0;
    double minValue=10000000000;
    for(idx=0;idx<targetSolution->num_selected;idx++){
        if(priorityValue[targetSolution->selected[idx]]<minValue){
            minItem=targetSolution->selected[idx];
            minValue=priorityValue[targetSolution->selected[idx]];
        }
    }
     //cout<<"min value: "<<minValue<<endl;
    return minItem;
    
}

void create_tabu_solution(solution *finalSolution,problem *p,int tabuLgh,double timeLimit){
    tabuList *mainTabuList=(tabuList*)malloc(sizeof(tabuList));
    //init a tabu vector
    initTabuVector(p, mainTabuList,tabuLgh);
    //construction process
    solution *tempSolution=create_empty_solution(p);
    int idx, itrCounter,ItemToAdd,viol;
    double priorityValue[p->n];
    double penalityVector[p->n];
    clock_t startC,stopC;
    int selectedMark[p->n];
    int commonTurn=0;
    itrCounter=0;
    startC= clock();
    stopC=clock()-startC;
    while(stopC<timeLimit*1000000){
        itrCounter+=1;
        //constructive phase
        //cout<<"tabu itr: "<<itrCounter<<endl;
        for(idx=0;idx<p->n;idx++)selectedMark[idx]=0;
        //cout<<"constructive "<<endl;
        //while no one is added
        //calculate priority value to be selected for each item
        surrogateConstraints(tempSolution, p, priorityValue);
        calculatePenality(tempSolution, mainTabuList, p, penalityVector, itrCounter+1);
        //cout<<"main tabu list freq : "<<mainTabuList->tabuFrequency[1]<<" "<<mainTabuList->tabuFrequency[90]<<endl;
        //cout<<"penality vec : "<<penalityVector[1]<<" "<<penalityVector[90]<<endl;
        if(commonTurn<=4){
            for(idx=0;idx<p->n;idx++)priorityValue[idx]=priorityValue[idx]-penalityVector[idx];
        }

        
        ItemToAdd=0;
        viol=1;
        commonTurn+=1;
        while(ItemToAdd!=-1){
            ItemToAdd=findMaxItem(selectedMark, priorityValue, tempSolution);
            
            if(ItemToAdd!=-1){
                viol=check_and_add_item(tempSolution, ItemToAdd, p);
                selectedMark[ItemToAdd]=1;
                if(viol==0){
                    //cout<<"item added: "<<ItemToAdd<<" solution value: "<<tempSolution->value<<endl;
                    surrogateConstraints(tempSolution, p, priorityValue);
                    if(commonTurn<=4){
                        for(idx=0;idx<p->n;idx++)priorityValue[idx]=priorityValue[idx]-penalityVector[idx];
                    }
                }
            }
            
        }

        if(tempSolution->value>finalSolution->value){
            clone_solution(tempSolution, finalSolution, p);
            cout<<"New global best solution, value: "<<finalSolution->value<<endl;
            renewTabuVector(tempSolution, p, mainTabuList);
            calculatePenality(tempSolution, mainTabuList, p, penalityVector, itrCounter+1);
            commonTurn=0;
        }
        
        //number to span = 1 to 6
        //cout<<"constructive span"<<endl;
        int numSpan=(rand() % 6) + 1;
        int countSpan=0;
        ItemToAdd=0;
        for(idx=0;idx<p->n;idx++)selectedMark[idx]=0;
        //add num_span additional items making solution infeasible
        
        while(countSpan<numSpan & ItemToAdd!=-1){
            ItemToAdd=findMaxItem(selectedMark, priorityValue, tempSolution);
            if(ItemToAdd!=-1){
                add_item(tempSolution, ItemToAdd, p);
                //cout<<"item added: "<<ItemToAdd<<" solution value: "<<tempSolution->value<<endl;
                surrogateConstraints(tempSolution, p, priorityValue);
                if(commonTurn<=4){
                    for(idx=0;idx<p->n;idx++)priorityValue[idx]=priorityValue[idx]-penalityVector[idx];
                }
                countSpan+=1;
            }
        }
        //cout<<"distructive"<<endl;
        //Distructive process
        check_violated_constraints(tempSolution, p);
        int removeItem=0;
        while(!is_feasible_solution(tempSolution)){
            removeItem=findMinItem(priorityValue, tempSolution);
            remove_item(tempSolution, removeItem, p);
            //cout<<"item removed: "<<removeItem<<" solution value: "<<tempSolution->value<<endl;
            check_violated_constraints(tempSolution, p);
            //cout<<"solution situation : "<<is_feasible_solution(tempSolution)<<endl;
        }
        if(tempSolution->value>finalSolution->value){
            clone_solution(tempSolution, finalSolution, p);
            cout<<"New global best solution, value: "<<finalSolution->value<<endl;
            renewTabuVector(tempSolution, p, mainTabuList);
            calculatePenality(tempSolution, mainTabuList, p, penalityVector, itrCounter+1);
            commonTurn=0;
        }
        //cout<<"distructive span"<<endl;
        //span distructive
        //number to span = 1 to 6
        numSpan=(rand() % 6) + 1;
        //cout<<"num span: "<<numSpan<<endl;
        countSpan=0;
        int itemToRemove=0;
        while(countSpan<numSpan & itemToRemove!=-1){
            itemToRemove=findMinItem(priorityValue, tempSolution);
            if(itemToRemove!=-1){
                
                remove_item(tempSolution, itemToRemove, p);
                //cout<<"item removed: "<<itemToRemove<<" solution value: "<<tempSolution->value<<endl;
                surrogateConstraints(tempSolution, p, priorityValue);
                if(commonTurn<=4){
                    for(idx=0;idx<p->n;idx++)priorityValue[idx]=priorityValue[idx]-penalityVector[idx];
                }
            }
            countSpan+=1;
        }
        //cout<<"best solution value: "<<finalSolution->value<<endl;
        stopC=clock()-startC;
    }
    

}
