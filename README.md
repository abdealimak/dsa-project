# Ocean Environmental Monitoring System

The **Ocean Environmental Monitoring System** is a sophisticated C++ application designed to simulate a maritime logistics and ecological monitoring dashboard. It tracks pollution incidents across interconnected oceanic regions, forecasts contamination spread, and plans efficient cleanup operations. The system extensively leverages core Data Structures and Algorithms (DSA) to model a network of ocean regions and calculate optimal responses to environmental emergencies.

## Core Features and Algorithms

This system relies on a graph-based simulation model of the ocean and employs several classical algorithms:

### 1. Live Data Ingestion (FIFO Queue)
The system simulates a continuous stream of telemetry packets coming from oceanic sensors. Alerts are read from a local data file (`Sensor_Inputs.txt`) and enqueued into a First-In, First-Out (FIFO) `std::queue`. The alerts are then processed sequentially to update the regional contamination levels. This ensures that the oldest incidents are responded to first, simulating real-world stream processing.

### 2. Pollution Hierarchy Ranking (Merge Sort)
As pollution metrics are accumulated, the system employs a custom implementation of **Merge Sort** to quickly order the regions by contamination level in descending order. Merge Sort is chosen for its `O(N log N)` time complexity and stable sorting properties, making it highly efficient for generating a ranked priority list to help authorities prioritize emergency responses.

### 3. Contamination Spread Forecasting (Breadth-First Search)
Using **Breadth-First Search (BFS)**, the system predicts how a spill will expand outward through the interconnected maritime network layer by layer. By treating regions as nodes and maritime routes as edges in an Adjacency List graph representation, BFS allows authorities to anticipate the exact sequence of regions that will be affected by a contamination source.

### 4. Cleanup Vessel Routing (Dijkstra's Algorithm)
When a severe incident occurs requiring physical intervention, **Dijkstra's Algorithm** is used to calculate the shortest travel time and exact maritime navigation path for a cleanup vessel. The vessel is dispatched from the base (Region 1) to the target coordinates. A Min-Priority Queue is used to efficiently resolve the optimal pathing on a weighted graph, determining the most rapid trajectory possible.

### 5. Environmental Audit Reporting
The system compiles all active metrics and incidents into a structured audit file (`Ocean_System_Report.txt`) for persistent record-keeping. The state of the queue, and the total contamination per node, are cleanly serialized to disk using C++ standard file streams.

## Project Structure

- `main.cpp` - The core application logic containing the graph structure, algorithms, and the interactive CLI dashboard.
- `Sensor_Inputs.txt` - Sample telemetry data ingested by the system queue. Each line represents an incoming telemetry packet in the format: `RegionID PollutionLevel`.
- `Ocean_System_Report.txt` - Generated audit log containing the current state of the oceanic grid. Created dynamically during execution.
- `.gitignore` - Standard configuration to avoid tracking local C++ executables (e.g., `main`).

## Getting Started

### Prerequisites

- A standard C++11 (or higher) compiler, such as `g++` or `clang++`.
- A terminal environment capable of running standard shell commands.

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

Upon launching, the system drops you into a real-time terminal dashboard representing the current ocean network status visually. The dashboard automatically updates as data is processed. Below the dashboard is the operational menu:

- `1`: **Auto-Load Telemetry File** - Loads sensor data from `Sensor_Inputs.txt` into the active queue.
- `2`: **Dequeue & Process Alerts** - Processes the queued packets, adding pollution values to the respective regions.
- `3`: **Evaluate Pollution Hierarchy** - Sorts and displays the regions ranked by contamination severity using Merge Sort.
- `4`: **Forecast Vector Expansion Spills** - Prompts for a region ID and uses BFS to forecast the spread of pollution.
- `5`: **Route Vessel Trajectory Path** - Prompts for a target region and calculates the optimal intervention route using Dijkstra's shortest path algorithm.
- `6`: **Print Permanent Audit Record** - Generates and saves a permanent text report (`Ocean_System_Report.txt`).
- `0`: **Kill Operational Subsystem Core** - Safely terminates the application.
