#ifndef SCORE_H
#define SCORE_H

#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif

#include "entity.h"
#include "helper_functions.h"

void checkUserScores(std::vector<EntityScore>& scores, INT16& points);
void checkGroupScores(std::vector<EntityScore>& scores, INT16& points);

#endif