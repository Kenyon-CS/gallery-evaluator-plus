#ifndef PARSER_H
#define PARSER_H

#include "models.h"
#include <string>
#include <vector>

std::vector<Painting> loadPaintings(const std::string& filename);
std::vector<Layout> loadLayouts(const std::string& filename);

#endif
