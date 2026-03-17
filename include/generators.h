#ifndef GENERATORS_H
#define GENERATORS_H

#include "models.h"
#include <vector>
#include <unordered_map>

Layout generateRandomLayout(
    const std::vector<Painting>& paintings,
    const std::string& layoutId,
    const std::string& wallLight,
    double wallWidthFt = 10.0
);

Layout generateGreedyLayout(
    const std::vector<Painting>& paintings,
    const std::string& layoutId,
    const std::string& wallLight,
    double wallWidthFt = 10.0
);

#endif
