#pragma once

void setup_sid_parameters();
void setup_sid_parameter_menu();

#include <LinkedList.h>
#include "parameters/Parameter.h"

extern LinkedList<BaseParameter*> available_parameters;
