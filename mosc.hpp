/**	
	Guillaume Elias 2020. Use this file at your own peril.
	This project is under BSD3 license. See LICENSE file.
*/

#pragma once


#include "userosc.h"


typedef struct Osc {
  float w0;
  float phase;
  uint16_t type;
  float pulse_width;
  
  uint16_t interval;
  uint16_t fine_interval;
} Osc;