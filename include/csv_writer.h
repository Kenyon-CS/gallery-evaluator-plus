#ifndef CSV_WRITER_H
#define CSV_WRITER_H

#include "models.h"
#include <string>
#include <vector>

void writeScoresCSV(
    const std::string& filename,
    const std::vector<std::pair<Layout, ScoreBreakdown>>& rows
);

#endif
