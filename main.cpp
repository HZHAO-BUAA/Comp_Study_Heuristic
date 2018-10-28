//
//  main.cpp
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
#include <dirent.h>
#include <fstream>
#include<iostream>
#include<stack>
#include<sys/time.h>  

#include "mkpdata.h"
#include "util.h"
#include "mkpio.h"
#include "mkpsolution.h"
#include "mkpproblem.h"
#include "mkpneighbourhood.h"
#include "ameliorateSolution.hpp"
#include "mkTabuSearch.hpp"

using namespace std;

int main(int argc, char * argv[]) {
    // insert code here...

    DIR *pDir;
    struct dirent    *ent;
    char argvCopy[200];
    clock_t start,stop;
    time_t startT,stopT;
    int count=0;
    
    //argv1 instance dir
    pDir=opendir(argv[1]);
    //argv2 result dir
    string resultFilename=argv[2];
    resultFilename+="resultat_";
    
    //arg3 opti method CETS / ACO
    cout<<"SLS method: "<<argv[3]<<endl;
    //arg4 time limit proportion
    cout<<"Time Limit: "<<argv[4]<<"*Maxtime"<<endl;
    int seed=1234;
    int argCons=atoi(argv[3]); //optimization method, 1 = tabu, 2 = ACO
    double timeLimitR=stod(argv[4]); //time limit proportion, 1 =  maxtime, 0.1=10% of maxtime...
    int itrLimit=atoi(argv[5]); //number of repetition
    cout<<"Number of repetition: "<<itrLimit<<endl;
    
    resultFilename+=argv[3];
    resultFilename+="_";
    resultFilename+=argv[4];
    resultFilename+=".txt";

    ofstream resultatFile(resultFilename);
    //init set of pointer to ants outside loop
    int num_ant=50,idx;
    

    
    //init timer
    start= clock();
    startT= time(NULL);
    //init random seed
    set_seed(seed);
    while((ent=readdir(pDir))!=NULL){
        
        if(ent->d_type & DT_DIR){
            continue;
        }
        else{
            strcpy(argvCopy,argv[1]);
            problem *p = read_problem(strcat(argvCopy,ent->d_name));
            solution *finalSolution=create_empty_solution(p);
            solution *commonSa=create_empty_solution(p);
            solution *commonSTemp=create_empty_solution(p);
            solution *commonFTemp=create_empty_solution(p);

            //print_problem(p);
            resultatFile<<ent->d_name<<" "; //write instance file name into result file
            //maximum runtime
            double timeMax=timeLimitR*(((double)p->m*(double)p->n)/10.0);
            
            cout<<"time limit: "<<timeMax<<endl;
            //solution construction

            if(argCons==1){
                int idx;
                
                for(idx=0;idx<itrLimit;idx++){
                    emptyExistingSolution(p,finalSolution);
                    cout<<"repeat test: "<<idx<<endl;
                    create_tabu_solution(finalSolution, p, 1,timeMax);
                    cout<<"Tabu value: "<<finalSolution->value<<endl;
                    resultatFile<<finalSolution->value<<" ";
                }
                resultatFile<<endl;
                
            }
            else{
                ant *antSet[num_ant];
                for(idx=0;idx<num_ant;idx++){
                    //init ant
                    antSet[idx]=(ant*)malloc(sizeof(ant));
                    antSet[idx]->antSolution=create_empty_solution(p);
                    antSet[idx]->heuristicInfo=(double*)calloc(p->n,sizeof(double));
                    antSet[idx]->selectProbability=(double*)calloc(p->n,sizeof(double));
                    //init heuristic information for empty solution
                    calculate_HI(antSet[idx]->heuristicInfo, antSet[idx]->antSolution, p);
                }
                int idx,idy;

                for(idx=0;idx<itrLimit;idx++){
                    for(idy=0;idy<num_ant;idy++)emptyExistingSolution(p, antSet[idy]->antSolution);
                    emptyExistingSolution(p, commonSTemp);
                    emptyExistingSolution(p, commonSa);
                    emptyExistingSolution(p, commonFTemp);
                    cout<<"repeat test: "<<idx<<endl;
                    create_ant_solution(50, finalSolution, p, timeMax,antSet,commonSa,commonSTemp,commonFTemp);
                    cout<<"ACO value: "<<finalSolution->value<<endl;
                    resultatFile<<finalSolution->value<<" ";
                }
                resultatFile<<endl;
                


            }
            //destroy temp solutions
            free(commonSa);
            free(commonSTemp);
            free(commonFTemp);
            free(finalSolution);
            count++;
        }
        
    }
    stop= clock()-start;
    stopT=time(NULL)-startT;
    resultatFile<<stop<<" "<<stopT<<endl;
    resultatFile.close();

    return 0;
}
