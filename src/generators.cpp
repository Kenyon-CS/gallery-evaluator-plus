#include "generators.h"
#include <algorithm>
#include <random>
#include <chrono>

namespace {
std::mt19937& rng() {
    static std::mt19937 gen(static_cast<unsigned>(
        std::chrono::high_resolution_clock::now().time_since_epoch().count()));
    return gen;
}

bool lightingMatches(const Painting& p, const std::string& wallLight) {
    return p.lightingNeed == wallLight;
}
}

Layout generateRandomLayout(
    const std::vector<Painting>& paintings,
    const std::string& layoutId,
    const std::string& wallLight,
    double wallWidthFt
) {
    Layout layout;
    layout.id = layoutId;
    layout.description = "Randomly generated layout";
    layout.wallLight = wallLight;

    std::vector<Painting> shuffled = paintings;
    std::shuffle(shuffled.begin(), shuffled.end(), rng());

    std::uniform_int_distribution<int> countDist(4, 10);
    int targetCount = countDist(rng());

    double x = 0.0;
    std::uniform_real_distribution<double> gapDist(0.25, 0.9);

    for (const auto& p : shuffled) {
        if (static_cast<int>(layout.placements.size()) >= targetCount) break;
        if (x + p.widthFt > wallWidthFt) continue;
        layout.placements.push_back({p.id, x});
        x += p.widthFt + gapDist(rng());
        if (x > wallWidthFt) break;
    }

    return layout;
}

Layout generateGreedyLayout(
    const std::vector<Painting>& paintings,
    const std::string& layoutId,
    const std::string& wallLight,
    double wallWidthFt
) {
    Layout layout;
    layout.id = layoutId;
    layout.description = "Greedy layout: prefer lighting match, then theme clusters, then smaller widths";
    layout.wallLight = wallLight;

    std::vector<Painting> candidates = paintings;
    std::stable_sort(candidates.begin(), candidates.end(), [&](const Painting& a, const Painting& b) {
        bool am = lightingMatches(a, wallLight);
        bool bm = lightingMatches(b, wallLight);
        if (am != bm) return am > bm;
        if (a.theme != b.theme) return a.theme < b.theme;
        return a.widthFt < b.widthFt;
    });

    double x = 0.0;
    const double targetGap = 0.6;

    for (const auto& p : candidates) {
        if (layout.placements.size() >= 10) break;
        if (x + p.widthFt > wallWidthFt) continue;
        layout.placements.push_back({p.id, x});
        x += p.widthFt + targetGap;
    }

    if (layout.placements.size() < 4) {
        layout.placements.clear();
        x = 0.0;
        std::vector<Painting> byWidth = paintings;
        std::sort(byWidth.begin(), byWidth.end(), [](const Painting& a, const Painting& b) {
            return a.widthFt < b.widthFt;
        });
        for (const auto& p : byWidth) {
            if (layout.placements.size() >= 4) break;
            if (x + p.widthFt > wallWidthFt) continue;
            layout.placements.push_back({p.id, x});
            x += p.widthFt + 0.4;
        }
    }

    return layout;
}
