#ifndef EVALUATOR_H
#define EVALUATOR_H

#include "models.h"
#include <unordered_map>

ScoreBreakdown evaluateLayout(
    const Layout& layout,
    const std::unordered_map<std::string, Painting>& paintingsById,
    double wallWidthFt = 10.0
);

#endif
