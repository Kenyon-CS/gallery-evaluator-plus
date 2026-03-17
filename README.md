# Gallery Evaluator Plus

A small C++17 starter project for experimenting with gallery-layout evaluation and simple layout-generation algorithms.

This project is designed for an algorithms course. It includes:

- 20 paintings stored in YAML
- 10 example layouts stored in YAML
- A fixed 10-foot wall model
- Evaluation functions for fit, spacing, overlap, theme coherence, period coherence, and lighting match
- A random layout generator
- A greedy layout generator
- CSV export for comparing layouts

## Project Structure

```text
gallery-evaluator-plus/
├── CMakeLists.txt
├── Makefile
├── README.md
├── data/
│   ├── paintings.yaml
│   └── layouts.yaml
├── include/
│   ├── models.h
│   ├── parser.h
│   ├── evaluator.h
│   ├── generators.h
│   └── csv_writer.h
└── src/
    ├── main.cpp
    ├── parser.cpp
    ├── evaluator.cpp
    ├── generators.cpp
    └── csv_writer.cpp
```

## Build

### With `make`

```bash
make
```

### With CMake

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

## Run

```bash
./gallery_eval_plus
```

The program will:

1. Load the 20 paintings from `data/paintings.yaml`
2. Load the 10 layouts from `data/layouts.yaml`
3. Evaluate all provided layouts
4. Generate one random layout
5. Generate one greedy layout
6. Export all results to `output/scores.csv`

## Notes on YAML

This project uses a **small custom parser** for a restricted YAML subset. It is intentionally simple so students can run the project without installing external YAML libraries.

The parser expects the structure used in the included data files. It is not a full YAML implementation.

## Data Model

### Paintings
Each painting has:

- `id`
- `title`
- `artist`
- `year`
- `period`
- `theme`
- `brightness` (0.0 to 1.0)
- `lighting_need` (`low`, `medium`, `high`)
- `width_ft`

### Layouts
Each layout has:

- `id`
- `description`
- `wall_light` (`low`, `medium`, `high`)
- `placements`
  - `painting_id`
  - `x_ft`

All layouts are placed on a 10-foot wall.

## Evaluation Overview

Each layout gets scored using weighted components:

- **fit score**: rewards paintings that fit on the wall
- **spacing score**: rewards clean spacing between adjacent works
- **overlap penalty**: heavily penalizes overlap
- **theme coherence**: rewards neighboring paintings with same or similar themes
- **period coherence**: rewards neighboring paintings from similar time periods
- **lighting match**: rewards lighting needs that match the wall lighting

## Example Extensions for Students

- Add new themes or periods
- Tune the evaluation weights
- Create a hill-climbing improvement pass
- Add simulated annealing
- Add multiple walls or rooms
- Support vertical placement and artwork height

## Output

A CSV file is written to:

```text
output/scores.csv
```

This can be opened in Excel, Numbers, Google Sheets, or imported into Python/R for analysis.

## License

This starter project is provided for teaching and experimentation.
