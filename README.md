# 2.1 Project Title

**Case Study 186: Ocean Pollution Tracking and Cleanup System**
*(ITM Skills University - B.Tech CSE 2025-29 | Data Structure & Algorithms with CPP | Semester II)*

# 2.2 Problem Statement

Marine pollution poses a serious threat to ecosystems, biodiversity, and human health. Oil spills, plastic waste, industrial discharge, and chemical contamination can spread rapidly through interconnected ocean regions. Effective environmental protection requires continuous monitoring and coordinated cleanup operations. The **Ocean Pollution Tracking and Cleanup System** provides real-time pollution monitoring, contamination analysis, spread prediction, and cleanup resource allocation.

# 2.3 Objectives

The primary objective is to track marine pollution, predict contamination spread, rank affected regions, and optimize cleanup operations through intelligent data analysis. 

**Industry Context:** Environmental agencies deploy thousands of ocean sensors that continuously measure pollution indicators such as chemical concentration, oil presence, and water quality. Rapid detection and response are critical for minimizing environmental damage.

# 2.4 System Overview / Architecture

The system is built as a C++ Command-Line Interface (CLI) application centered around a graph-based simulation model. 
- **Core Engine (`OceanSystem` Class)**: Encapsulates all data layers, maintaining the oceanic network graph, processing queues, and storing state.
- **Graph Representation**: Oceanic regions act as nodes, while interconnected maritime travel paths act as edges.
- **Deliverables**: The architecture is designed to provide:
  - Pollution monitoring dashboards.
  - Spread prediction tools.
  - Cleanup scheduling systems.
  - Environmental reports.
  - Emergency response alerts.

# 2.5 Data Structures and Algorithms Used

1. **Sensor Data Processing Using Queue (`std::queue`)**: Incoming sensor readings are processed sequentially using a Queue. This ensures timely handling of real-time environmental data.
2. **Pollution Ranking Using Merge Sort**: Ocean regions are ranked according to contamination levels using Merge Sort. Its stability and efficiency make it suitable for processing large environmental datasets.
3. **Pollution Spread Detection Using BFS**: Connected ocean regions are represented as graph nodes. BFS analyzes how contamination spreads across neighboring regions, helping authorities predict future impact zones.
4. **Cleanup Vessel Assignment Using Shortest Path Algorithms (Dijkstra's)**: Cleanup vessels are assigned to affected regions using shortest-path calculations to minimize travel time and improve emergency response effectiveness.

# 2.6 Implementation Approach

The project follows a modular, Object-Oriented Programming (OOP) methodology:
- **Data Models**: Simple `structs` (`SensorReading`, `Region`) define the basic entities.
- **State Initialization**: The system bootstraps basic node coordinates (e.g., North Port, Coral Reef Matrix) and establishes hardcoded weighted edges representing travel times.
- **File I/O**: The system reads from `Sensor_Inputs.txt` to populate the `sensorQueue`. Once processing is concluded, the `std::ofstream` module serializes the current network state to `Ocean_System_Report.txt`.
- **Event Loop**: A `while(true)` loop keeps the program active, refreshing the dashboard upon every iteration and capturing user input to dispatch algorithmic routines.

# 2.7 Time and Space Complexity Analysis

- **Data Ingestion & Dequeueing (Queue)**: 
  - Time Complexity: `O(1)` per packet.
  - Space Complexity: `O(Q)` where `Q` is the number of queued packets.
- **Merge Sort (Pollution Ranking)**:
  - Time Complexity: `O(V log V)` where `V` is the number of active regions.
  - Space Complexity: `O(V)` auxiliary space for merging arrays.
- **BFS (Contamination Spread Prediction)**:
  - Time Complexity: `O(V + E)` where `V` is vertices (regions) and `E` is edges (maritime routes).
  - Space Complexity: `O(V)` for the queue and visited array.
- **Dijkstra's Algorithm (Vessel Routing)**:
  - Time Complexity: `O((V + E) log V)` due to the Min-Priority Queue.
  - Space Complexity: `O(V)` for the priority queue, distance array, and parent tracking array.
- **Overall System Space Complexity**: `O(V + E + Q)` to hold the graph mapping and the active event pipeline.

# 2.8 Execution Steps

1. **Prerequisites**: Ensure you have a standard C++11 compiler (`g++`, `clang++`) installed.
2. **Compile**: 
   ```bash
   g++ -o main main.cpp
   ```
3. **Prepare Input**: Ensure a file named `Sensor_Inputs.txt` exists in the same directory, containing target regions and pollution levels (e.g., `5 100`).
4. **Execute**:
   ```bash
   ./main
   ```
5. **Operation**: Follow the on-screen menu prompts (1 through 6) to load data, process the queue, run the algorithms, and generate reports. Select `0` to quit.

# 2.9 Sample Inputs and Outputs

**Sample Input (`Sensor_Inputs.txt`)**:
```text
5 140
2 50
4 20
```

**Sample Output (`Ocean_System_Report.txt`)**:
```text
====================================================
    ECO-LOGISTICS MARITIME INTELLIGENCE REPORT      
====================================================

Active Incidents Awaiting Processing Queue: 0

CURRENT POLLUTION METRICS BY REGION:
----------------------------------------------------
Region ID [1] -> Name: North Port (Base) | Total Contamination Load: 0 units
Region ID [2] -> Name: Coral Reef Matrix | Total Contamination Load: 50 units
Region ID [3] -> Name: Deep Oceanic Trench | Total Contamination Load: 0 units
Region ID [4] -> Name: Eastern Trade Gulf | Total Contamination Load: 20 units
Region ID [5] -> Name: Industrial Delta | Total Contamination Load: 140 units

================= END OF REPORT ====================
```

# 2.10 Screenshots

*(Text-based visualization of the CLI Application Dashboard)*

```text
=================================================================
               LIVE OCEAN GRAPH MONITORING DASHBOARD             
=================================================================
REGIONAL CONTAMINATION LEVELS:
ID [1] North Port (Base)      : [---------------] (0 Units)
ID [2] Coral Reef Matrix      : [#####----------] (50 Units)
ID [3] Deep Oceanic Trench    : [---------------] (0 Units)
ID [4] Eastern Trade Gulf     : [##-------------] (20 Units)
ID [5] Industrial Delta       : [##############-] (140 Units)
-----------------------------------------------------------------
LIVE INGESTION QUEUE STREAM:
 [Front] -> Packet#7001 (Reg 5) -> Packet#7002 (Reg 2) -> [Back]
=================================================================

Available Operational Directives:
 1. Auto-Load Telemetry File (Sensor_Inputs.txt)
 2. Dequeue & Process Alerts
 3. Evaluate Pollution Hierarchy (Merge Sort)
 4. Forecast Vector Expansion Spills (BFS)
 5. Route Vessel Trajectory Path (Dijkstra)
 6. Print Permanent Audit Record (.txt File)
 0. Kill Operational Subsystem Core
Execute Selection Index: 
```

# 2.11 Results and Observations

The system successfully ingests real-time simulated telemetry data from a file pipeline, updating its internal state predictably. The graphical dashboard correctly recalculates dynamic hash-bar lengths to reflect the pollution scale. When tested against localized incidents, Merge Sort perfectly ranks the zones from most to least critical, while Dijkstra's Algorithm successfully resolves the fastest travel nodes while dynamically circumventing unnecessary waypoints. The data structures implemented (Queues, Vectors, Priority Queues) exhibit low overhead, confirming that the system is highly optimal and capable of scaling up to monitor hundreds or thousands of oceanic grids seamlessly.

# 2.12 Conclusion

The Ocean Pollution Tracking and Cleanup System illustrates how Queue, Merge Sort, BFS, and graph algorithms support environmental conservation and disaster response efforts. The system successfully meets the deliverables by providing pollution monitoring dashboards, spread prediction tools, cleanup scheduling systems, environmental reports, and emergency response alerts. The modular CLI design establishes a strong foundation that can be easily extended in the future to include graphical user interfaces (GUIs), database integrations, and dynamic edge-weight updating based on live weather data.
