# Ocean Environmental Monitoring System

The **Ocean Environmental Monitoring System** is a C++ application that simulates a maritime logistics and ecological monitoring dashboard. It tracks pollution incidents across interconnected oceanic regions, forecasts contamination spread, and plans efficient cleanup operations using core Data Structures and Algorithms (DSA).

## Features

This system relies on a graph-based simulation model of the ocean and employs several classical algorithms:

1. **Live Data Ingestion (FIFO Queue)**  
   Simulates a continuous stream of telemetry packets coming from oceanic sensors. Alerts are enqueued into a First-In, First-Out (FIFO) queue from a local data file (`Sensor_Inputs.txt`) and processed sequentially.

2. **Pollution Hierarchy Ranking (Merge Sort)**  
   As pollution metrics are gathered, the system employs **Merge Sort** to quickly order the regions by contamination level in descending order. This helps prioritize emergency responses.

3. **Contamination Spread Forecasting (Breadth-First Search - BFS)**  
   Using **BFS**, the system predicts how a spill will expand outward through the maritime network layer by layer. This allows authorities to anticipate which regions will be affected next.

4. **Cleanup Vessel Routing (Dijkstra's Algorithm)**  
   When a severe incident occurs, **Dijkstra's Algorithm** is used to calculate the shortest travel time and exact maritime navigation path for a cleanup vessel dispatched from the base (Region 1) to the target coordinates.

5. **Environmental Audit Reporting**  
   The system compiles all active metrics and incidents into a structured audit file (`Ocean_System_Report.txt`) for persistent record-keeping.

## Project Structure

- `main.cpp` - The core application logic, algorithms, and interactive CLI dashboard.
- `Sensor_Inputs.txt` - Sample telemetry data ingested by the system queue (format: `RegionID PollutionLevel`).
- `Ocean_System_Report.txt` - Generated audit logs (created during execution).

## Getting Started

### Prerequisites

- A standard C++11 (or higher) compiler (e.g., `g++` or `clang++`).

### Compilation & Execution

1. **Compile the system:**
   ```bash
   g++ -o main main.cpp
   ```

2. **Run the simulation:**
   ```bash
   ./main
   ```

*(Ensure `Sensor_Inputs.txt` is located in the same directory as the executable prior to running the ingestion module).*

## Interactive Dashboard Menu

Upon launching, the system drops you into a real-time terminal dashboard representing the current ocean network status, followed by an operational menu:

- `1`: Load sensor data from text file into the active queue.
- `2`: Dequeue packets to update the live regional pollution levels.
- `3`: Sort and display the regions ranked by contamination severity.
- `4`: Forecast the spread of pollution from a specific region using BFS.
- `5`: Calculate the optimal intervention route using Dijkstra's shortest path algorithm.
- `6`: Generate and save a permanent text report.
- `0`: Safely terminate the application.
