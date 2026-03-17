#include "parser.h"
#include "evaluator.h"
#include "generators.h"
#include "csv_writer.h"

#include <iostream>
#include <unordered_map>
#include <iomanip>
#include <vector>
#include <algorithm>

int main() {
    try {
        const double wallWidthFt = 10.0;

        auto paintings = loadPaintings("data/paintings.yaml");
        auto layouts = loadLayouts("data/layouts.yaml");

        std::unordered_map<std::string, Painting> paintingsById;
        for (const auto& p : paintings) {
            paintingsById[p.id] = p;
        }

        std::vector<std::pair<Layout, ScoreBreakdown>> allRows;

        std::cout << "Loaded " << paintings.size() << " paintings\n";
        std::cout << "Loaded " << layouts.size() << " predefined layouts\n\n";

        for (const auto& layout : layouts) {
            auto score = evaluateLayout(layout, paintingsById, wallWidthFt);
            allRows.push_back({layout, score});
        }

        Layout randomLayout = generateRandomLayout(paintings, "generated_random", "medium", wallWidthFt);
        Layout greedyLayout = generateGreedyLayout(paintings, "generated_greedy", "medium", wallWidthFt);

        allRows.push_back({randomLayout, evaluateLayout(randomLayout, paintingsById, wallWidthFt)});
        allRows.push_back({greedyLayout, evaluateLayout(greedyLayout, paintingsById, wallWidthFt)});

        std::sort(allRows.begin(), allRows.end(), [](const auto& a, const auto& b) {
            return a.second.total > b.second.total;
        });

        std::cout << std::left
                  << std::setw(18) << "Layout"
                  << std::setw(12) << "WallLight"
                  << std::setw(10) << "Paintings"
                  << std::setw(12) << "Total"
                  << std::setw(10) << "Fit"
                  << std::setw(10) << "Theme"
                  << std::setw(10) << "Period"
                  << std::setw(10) << "Light"
                  << '\n';
        std::cout << std::string(82, '-') << '\n';

        for (const auto& row : allRows) {
            const auto& layout = row.first;
            const auto& score = row.second;
            std::cout << std::setw(18) << layout.id
                      << std::setw(12) << layout.wallLight
                      << std::setw(10) << layout.placements.size()
                      << std::setw(12) << std::fixed << std::setprecision(2) << score.total
                      << std::setw(10) << score.fitScore
                      << std::setw(10) << score.themeScore
                      << std::setw(10) << score.periodScore
                      << std::setw(10) << score.lightingScore
                      << '\n';
        }

        writeScoresCSV("output/scores.csv", allRows);
        std::cout << "\nWrote output/scores.csv\n";
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << '\n';
        return 1;
    }

    return 0;
}
