//
//  ameliorateSolution.cpp
//  HO_Imp_TD
//
//  Created by hanqing_zhao on 2018/4/6.
//  Copyright © 2018年 hanqing_zhao. All rights reserved.
//

#include "ameliorateSolution.hpp"
#include <stdio.h>
#include <math.h>
#include <limits.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "toyoda.h"
#include <iostream>
#include <algorithm>

#include "util.h"
#include "mkpio.h"
#include "mkpsolution.h"

using namespace std;
void generatedSuffledNotSelected(problem *p, solution *s,int *suffledNotSelected, int mode){
    int  i,j;
    int checkedChoix[p->n];
    //checkedChoix=(int*)malloc(s->num_discarded * sizeof(int));
    for(i=0;i<p->n;i++)checkedChoix[i]=0;
    
    if(mode==1){
        //if mode==1, discarded items are randomly added
        shuffle_int(suffledNotSelected, s->num_discarded);
        //uffledNotSelected = create_shuffled(s->num_discarded);
        
    }
    //mode=2 -> greedy
    else if(mode==2){
        bool findProfit;
        //store values of discarded items
        //suffledNotSelected= (int *)malloc(s->num_discarded * sizeof(int));
        int temProfit[s->num_discarded];
        for (i=0;i<s->num_discarded;i++){
            temProfit[i]=p->profits[s->discarded[i]];
        }
        std::sort(temProfit,temProfit+s->num_discarded);
        
        for(i=0;i<s->num_discarded;i++){
            findProfit=false;
            for(j=0;j<s->num_discarded;j++){
                if(p->profits[s->discarded[j]]==temProfit[s->num_discarded-1-i] & checkedChoix[j]==0 & !findProfit){
                    suffledNotSelected[i]=j; //indicators in s->discarded are suffled
                    //cout<<"Index j= "<<j<<" discarded value= "<<s->discarded[j]<<endl;
                    if(!findProfit){
                        checkedChoix[j]=1;
                        findProfit=true;
                    }
                }
            }
        }
    }
    //mode = 3 -> toyoda
    else if(mode==3){
        bool findPU;
        //suffledNotSelected= (int *)malloc(s->num_discarded * sizeof(int));
        float *puValues = calculatePUValue(p, s);
        float temPU[s->num_discarded];
        for (i=0;i<s->num_discarded;i++){
            temPU[i]=puValues[s->discarded[i]];
        }
        std::sort(temPU,temPU+s->num_discarded);
        for(i=0;i<s->num_discarded;i++){
            findPU=false;
            for(j=0;j<s->num_discarded;j++){
                if(puValues[s->discarded[j]]==temPU[s->num_discarded-1-i] & checkedChoix[j]==0 & !findPU){
                    suffledNotSelected[i]=j; //indicators in s->discarded are suffled
                    //cout<<"Index j= "<<j<<" discarded value= "<<s->discarded[j]<<endl;
                    if(!findPU){
                        checkedChoix[j]=1;
                        findPU=true;
                    }
                }
            }
        }
    }
    else{
        suffledNotSelected = create_shuffled(s->num_discarded);
    }
   // return suffledNotSelected;
}

void amelirate_solution_FI(problem *p, solution *s, solution *sFinale, int mode){
    //Store final solution
    //solution *sa=create_empty_solution(p);
    emptyExistingSolution(p, sFinale);
    int suffledSelected[s->num_selected];
    int suffledNotSelected[s->num_discarded];
    int i=0,j,viol, newProfit;
    
    for(i=0;i<s->num_selected;i++){
        suffledSelected[i]=i;
    }
    shuffle_int(suffledSelected, s->num_selected);
    for(i=0;i<s->num_discarded;i++){
        suffledNotSelected[i]=i;
    }
    
    generatedSuffledNotSelected(p, s,suffledNotSelected, mode);
    
    
    //for each selected item in s
    newProfit=s->value;
    i=0;
    //print_solution(s);
    while(newProfit<=s->value & i<s->num_selected){
        //Store modified solution

        emptyExistingSolution(p, sFinale);
        
        //cout<< "adding existing items" <<endl;
        for(int ida=0;ida<s->num_selected;ida++){
            if(ida != suffledSelected[i]){
                //Add rest of solution to sa, except one selected item
                viol = check_and_add_item(sFinale, s->selected[ida], p);
                //print_solution(sa);
            }
        }
        //cout<< "adding discarded items" <<endl;
        //Ajouter discarded items randomly
        for(j=0;j<s->num_discarded;j++){
            //cout<<"now try to add "<<s->discarded[suffledNotSelected[j]]<<" corresponding profit: "<<p->profits[s->discarded[suffledNotSelected[j]]]<<endl;
            viol = check_and_add_item(sFinale, s->discarded[suffledNotSelected[j]], p);
            //print_solution(sa);
        }
        i++;
        newProfit=sFinale->value;
        //cout<< "FI SA value: "<<sa->value<<endl;
    }
    if(newProfit<=s->value){
        clone_solution(s, sFinale, p);
    }
}

void amelirate_solution_BI(problem *p, solution *s,solution *sa, solution *sFinale, int mode){
    
    //Store final solution
    //solution *sa=create_empty_solution(p);
    //solution *sFinale=create_empty_solution(p);

    int suffledSelected[s->num_selected];
    
    //int *suffledSelected = create_shuffled(s->num_selected);
    int suffledNotSelected[s->num_discarded];
    int i=0,j,viol, maxProfit;
    
    for (i = 0 ; i < s->num_selected ; i++) suffledSelected[i] = i;
    
    for(i=0;i<s->num_discarded;i++){
        suffledNotSelected[i]=i;
    }
    
    generatedSuffledNotSelected(p, s,suffledNotSelected, mode);
    
    
    //for each selected item in s
    maxProfit=s->value;
    i=0;
    //print_solution(s);
    //gothrough all possible neighbors, k=1
    while(i<s->num_selected){
        //Store modified solution
        emptyExistingSolution(p, sa);
        //cout<< "adding existing items" <<endl;
        for(int ida=0;ida<s->num_selected;ida++){
            if(ida != suffledSelected[i]){
                //Add rest of solution to sa, except one selected item
                viol = check_and_add_item(sa, s->selected[ida], p);
                //print_solution(sa);
            }
        }
        //cout<< "adding discarded items" <<endl;
        //Ajouter discarded items randomly
        for(j=0;j<s->num_discarded;j++){
            //cout<<"now try to add "<<s->discarded[suffledNotSelected[j]]<<" corresponding profit: "<<p->profits[s->discarded[suffledNotSelected[j]]]<<endl;
            viol = check_and_add_item(sa, s->discarded[suffledNotSelected[j]], p);
            //print_solution(sa);
        }
        i++;
        if(sa->value>maxProfit){
            clone_solution(sa,sFinale, p);
            maxProfit=sa->value;
            //cout<< "BI maxProfit value: "<<sa->value<<endl;
        }
        
        //cout<< "BI SA value: "<<sa->value<<endl;
    }
    //destroy_solution(sa);
    if(maxProfit<=s->value){
        clone_solution(s, sFinale, p);
    }
    
}
//extract all combinations from given number.
void combine (int *arr,int start,int *result, stack<int> &resultStack,int index,int n,int arr_len)
{
    int ct = 0;
    for(ct = start;ct < arr_len-index+1;ct++){
        result[index-1] = ct;
        if(index-1==0){
            int j;
            for(j = n-1;j>=0;j--){
                resultStack.push(arr[result[j]]);
            }
        }
        else
            combine(arr,ct+1,result,resultStack,index-1,n,arr_len);
    }
}
//VND solution, FI based
void ameliorateSolutionVND(int k,solution *s,problem *p,solution *sa, solution *sFinale, int mode){
    //solution *sa=create_empty_solution(p);
    //solution *sFinale=create_empty_solution(p);
    int i,j,idx,viol,maxProfit;
    int suffledNotSelected[s->num_discarded];

    maxProfit=s->value;
    
    for(i=0;i<s->num_discarded;i++){
        suffledNotSelected[i]=i;
    }
    
    generatedSuffledNotSelected(p, s,suffledNotSelected, mode);
    
    //generate selected part of new solution
    if(k>=s->num_selected){
        emptyExistingSolution(p, sa);
        //add discarded items
        for(j=0;j<s->num_discarded;j++){
            //cout<<"now try to add "<<s->discarded[suffledNotSelected[j]]<<" corresponding profit: "<<p->profits[s->discarded[suffledNotSelected[j]]]<<endl;
            viol = check_and_add_item(sa, s->discarded[suffledNotSelected[j]], p);
            //print_solution(sa);
        }
        
        if(sa->value>maxProfit){
            clone_solution(sa,sFinale, p);
            maxProfit=sa->value;
            //cout<< "BI maxProfit value: "<<sa->value<<endl;
        }
        
    }
    else{
    
    bool addable;
    
        
    int arr[s->num_selected];
    for(i=0;i<s->num_selected;i++) arr[i]=s->selected[i];
    shuffle_int(arr, s->num_selected);

    int result[4];
    stack<int> resultStack;
    
    int arr_len = s->num_selected;
    
    int n = k,index = k;
    int ct = 0;
        if(k<=2){
            combine(arr,ct,result,resultStack,index,n,arr_len);
            int numCombination=combinator(s->num_selected, k);
            //cout<<"num comb.: "<<numCombination<<endl;
            //for each combination not to added into selection
            int execptedSelection[k];
            //for each combination
            i=0;
            while(maxProfit<=s->value & i<numCombination){
                emptyExistingSolution(p, sa);
                //pop k elements
                for(j=0;j<k;j++){
                    execptedSelection[j]=resultStack.top();
                    //cout<<execptedSelection[j]<<endl;
                    resultStack.pop();
                }
                //add rest elements into solution
                for(j=0;j<s->num_selected;j++){
                    addable=true;
                    for(idx=0;idx<k;idx++){
                        if(s->selected[j]==execptedSelection[idx]) addable=false;
                    }
                    if(addable) viol=check_and_add_item(sa, s->selected[j], p);
                }
                //add discarded items
                for(j=0;j<s->num_discarded;j++){
                    //cout<<"now try to add "<<s->discarded[suffledNotSelected[j]]<<" corresponding profit: "<<p->profits[s->discarded[suffledNotSelected[j]]]<<endl;
                    viol = check_and_add_item(sa, s->discarded[suffledNotSelected[j]], p);
                    //print_solution(sa);
                }
                //cout<<"VND SA VALUE: "<<sa->value<<endl;
                if(sa->value>maxProfit){
                    clone_solution(sa,sFinale, p);
                    maxProfit=sa->value;
                }
                i++;
            }
        }
        else if(k==3){
            int execptedSelection[k];
            
            int k1=0,k2=1,k3=2,max_ctrl=0;
            while(maxProfit<=s->value &k1<s->num_selected-2 ){
                while(maxProfit<=s->value & k2<s->num_selected-1 ){
                    while(maxProfit<=s->value & k3<s->num_selected ){
                        execptedSelection[0]=arr[k1];
                        execptedSelection[1]=arr[k2];
                        execptedSelection[2]=arr[k3];
                        max_ctrl++;
                        k3++;
                        emptyExistingSolution(p, sa);
                        //cout<<execptedSelection[0]<<" "<<execptedSelection[1]<<" "<<execptedSelection[2]<<endl;
                        //add rest elements into solution
                        for(j=0;j<s->num_selected;j++){
                            addable=true;
                            for(idx=0;idx<k;idx++){
                                if(s->selected[j]==execptedSelection[idx]) addable=false;
                            }
                            if(addable) viol=check_and_add_item(sa, s->selected[j], p);
                        }
                        //add discarded items
                        for(j=0;j<s->num_discarded;j++){
                            //cout<<"now try to add "<<s->discarded[suffledNotSelected[j]]<<" corresponding profit: "<<p->profits[s->discarded[suffledNotSelected[j]]]<<endl;
                            viol = check_and_add_item(sa, s->discarded[suffledNotSelected[j]], p);
                            //print_solution(sa);
                        }
                        //cout<<"VND SA VALUE: "<<sa->value<<endl;
                        //cout<<"SA value: "<<sa->value<<" S value: "<<s->value<<endl;
                        if(sa->value>maxProfit){
                            clone_solution(sa,sFinale, p);
                            maxProfit=sa->value;
                            //cout<<"max Profit: "<<maxProfit<<" S value: "<<s->value<<endl;
                        }
                    }
                    k2++;
                    k3=k2+1;
                }
                k1++;
                k2=k1+1;
                k3=k2+1;
            }
            
        }

    }
    if(maxProfit<=s->value){
        clone_solution(s, sFinale, p);
    }
}

void ameliorate_recursive(problem *p, solution *s,solution *sa, solution *sFinale,solution *sTemp, int mode, int facon,int k){
    //facon=1 --> FI
    emptyExistingSolution(p, sTemp);
    if(facon==1){
        int oldValue=s->value;
        
        amelirate_solution_FI(p, s, sFinale, mode);
        //cout<<"recursive 1 value: "<<sa->value<<endl;
        while(sFinale->value >oldValue){
            emptyExistingSolution(p, sTemp);
            clone_solution(sFinale, sTemp,p);
            oldValue=sFinale->value;
            amelirate_solution_FI(p, sTemp,sFinale, mode);
            //cout<<"recursive value: "<<sa->value<<endl;
        }
        
    }
    //facon = 2 -> BI
    else if(facon==2){
        int oldValue=s->value;
        
        amelirate_solution_BI(p, s,sa, sFinale, mode);
        //cout<<"recursive 1 value: "<<sa->value<<endl;
        while(sFinale->value >oldValue){
            oldValue=sFinale->value;
            clone_solution(sFinale, sTemp,p);
            amelirate_solution_BI(p, sTemp,sa,sFinale, mode);
            //cout<<"recursive value: "<<sa->value<<endl;
        }
    }
    else if(facon==3){
        int oldValue=s->value;
        double improve=10;
        int itrCounter=0;
        ameliorateSolutionVND(k, s, p,sa,sFinale, mode);
        //cout<<"recursive 1 value: "<<sa->value<<endl;
        
        while(sFinale->value >oldValue & improve>0.0001){
            
            itrCounter++;
            oldValue=sFinale->value;
            
            clone_solution(sFinale, sTemp,p);
            ameliorateSolutionVND(k, sTemp,p,sa,sFinale, mode);
            improve=((double)(sFinale->value-oldValue))/((double)oldValue);
            //cout<<"VND ITR: "<<itrCounter<<" improve1: "<<improve<<endl;
            //cout<<"recursive value: "<<sa->value<<endl;
        }
    }

}
