#include "csv_writer.h"
#include <fstream>
#include <filesystem>

void writeScoresCSV(
    const std::string& filename,
    const std::vector<std::pair<Layout, ScoreBreakdown>>& rows
) {
    std::filesystem::create_directories(std::filesystem::path(filename).parent_path());
    std::ofstream out(filename);
    out << "layout_id,description,wall_light,num_paintings,fit_score,spacing_score,overlap_penalty,theme_score,period_score,lighting_score,total,valid_fit\n";

    for (const auto& row : rows) {
        const Layout& layout = row.first;
        const ScoreBreakdown& s = row.second;
        out << layout.id << ','
            << '"' << layout.description << '"' << ','
            << layout.wallLight << ','
            << layout.placements.size() << ','
            << s.fitScore << ','
            << s.spacingScore << ','
            << s.overlapPenalty << ','
            << s.themeScore << ','
            << s.periodScore << ','
            << s.lightingScore << ','
            << s.total << ','
            << (s.validFit ? "true" : "false") << '\n';
    }
}
