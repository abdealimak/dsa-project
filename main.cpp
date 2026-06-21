#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <climits>
#include <algorithm>
#include <fstream> // Required for reading input and writing reports

using namespace std;

// ==========================================
// 1. DATA MODELS
// ==========================================

struct SensorReading {
    int id;             // Unique code for tracking transmissions
    int regionId;       // The target area ID
    int pollutionLevel; // Amount of toxic material detected
};

struct Region {
    int id;
    string name;
    int pollutionLevel; // Cumulative score of how polluted this region is
};

// ==========================================
// 2. MAIN SYSTEM CLASS
// ==========================================
class OceanSystem {
private:
    vector<Region> regions;                  // Index matches Region ID directly (O(1) lookups)
    vector<vector<pair<int, int>>> adjList;  // Adjacency List: adjList[u] = {neighbor, time}
    queue<SensorReading> sensorQueue;        // The core FIFO Queue for stream processing

    // --- CUSTOM MERGE SORT FOR RANKING ---
    void merge(vector<Region>& arr, int left, int mid, int right) {
        int n1 = mid - left + 1;
        int n2 = right - mid;
        vector<Region> L(n1), R(n2);

        for (int i = 0; i < n1; i++) L[i] = arr[left + i];
        for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];

        int i = 0, j = 0, k = left;
        while (i < n1 && j < n2) {
            if (L[i].pollutionLevel >= R[j].pollutionLevel) { // Descending order
                arr[k++] = L[i++];
            } else {
                arr[k++] = R[j++];
            }
        }
        while (i < n1) arr[k++] = L[i++];
        while (j < n2) arr[k++] = R[j++];
    }

    void mergeSort(vector<Region>& arr, int left, int right) {
        if (left >= right) return;
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }

public:
    OceanSystem(int maxRegions = 10) {
        regions.resize(maxRegions);
        adjList.resize(maxRegions);
    }

    void addRegion(int id, string name) {
        if (id >= (int)regions.size()) return;
        regions[id] = {id, name, 0};
    }

    void addConnection(int u, int v, int travelTime) {
        if (u >= (int)adjList.size() || v >= (int)adjList.size()) return;
        adjList[u].push_back({v, travelTime});
        adjList[v].push_back({u, travelTime}); // Bidirectional maritime lanes
    }

    // --- VISUALIZER DASHBOARD ---
    // This function draws the current status of the entire ocean map into the terminal console
    void displayDashboard() {
        cout << "\n=================================================================\n";
        cout << "               LIVE OCEAN GRAPH MONITORING DASHBOARD             \n";
        cout << "=================================================================\n";
        
        // 1. Render the Visual Pollution Status Bars
        cout << "REGIONAL CONTAMINATION LEVELS:\n";
        for (size_t i = 1; i < regions.size(); i++) {
            if (!regions[i].name.empty()) {
                // Format the name nicely so columns line up
                string paddedName = regions[i].name;
                while (paddedName.length() < 22) paddedName += " ";

                cout << "ID [" << regions[i].id << "] " << paddedName << " : ";
                
                // Generate a status bar. 1 '#' character for every 10 units of pollution.
                int bars = regions[i].pollutionLevel / 10;
                if (bars > 15) bars = 15; // Cap visual length so it doesn't break rows
                
                cout << "[";
                for (int b = 0; b < bars; b++) cout << "#";
                for (int b = 0; b < (15 - bars); b++) cout << "-";
                cout << "] (" << regions[i].pollutionLevel << " Units)\n";
            }
        }
        cout << "-----------------------------------------------------------------\n";

        // 2. Render the Live Queue Pipe Line
        cout << "LIVE INGESTION QUEUE STREAM:\n";
        if (sensorQueue.empty()) {
            cout << " [Empty] - No pending telemetry packets.\n";
        } else {
            cout << " [Front] -> ";
            // Create a temporary copy of the queue to print its contents safely without erasing data
            queue<SensorReading> tempQueue = sensorQueue;
            int displayLimit = 0;
            while (!tempQueue.empty() && displayLimit < 4) {
                cout << "Packet#" << tempQueue.front().id << " (Reg " << tempQueue.front().regionId << ") -> ";
                tempQueue.pop();
                displayLimit++;
            }
            if (!tempQueue.empty()) cout << "[" << tempQueue.size() << " more] -> ";
            cout << "[Back]\n";
        }
        cout << "=================================================================\n";
    }

    // --- REQUIREMENT 1: FILE-BASED QUEUE INGESTION ---
    void loadSensorDataFromFile(string filename) {
        ifstream inputFile(filename);
        if (!inputFile.is_open()) {
            cout << "\n[Error] Could not find '" << filename << "'. Make sure it's in the same folder!\n";
            return;
        }

        int rId, pLevel;
        int autoAlertId = 7001; // Unique identification tracking serial
        int count = 0;

        while (inputFile >> rId >> pLevel) {
            if (rId <= 0 || rId >= (int)regions.size() || regions[rId].name.empty()) continue;

            // Push elements directly to the back of our line pipeline
            sensorQueue.push({autoAlertId++, rId, pLevel});
            count++;
        }
        inputFile.close();
        cout << "\n[Success] Ingested " << count << " telemetry packets into the FIFO Queue.\n";
    }

    // --- REQUIREMENT 1 (CONT.): PROCESSING SENSOR DATA FROM QUEUE ---
    void processSensorQueue() {
        if (sensorQueue.empty()) {
            cout << "\n[Status] Processing Aborted: The queue backlog is clear.\n"; return;
        }

        cout << "\n--- Emptying Queue Pipeline (First-In, First-Out) ---\n";
        while (!sensorQueue.empty()) {
            SensorReading reading = sensorQueue.front(); // Capture front element
            sensorQueue.pop();                           // Erase front element from queue

            regions[reading.regionId].pollutionLevel += reading.pollutionLevel; // Accumulate metric
            cout << " -> Dispatched Packet #" << reading.id << " to " << regions[reading.regionId].name << "\n";
        }
    }

    // --- REQUIREMENT 2: MERGE SORT RANKING ---
    void getRankedPollutedRegions() {
        cout << "\n--- Priority Sorting Assessment (Merge Sort) ---\n";
        vector<Region> activeRegions;

        for (size_t i = 1; i < regions.size(); i++) {
            if (!regions[i].name.empty()) activeRegions.push_back(regions[i]);
        }

        if (!activeRegions.empty()) {
            mergeSort(activeRegions, 0, activeRegions.size() - 1);
        }

        cout << "Ranked Impact Hierarchy:\n";
        for (size_t i = 0; i < activeRegions.size(); i++) {
            cout << " " << i + 1 << ". " << activeRegions[i].name << " (Level: " << activeRegions[i].pollutionLevel << ")\n";
        }
    }

    // --- REQUIREMENT 3: BFS POLLUTION SPREAD DETECTION ---
    void predictPollutionSpread(int startRegionId) {
        if (regions[startRegionId].name.empty()) {
            cout << "\n[Error] Invalid origin parameters.\n"; return;
        }

        cout << "\n--- Environmental Vector Expansion Forecasting (BFS) ---\n";
        vector<bool> visited(regions.size(), false);
        queue<pair<int, int>> bfsQueue; // Holds {regionId, distanceLayer}

        bfsQueue.push({startRegionId, 0});
        visited[startRegionId] = true;

        while (!bfsQueue.empty()) {
            int currentId = bfsQueue.front().first;
            int layer = bfsQueue.front().second;
            bfsQueue.pop();

            if (layer > 0) {
                cout << " [!] Spread Layer " << layer << " Alert: Contamination expanding to -> " << regions[currentId].name << "\n";
            }

            for (auto neighbor : adjList[currentId]) {
                int neighborId = neighbor.first;
                if (!visited[neighborId]) {
                    visited[neighborId] = true;
                    bfsQueue.push({neighborId, layer + 1});
                }
            }
        }
    }

    // --- REQUIREMENT 4: DIJKSTRA SHORTEST PATH ROUTING ---
    void dispatchCleanupVessel(int startId, int targetId) {
        if (regions[startId].name.empty() || regions[targetId].name.empty()) {
            cout << "\n[Error] Coordinate out of bounds.\n"; return;
        }

        cout << "\n--- Calculating Interconnected Travel Vectors (Dijkstra) ---\n";
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
        
        vector<int> distances(regions.size(), INT_MAX);
        vector<int> parent(regions.size(), -1);

        distances[startId] = 0;
        pq.push({0, startId});

        while (!pq.empty()) {
            int currentDist = pq.top().first;
            int currentId = pq.top().second;
            pq.pop();

            if (currentId == targetId) break;
            if (currentDist > distances[currentId]) continue;

            for (auto neighbor : adjList[currentId]) {
                int neighborId = neighbor.first;
                int weight = neighbor.second;

                if (distances[currentId] + weight < distances[neighborId]) {
                    distances[neighborId] = distances[currentId] + weight;
                    parent[neighborId] = currentId;
                    pq.push({distances[neighborId], neighborId});
                }
            }
        }

        if (distances[targetId] == INT_MAX) {
            cout << "[Critical Failure] Destined grid coordinates are structurally isolated.\n";
            return;
        }

        vector<int> route;
        for (int at = targetId; at != -1; at = parent[at]) route.push_back(at);
        reverse(route.begin(), route.end());

        cout << "Vessel Vector Confirmed. Transit Estimate: " << distances[targetId] << " hours.\n";
        cout << "Navigation Leg: ";
        for (size_t i = 0; i < route.size(); ++i) {
            cout << regions[route[i]].name;
            if (i < route.size() - 1) cout << " -> ";
        }
        cout << "\n";
    }

    // --- ENVIRONMENTAL REPORT GENERATION (.TXT) ---
    void generateEnvironmentalReport() {
        ofstream file("Ocean_System_Report.txt");
        if (!file.is_open()) {
            cout << "\n[Error] System file block failure.\n"; return;
        }

        file << "====================================================\n";
        file << "    ECO-LOGISTICS MARITIME INTELLIGENCE REPORT      \n";
        file << "====================================================\n\n";
        file << "Active Incidents Awaiting Processing Queue: " << sensorQueue.size() << "\n\n";
        file << "CURRENT POLLUTION METRICS BY REGION:\n";
        file << "----------------------------------------------------\n";

        for (size_t i = 1; i < regions.size(); i++) {
            if (!regions[i].name.empty()) {
                file << "Region ID [" << regions[i].id << "] -> Name: " << regions[i].name 
                     << " | Total Contamination Load: " << regions[i].pollutionLevel << " units\n";
            }
        }
        file << "\n================= END OF REPORT ====================\n";
        file.close();
        cout << "\n[Report] File compiled. Written safely to 'Ocean_System_Report.txt'\n";
    }
};

// ==========================================
// 3. MENU DRIVEN CONTROLLER
// ==========================================
int main() {
    OceanSystem system(10);

    // Bootstrap basic node coordinates
    system.addRegion(1, "North Port (Base)");
    system.addRegion(2, "Coral Reef Matrix");
    system.addRegion(3, "Deep Oceanic Trench");
    system.addRegion(4, "Eastern Trade Gulf");
    system.addRegion(5, "Industrial Delta");

    // Establish link metrics
    system.addConnection(1, 2, 4);
    system.addConnection(1, 4, 10);
    system.addConnection(2, 3, 5);
    system.addConnection(2, 4, 2);
    system.addConnection(4, 5, 3);
    system.addConnection(3, 5, 8);

    int actionSelection;

    while (true) {
        // ALWAYS print out the visual map dashboard at the top of the menu loop!
        system.displayDashboard();

        cout << "\nAvailable Operational Directives:\n";
        cout << " 1. Auto-Load Telemetry File (Sensor_Inputs.txt)\n";
        cout << " 2. Dequeue & Process Alerts\n";
        cout << " 3. Evaluate Pollution Hierarchy (Merge Sort)\n";
        cout << " 4. Forecast Vector Expansion Spills (BFS)\n";
        cout << " 5. Route Vessel Trajectory Path (Dijkstra)\n";
        cout << " 6. Print Permanent Audit Record (.txt File)\n";
        cout << " 0. Kill Operational Subsystem Core\n";
        cout << "Execute Selection Index: ";

        cin >> actionSelection;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Input error. Enter an integer identifier index.\n";
            continue;
        }

        if (actionSelection == 0) {
            cout << "System Core Disengaged.\n"; break;
        }

        switch (actionSelection) {
            case 1:
                system.loadSensorDataFromFile("Sensor_Inputs.txt");
                break;
            case 2:
                system.processSensorQueue();
                break;
            case 3:
                system.getRankedPollutedRegions();
                break;
            case 4: {
                int rSelection;
                cout << "Identify source spill incident point node (1-5): ";
                cin >> rSelection;
                system.predictPollutionSpread(rSelection);
                break;
            }
            case 5: {
                int targetSelection;
                cout << "Identify critical intervention site target (1-5): ";
                cin >> targetSelection;
                system.dispatchCleanupVessel(1, targetSelection);
                break;
            }
            case 6:
                system.generateEnvironmentalReport();
                break;
            default:
                cout << "Selection criteria unmatched. Re-enter command instruction.\n";
        }
        
        // Minor addition to pause the terminal screen so you can read algorithm details 
        // before the screen updates again.
        cout << "\nPress Enter to return to main dashboard menu...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get(); 
    }
    return 0;
}