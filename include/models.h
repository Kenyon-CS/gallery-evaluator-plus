#ifndef MODELS_H
#define MODELS_H

#include <string>
#include <vector>

struct Painting {
    std::string id;
    std::string title;
    std::string artist;
    int year = 0;
    std::string period;
    std::string theme;
    double brightness = 0.0;
    std::string lightingNeed;
    double widthFt = 0.0;
};

struct Placement {
    std::string paintingId;
    double xFt = 0.0;
};

struct Layout {
    std::string id;
    std::string description;
    std::string wallLight;
    std::vector<Placement> placements;
};

struct ScoreBreakdown {
    double fitScore = 0.0;
    double spacingScore = 0.0;
    double overlapPenalty = 0.0;
    double themeScore = 0.0;
    double periodScore = 0.0;
    double lightingScore = 0.0;
    double total = 0.0;
    bool validFit = true;
};

#endif
