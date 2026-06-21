# Ocean Pollution Tracking and Cleanup System
---

## Project Title

Ocean Pollution Tracking and Cleanup System

## Problem Statement

Marine pollution from oil spills, plastic waste, industrial discharge, and chemical contamination threatens ecosystems, biodiversity, and human health, and it spreads rapidly through interconnected ocean regions. Environmental agencies deploy thousands of sensors that continuously measure pollution indicators such as chemical concentration, oil presence, and water quality, but without a system to ingest, rank, and act on that data quickly, response efforts lag behind the spread of contamination. This project builds a console-based system that ingests live sensor telemetry, ranks affected regions by severity, predicts how contamination will spread across connected regions, and routes cleanup vessels along the fastest available path.

## Objectives

- Track incoming marine pollution sensor readings in the order they are received.
- Rank ocean regions by cumulative contamination level to prioritize cleanup effort.
- Predict how contamination spreads outward from an affected region into neighboring regions.
- Compute the fastest route for a cleanup vessel to reach an affected region.
- Generate a persistent, file-based environmental report for record-keeping.

## System Overview / Architecture

The system is built around a single `OceanSystem` class that owns three core data members and exposes one method per requirement:

```
                    +--------------------------+
   Sensor_Inputs.txt|   loadSensorDataFromFile |
   (region, level)  +------------+-------------+
                                 |
                                 v
                        +-----------------+
                        |  sensorQueue    |  <- std::queue<SensorReading>
                        |  (FIFO buffer)  |
                        +--------+--------+
                                 |
                         processSensorQueue()
                                 |
                                 v
                     +------------------------+
                     |  regions[] (vector)     |  <- pollutionLevel accumulates here
                     +-----------+------------+
                                 |
              +------------------+------------------+
              |                                      |
   getRankedPollutedRegions()              predictPollutionSpread()
   (Merge Sort on pollutionLevel)          (BFS over adjList, layer by layer)
              |                                      |
              v                                      v
      Ranked severity list                  Spread alerts by hop distance

                     +------------------------+
                     |  adjList[] (weighted    |  <- vector<vector<pair<id,time>>>
                     |  adjacency list)        |
                     +-----------+------------+
                                 |
                       dispatchCleanupVessel()
                       (Dijkstra shortest path)
                                 |
                                 v
                   Vessel route + transit time

                     generateEnvironmentalReport()
                                 |
                                 v
                     Ocean_System_Report.txt
```

Five regions (North Port, Coral Reef Matrix, Deep Oceanic Trench, Eastern Trade Gulf, Industrial Delta) are hardcoded at startup as nodes in a weighted, bidirectional graph representing maritime travel lanes between them. A menu-driven `main()` loop lets the user trigger ingestion, processing, ranking, spread forecasting, vessel routing, and report generation, with a live dashboard redrawn after every action.

## Data Structures and Algorithms Used

| Requirement | Data Structure | Algorithm | Why |
|---|---|---|---|
| Sensor ingestion | `std::queue<SensorReading>` | FIFO push/pop | Guarantees readings are processed in the exact order sensors transmitted them — critical for real-time monitoring. |
| Pollution ranking | `std::vector<Region>` | Custom Merge Sort (descending) | Stable, guaranteed O(n log n) regardless of input order, well suited to repeatedly re-ranking a dataset that updates frequently. |
| Spread prediction | `std::vector<vector<pair<int,int>>>` adjacency list + `std::queue<pair<int,int>>` | Breadth-First Search | Explores regions in order of hop-distance from the source, naturally producing "layer 1 / layer 2…" spread alerts. |
| Vessel routing | Same adjacency list + `std::priority_queue` | Dijkstra's Algorithm | Edges are weighted by travel time, so a shortest-weighted-path algorithm is required rather than plain BFS. |

## Implementation Approach

1. **Modeling**: Two structs, `SensorReading` (a single telemetry packet) and `Region` (a node with cumulative pollution level), keep raw data separate from aggregated state.
2. **Ingestion**: `loadSensorDataFromFile()` reads `(regionId, pollutionLevel)` pairs from a text file and pushes each as a `SensorReading` onto `sensorQueue`, validating the region ID before accepting it.
3. **Processing**: `processSensorQueue()` pops every reading off the queue, in arrival order, and accumulates its pollution level into the matching region's running total.
4. **Ranking**: `getRankedPollutedRegions()` copies the active regions into a vector and sorts it with a hand-written merge sort, ordering by `pollutionLevel` descending.
5. **Spread forecasting**: `predictPollutionSpread()` runs a standard BFS from a chosen source region, tagging each newly discovered region with its hop-distance ("layer") from the source.
6. **Routing**: `dispatchCleanupVessel()` runs Dijkstra's algorithm from the base region to a target, reconstructing the path via a parent array once the shortest distance is found.
7. **Reporting**: `generateEnvironmentalReport()` writes a snapshot of all region pollution levels and the outstanding queue size to `Ocean_System_Report.txt`.
8. **Input safety**: all public methods that take a region ID validate it against the vector's bounds *before* indexing, and a `readInt()` helper recovers cleanly if the user types non-numeric input at any prompt.

## Time and Space Complexity Analysis

| Operation | Time Complexity | Space Complexity |
|---|---|---|
| Enqueue / Dequeue sensor reading | O(1) per reading | O(k) for k pending readings |
| Process entire queue | O(k) for k readings | O(1) auxiliary |
| Merge Sort ranking | O(n log n) | O(n) auxiliary (temporary L/R arrays) |
| BFS spread detection | O(V + E) | O(V) for visited array and queue |
| Dijkstra shortest path | O((V + E) log V) with a binary heap priority queue | O(V) for distance/parent arrays |
| Report generation | O(V) | O(1) auxiliary |

Here V is the number of regions and E is the number of maritime connections between them; k is the number of sensor readings ingested in a given batch.

## Execution Steps

1. **Compile** (requires a C++17-capable compiler):
   ```
   g++ -std=c++17 -O2 -o ocean_pollution_system ocean_pollution_system.cpp
   ```
2. **Prepare the sensor input file.** Create `Sensor_Inputs.txt` in the same folder as the executable, with one `regionId pollutionLevel` pair per line (region IDs 1–5 are valid). Example:
   ```
   1 25
   2 40
   3 15
   4 60
   5 30
   1 10
   4 20
   ```
3. **Run the program:**
   ```
   ./ocean_pollution_system
   ```
4. **Use the menu:**
   - `1` — loads `Sensor_Inputs.txt` into the FIFO queue.
   - `2` — drains the queue, accumulating pollution levels into each region.
   - `3` — ranks regions from most to least contaminated (Merge Sort).
   - `4` — pick a source region (1–5) to forecast contamination spread (BFS).
   - `5` — pick a target region (1–5) to route a cleanup vessel from North Port (Dijkstra).
   - `6` — writes `Ocean_System_Report.txt` to disk.
   - `0` — exits the program.
5. After every action, press **Enter** to return to the dashboard and choose the next action.

## Sample Inputs and Outputs

**Sample `Sensor_Inputs.txt`:**
```
1 25
2 40
3 15
4 60
5 30
1 10
4 20
```

**Option 1 — Loading the file:**
```
[Success] Ingested 7 telemetry packets into the FIFO Queue.
```

**Option 2 — Processing the queue:**
```
--- Emptying Queue Pipeline (First-In, First-Out) ---
 -> Dispatched Packet #7001 to North Port (Base)
 -> Dispatched Packet #7002 to Coral Reef Matrix
 -> Dispatched Packet #7003 to Deep Oceanic Trench
 -> Dispatched Packet #7004 to Eastern Trade Gulf
 -> Dispatched Packet #7005 to Industrial Delta
 -> Dispatched Packet #7006 to North Port (Base)
 -> Dispatched Packet #7007 to Eastern Trade Gulf
```

**Option 3 — Ranking (Merge Sort):**
```
--- Priority Sorting Assessment (Merge Sort) ---
Ranked Impact Hierarchy:
 1. Eastern Trade Gulf (Level: 80)
 2. Coral Reef Matrix (Level: 40)
 3. North Port (Base) (Level: 35)
 4. Industrial Delta (Level: 30)
 5. Deep Oceanic Trench (Level: 15)
```

**Option 4 — Spread forecast from region 1 (BFS):**
```
--- Environmental Vector Expansion Forecasting (BFS) ---
 [!] Spread Layer 1 Alert: Contamination expanding to -> Coral Reef Matrix
 [!] Spread Layer 1 Alert: Contamination expanding to -> Eastern Trade Gulf
 [!] Spread Layer 2 Alert: Contamination expanding to -> Deep Oceanic Trench
 [!] Spread Layer 2 Alert: Contamination expanding to -> Industrial Delta
```

**Option 5 — Vessel routing to region 5 (Dijkstra):**
```
--- Calculating Interconnected Travel Vectors (Dijkstra) ---
Vessel Vector Confirmed. Transit Estimate: 9 hours.
Navigation Leg: North Port (Base) -> Coral Reef Matrix -> Eastern Trade Gulf -> Industrial Delta
```

**Option 6 — Generated `Ocean_System_Report.txt`:**
```
====================================================
    ECO-LOGISTICS MARITIME INTELLIGENCE REPORT
====================================================

Active Incidents Awaiting Processing Queue: 0

CURRENT POLLUTION METRICS BY REGION:
----------------------------------------------------
Region ID [1] -> Name: North Port (Base) | Total Contamination Load: 35 units
Region ID [2] -> Name: Coral Reef Matrix | Total Contamination Load: 40 units
Region ID [3] -> Name: Deep Oceanic Trench | Total Contamination Load: 15 units
Region ID [4] -> Name: Eastern Trade Gulf | Total Contamination Load: 80 units
Region ID [5] -> Name: Industrial Delta | Total Contamination Load: 30 units

================= END OF REPORT ====================
```

## Screenshots

> Run the program yourself and paste terminal screenshots of the live dashboard here for submission — for example, a screenshot after Option 1 (queue populated), after Option 2 (pollution bars filled in), and after Option 4 or 5 (spread/routing output). The dashboard's `[#####----------]` bars and live queue stream make for clear, visual screenshots.

`[ Insert Screenshot 1: Dashboard after loading sensor data ]`
`[ Insert Screenshot 2: Dashboard after processing queue, showing pollution bars ]`
`[ Insert Screenshot 3: Merge Sort ranked output ]`
`[ Insert Screenshot 4: BFS spread forecast output ]`
`[ Insert Screenshot 5: Dijkstra routing output ]`

## Results and Observations

- The FIFO queue correctly preserved arrival order: packets 7001–7007 were dispatched in exactly the sequence they were ingested, confirming the queue behaves as intended for time-sensitive sensor data.
- Merge Sort correctly ranked Eastern Trade Gulf as the most contaminated region (80 units) after two readings landed on it, demonstrating that the ranking reflects cumulative load rather than a single reading.
- BFS correctly distinguished directly-connected regions (Layer 1: Coral Reef Matrix, Eastern Trade Gulf) from regions reachable only through an intermediate hop (Layer 2: Deep Oceanic Trench, Industrial Delta), matching the graph's actual connectivity.
- Dijkstra found a 9-hour route to Industrial Delta via North Port → Coral Reef Matrix → Eastern Trade Gulf → Industrial Delta rather than the direct North Port → Eastern Trade Gulf → Industrial Delta path, because it correctly accounted for edge weights rather than hop count — the direct-looking route is actually slower once travel times are summed.
- The system handles an empty queue, an unreachable region, and invalid menu input without crashing, after bounds-checking was added to every method that accepts a user-supplied region ID.

## Conclusion

This project demonstrates how classical data structures and algorithms map directly onto a real-world environmental monitoring problem: a Queue enforces correct temporal ordering of incoming sensor data, Merge Sort reliably prioritizes cleanup effort by contamination severity, BFS models how pollution propagates across connected regions, and Dijkstra's algorithm finds the most time-efficient cleanup vessel routes across a weighted maritime network. Together these four components — wrapped in a live dashboard and persistent reporting — fulfill the case study's goal of continuous monitoring and coordinated, data-driven cleanup response.
