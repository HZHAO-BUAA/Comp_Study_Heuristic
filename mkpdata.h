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

#ifndef __MKPDATA_H__
#define __MKPDATA_H__

#include <stdio.h>
#include <math.h>
#include <limits.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

/*
 * generic solution for a MKP problem
 * 
 * n              : number of objects
 * m              : number of knapsacks
 * value          : objective function value
 * sol            : 0/1 vector (item j discarded/selected)
 *
 * num_selected   : number of items selected
 * selected       : array containing the items selected in the solution
 *                 (corresponding to 1 in sol)
 * num_discarded  : number of items discarded
 * discarded      : array containing the items not selected in the solution
 *                  (corresponding to 0 in sol)
 *
 * resources_used : vector of length m containing the amount of resources used
 *                  by the current solution
 * 
 * num_violated   : number of violated constraints
 * violated       : 0/1 vector of length m (1 : j-th constraint violated)
 */
typedef struct _solution {
  int n;
  int m;
  int value;
  int *sol;

  int num_selected;
  int *selected;
  int num_discarded;
  int *discarded;

  int *resources_used;
  int *violated;
  int num_violated;
} solution;

typedef struct _params {
  char *instance_file;
  int seed;
} params;


// could be used for both profit and cost
typedef struct _item_val {
  int id;
  double value;
} item_val;

#endif
