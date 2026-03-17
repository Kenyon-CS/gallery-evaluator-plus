#include "parser.h"
#include <fstream>
#include <stdexcept>

namespace {
std::string trim(const std::string& s) {
    const auto start = s.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    const auto end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

std::string stripQuotes(const std::string& s) {
    std::string t = trim(s);
    if (t.size() >= 2 && ((t.front() == '"' && t.back() == '"') || (t.front() == '\'' && t.back() == '\''))) {
        return t.substr(1, t.size() - 2);
    }
    return t;
}

bool startsWith(const std::string& s, const std::string& prefix) {
    return s.rfind(prefix, 0) == 0;
}

std::string valueAfterColon(const std::string& line) {
    auto pos = line.find(':');
    if (pos == std::string::npos) return "";
    return stripQuotes(line.substr(pos + 1));
}
}

std::vector<Painting> loadPaintings(const std::string& filename) {
    std::ifstream in(filename);
    if (!in) throw std::runtime_error("Could not open paintings file: " + filename);

    std::vector<Painting> paintings;
    Painting current;
    bool inItem = false;
    std::string line;

    while (std::getline(in, line)) {
        std::string t = trim(line);
        if (t.empty() || t == "paintings:") continue;

        if (startsWith(t, "- id:")) {
            if (inItem) paintings.push_back(current);
            current = Painting{};
            current.id = valueAfterColon(t);
            inItem = true;
        } else if (startsWith(t, "title:")) {
            current.title = valueAfterColon(t);
        } else if (startsWith(t, "artist:")) {
            current.artist = valueAfterColon(t);
        } else if (startsWith(t, "year:")) {
            current.year = std::stoi(valueAfterColon(t));
        } else if (startsWith(t, "period:")) {
            current.period = valueAfterColon(t);
        } else if (startsWith(t, "theme:")) {
            current.theme = valueAfterColon(t);
        } else if (startsWith(t, "brightness:")) {
            current.brightness = std::stod(valueAfterColon(t));
        } else if (startsWith(t, "lighting_need:")) {
            current.lightingNeed = valueAfterColon(t);
        } else if (startsWith(t, "width_ft:")) {
            current.widthFt = std::stod(valueAfterColon(t));
        }
    }
    if (inItem) paintings.push_back(current);
    return paintings;
}

std::vector<Layout> loadLayouts(const std::string& filename) {
    std::ifstream in(filename);
    if (!in) throw std::runtime_error("Could not open layouts file: " + filename);

    std::vector<Layout> layouts;
    Layout currentLayout;
    bool inLayout = false;
    bool inPlacements = false;
    std::string line;

    while (std::getline(in, line)) {
        std::string t = trim(line);
        if (t.empty() || t == "layouts:") continue;

        if (startsWith(t, "- id:")) {
            if (inLayout) {
                layouts.push_back(currentLayout);
            }
            currentLayout = Layout{};
            currentLayout.id = valueAfterColon(t);
            inLayout = true;
            inPlacements = false;
        } else if (startsWith(t, "description:")) {
            currentLayout.description = valueAfterColon(t);
        } else if (startsWith(t, "wall_light:")) {
            currentLayout.wallLight = valueAfterColon(t);
        } else if (t == "placements:") {
            inPlacements = true;
        } else if (inPlacements && startsWith(t, "- painting_id:")) {
            Placement p;
            p.paintingId = valueAfterColon(t);
            currentLayout.placements.push_back(p);
        } else if (inPlacements && startsWith(t, "x_ft:")) {
            if (currentLayout.placements.empty()) {
                throw std::runtime_error("Malformed layouts file: x_ft before painting_id");
            }
            currentLayout.placements.back().xFt = std::stod(valueAfterColon(t));
        }
    }

    if (inLayout) layouts.push_back(currentLayout);
    return layouts;
}
