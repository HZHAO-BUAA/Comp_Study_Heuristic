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

#include "mkpneighbourhood.h"

void add_item(solution *s, int item, problem *p) {
  // if item was already in the solution, do nothing

  if (s->sol[item] == 0) {
    s->sol[item] = 1;
    s->selected[s->num_selected] = item;
    s->num_selected++;
    s->value += p->profits[item]; // delta evaluation

    // remove the item from the list of items not in the solution
    // probably not the most efficient way...
    int i = 0;
    while (i < s->num_discarded) {
      if (s->discarded[i] == item) break;
      i++;
    }
    while (i < s->num_discarded - 1) {
      s->discarded[i] = s->discarded[i+1];
      i++;
    }
    s->discarded[i] = 0;
    s->num_discarded = s->num_discarded - 1;
    
    // update list of resources used by the surrent solution
    for (i = 0 ; i < s->m ; i++) {
      s->resources_used[i] += p->constraints[i][item];
    }
  }
}
void remove_item_by_add(solution *s, int item, problem *p){
    int selected_items[s->num_selected];
    int idx, totalSelec=s->num_selected;
    for(idx=0;idx<s->num_selected;idx++)selected_items[idx]=s->selected[idx];
    emptyExistingSolution(p, s);
    for(idx=0;idx<totalSelec;idx++){
        if(selected_items[idx]!=item){
            check_and_add_item(s,selected_items[idx], p);
        }
    }

}
void remove_item(solution *s, int item, problem *p) {
    // if item was already in the solution, do nothing
    
    if (s->sol[item] == 1) {
        s->sol[item] = 0;
        s->discarded[s->num_discarded] = item;
        s->num_discarded++;
        s->value -= p->profits[item]; // delta evaluation
        
        // remove the item from the list of items not in the solution
        // probably not the most efficient way...
        int i = 0;
        while (i < s->num_selected) {
            if (s->selected[i] == item) break;
            i++;
        }
        while (i < s->num_selected - 1) {
            s->selected[i] = s->selected[i+1];
            i++;
        }
        s->selected[i] = 0;
        s->num_selected = s->num_selected - 1;
        
        // update list of resources used by the surrent solution
        for (i = 0 ; i < s->m ; i++) {
            s->resources_used[i] -= p->constraints[i][item];
        }
    }
}

bool check_item(solution *s, int item, problem *p) {
    int i, v = 0;
    // check whether adding the item would lead to violate some contraints
    // and count how many
    for (i = 0 ; i < s->m ; i++) {
        if (s->resources_used[i] + p->constraints[i][item] > p->capacities[i]) {
            v++;
        }
    }
    // if no constraint is violated, return true
    if (v == 0) {
        return true;
    }
    else{
        return false;
    }
}

int check_and_add_item(solution *s, int item, problem *p) {
  int i, v = 0; 
  // check whether adding the item would lead to violate some contraints
  // and count how many
  for (i = 0 ; i < s->m ; i++) {
    if (s->resources_used[i] + p->constraints[i][item] > p->capacities[i]) {
      v++;
    }
  }
  // if no constraint is violated, add the item to the solution
  if (v == 0) {
    add_item(s, item, p);
  }
  return(v);
}

int check_and_add_item(solution *s, int item, problem *p, float **contraints) {
    int i, v = 0;
    // check whether adding the item would lead to violate some contraints
    // and count how many
    for (i = 0 ; i < s->m ; i++) {
        if ((float)s->resources_used[i] + contraints[i][item] > 1) {
            v++;
        }
    }
    // if no constraint is violated, add the item to the solution
    if (v == 0) {
        add_item(s, item, p);
    }
    return(v);
}

