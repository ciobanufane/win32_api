#pragma once

#include <vector>
#include "user.h"

void checkUserScores(std::vector<EntityScore> & scores, INT16 & points);
void checkGroupScores(std::vector<EntityScore> & scores, INT16 & points);

static int searchForEntityName(std::vector<Entity>& entities, LPCTSTR name);
static bool testEntityScoresForDuplicateName(std::vector<EntityScore>& scores);