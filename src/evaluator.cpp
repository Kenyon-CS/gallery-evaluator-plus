#include "evaluator.h"
#include <vector>
#include <algorithm>
#include <cmath>

namespace {
int lightLevel(const std::string& s) {
    if (s == "low") return 0;
    if (s == "medium") return 1;
    return 2;
}

double themeSimilarity(const std::string& a, const std::string& b) {
    if (a == b) return 1.0;
    if ((a == "landscape" && b == "cityscape") || (a == "cityscape" && b == "landscape")) return 0.6;
    if ((a == "portrait" && b == "child_portrait") || (a == "child_portrait" && b == "portrait")) return 0.7;
    if ((a == "political" && b == "history") || (a == "history" && b == "political")) return 0.6;
    return 0.0;
}

double periodSimilarity(int yearA, int yearB) {
    double diff = std::abs(yearA - yearB);
    double score = 1.0 - std::min(diff / 300.0, 1.0);
    return score;
}
}

ScoreBreakdown evaluateLayout(
    const Layout& layout,
    const std::unordered_map<std::string, Painting>& paintingsById,
    double wallWidthFt
) {
    ScoreBreakdown score;

    struct SpanItem {
        double left;
        double right;
        const Painting* painting;
    };

    std::vector<SpanItem> spans;

    for (const auto& placement : layout.placements) {
        auto it = paintingsById.find(placement.paintingId);
        if (it == paintingsById.end()) continue;
        const Painting& p = it->second;
        double left = placement.xFt;
        double right = placement.xFt + p.widthFt;

        if (left < 0.0 || right > wallWidthFt) {
            score.fitScore -= 40.0;
            score.validFit = false;
        } else {
            score.fitScore += 10.0;
        }

        int wall = lightLevel(layout.wallLight);
        int need = lightLevel(p.lightingNeed);
        int diff = std::abs(wall - need);
        score.lightingScore += (diff == 0 ? 8.0 : (diff == 1 ? 3.0 : -6.0));

        spans.push_back({left, right, &p});
    }

    std::sort(spans.begin(), spans.end(), [](const SpanItem& a, const SpanItem& b) {
        return a.left < b.left;
    });

    for (size_t i = 0; i + 1 < spans.size(); ++i) {
        double gap = spans[i + 1].left - spans[i].right;
        if (gap < 0.0) {
            score.overlapPenalty += 60.0 * std::abs(gap);
            score.spacingScore -= 20.0;
        } else if (gap >= 0.4 && gap <= 1.2) {
            score.spacingScore += 12.0;
        } else if (gap > 0.0 && gap < 0.4) {
            score.spacingScore += 3.0;
        } else if (gap > 1.2 && gap <= 2.2) {
            score.spacingScore += 2.0;
        } else {
            score.spacingScore -= 4.0;
        }

        score.themeScore += 10.0 * themeSimilarity(spans[i].painting->theme, spans[i + 1].painting->theme);
        score.periodScore += 8.0 * periodSimilarity(spans[i].painting->year, spans[i + 1].painting->year);
    }

    score.total =
        score.fitScore +
        score.spacingScore -
        score.overlapPenalty +
        score.themeScore +
        score.periodScore +
        score.lightingScore;

    return score;
}
