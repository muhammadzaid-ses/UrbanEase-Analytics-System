#include "store.h"
#include "task0.h"
#include "Cluster.h"
#include "Report.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <string>

using namespace std;

class DataTables {
public:
    static const int R_LIMIT = 32767;
    static const char* CITY_NAMES[15];
    static const char* AREA_NAMES[15];
    static const char* DEPTS[6];
    static const char* FIRST_NAMES[15];
    static const char* LAST_NAMES[12];
};

const char* DataTables::CITY_NAMES[15] = {
    "Karachi","Lahore","Islamabad","Faisalabad","Rawalpindi",
    "Multan","Peshawar","Quetta","Hyderabad","Sialkot",
    "Gujranwala","Sargodha","Bahawalpur","Sukkur","Larkana"
};

const char* DataTables::AREA_NAMES[15] = {
    "Central","North","South","East","West","Plaza","Mall",
    "Metro","City","Prime","Elite","Grand","Royal","Urban","Market"
};

const char* DataTables::DEPTS[6] = {
    "Operations","Sales","Logistics","Finance","HR","Marketing"
};

const char* DataTables::FIRST_NAMES[15] = {
    "Ali","Ahmed","Fatima","Ayesha","Usman","Zara","Hassan","Sara",
    "Omar","Hina","Bilal","Sana","Tariq","Nadia","Imran"
};

const char* DataTables::LAST_NAMES[12] = {
    "Khan","Malik","Raza","Sheikh","Qureshi","Siddiqui",
    "Akhtar","Baig","Chaudhry","Hussain","Mirza","Rehman"
};

double randDouble(double lo, double hi) {
    double r = (double)(rand() % (DataTables::R_LIMIT + 1)) / DataTables::R_LIMIT;
    return lo + r * (hi - lo);
}

int randInt(int lo, int hi) {
    return lo + rand() % (hi - lo + 1);
}

string randomName() {
    string f = DataTables::FIRST_NAMES[rand() % 15];
    string l = DataTables::LAST_NAMES[rand() % 12];
    return f + "_" + l;
}

string randomStoreName() {
    string p = "UrbanEase_";
    string c = DataTables::CITY_NAMES[rand() % 15];
    string a = DataTables::AREA_NAMES[rand() % 15];
    return p + c + "_" + a;
}

void generateStore(Store& s, int id) {
    s.setID(id);
    s.setName(randomStoreName());

    s.setLocation(randDouble(23.5, 37.0), randDouble(61.0, 77.0));

    Manager mgr(
        randomName(),
        id * 10 + 1,
        randDouble(80000, 250000),
        DataTables::DEPTS[rand() % 6]
    );

    s.setManager(mgr);

    int staffN = randInt(3, 8);
    s.allocateStaff(staffN);

    for (int j = 0; j < staffN; j++) {
        Employee emp(
            randomName(),
            id * 100 + j,
            randDouble(30000, 80000)
        );
        s.setStaffMember(j, emp);
    }

    double baseSales = randDouble(200000, 2000000);
    double trend = randDouble(-0.01, 0.03);

    for (int m = 0; m < 24; m++) {
        double seasonal = 1.0;
        int mIdx = m % 12;

        if (mIdx == 3 || mIdx == 4) seasonal = 1.4;
        else if (mIdx == 11) seasonal = 1.2;
        else if (mIdx == 6) seasonal = 0.8;

        double noise = randDouble(0.90, 1.10);

        double sale = baseSales *
                      pow(1 + trend, m) *
                      seasonal *
                      noise;

        s.setSales(m, sale);
        s.setCost(m, sale * randDouble(0.40, 0.65));
        s.setCustomers(m, (int)(sale / randDouble(500, 2000)));
    }
}

void performClustering(Store* allStores, int totalStores, Cluster* clusters, int k) {
    for (int i = 0; i < k; i++) {
        clusters[i].setID(i + 1);
        clusters[i].setCentroid(
            allStores[i].getLat(),
            allStores[i].getLon()
        );
    }

    Centroid prevCentroids[10];

    for (int iter = 0; iter < 20; iter++) {
        for (int j = 0; j < k; j++) {
            prevCentroids[j] = clusters[j].getCentroid();
        }

        for (int j = 0; j < k; j++) {
            clusters[j].clearMembers();
        }

        for (int i = 0; i < totalStores; i++) {
            double minDist = 1e18;
            int best = 0;

            for (int j = 0; j < k; j++) {
                double d = Cluster::calculateDistance(
                    allStores[i].getLat(),
                    allStores[i].getLon(),
                    clusters[j].getCentroidLat(),
                    clusters[j].getCentroidLon()
                );

                if (d < minDist) {
                    minDist = d;
                    best = j;
                }
            }

            clusters[best].addStore(&allStores[i]);
        }

        bool converged = true;

        for (int j = 0; j < k; j++) {
            int members = clusters[j].getMemberCount();

            if (members > 0) {
                double sumLat = 0, sumLon = 0;

                for (int m = 0; m < members; m++) {
                    sumLat += clusters[j].getMember(m)->getLat();
                    sumLon += clusters[j].getMember(m)->getLon();
                }

                Centroid newC(sumLat / members, sumLon / members);

                if (newC != prevCentroids[j]) {
                    converged = false;
                }

                clusters[j].setCentroid(newC.getLat(), newC.getLon());
            }
        }

        if (converged) break;
    }
}

void showPersonDetails(Person* p) {
    p->display();
    cout << "\n";
}

Store* startupMenu(int& count) {
    int choice;

    cout << "\n=== UrbanEase Analytics System ===\n";
    cout << "1. Load existing data from file\n";
    cout << "2. Generate new dataset\n";
    cout << "Choice: ";
    cin >> choice;

    Store* dataset = nullptr;

    if (choice == 1) {
        ifstream inFile("data.txt");

        if (!inFile) {
            cout << "[!] File 'data.txt' not found. Switching to generation mode.\n";
            choice = 2;
        } else {
            inFile >> count;
            dataset = new Store[count];

            for (int i = 0; i < count; i++) {
                dataset[i].load(inFile);
            }

            inFile.close();

            cout << "[OK] Loaded " << count << " stores from data.txt\n";
            return dataset;
        }
    }

    if (choice == 2) {
        do {
            cout << "Enter store count (min 100, recommended 500): ";
            cin >> count;

            if (count < 100) {
                cout << "[!] Minimum is 100. Try again.\n";
            }
        } while (count < 100);

        dataset = new Store[count];

        cout << "Generating " << count << " stores...\n";

        for (int i = 0; i < count; i++) {
            generateStore(dataset[i], i + 1);
        }

        ofstream outFile("data.txt");

        outFile << count << "\n";

        for (int i = 0; i < count; i++) {
            dataset[i].save(outFile);
        }

        outFile.close();

        cout << "[OK] Generated and saved " << count << " stores to data.txt\n";
    }

    return dataset;
}

int main() {
    srand((unsigned)time(0));

    int count = 0;

    Store* dataset = startupMenu(count);

    if (!dataset) {
        cout << "Failed to initialize dataset.\n";
        return 1;
    }

    cout << "\nCalculating performance scores...\n";

    for (int i = 0; i < count; i++) {
        dataset[i].updatePerformance();
    }

    cout << "Running sales forecasts...\n";

    for (int i = 0; i < count; i++) {
        dataset[i].runForecast();
    }

    int K = 5;

    Cluster* clusters = new Cluster[K];

    performClustering(dataset, count, clusters, K);

    cout << "\n--- Geographic Clusters ---\n";

    for (int i = 0; i < K; i++) {
        cout << clusters[i];
    }

    cout << "\n--- Two-Tier Sub-Clustering (Profitability) ---\n";

    for (int i = 0; i < K; i++) {
        clusters[i].performProfitSubClustering(3);
    }

    Report fullReport("Store Performance Rankings", count);

    for (int i = 0; i < count; i++) {
        fullReport.addStore(&dataset[i]);
    }

    int sortChoice;

    cout << "\n========== RANKING MENU ==========\n";
    cout << "1. Sort by Composite Score\n";
    cout << "2. Sort by Total Profit\n";
    cout << "3. Sort by Growth Rate\n";
    cout << "Enter your choice (1-3): ";
    cin >> sortChoice;

    switch (sortChoice) {
    case 1:
        fullReport.sortByComposite();
        break;
    case 2:
        fullReport.sortByProfit();
        break;
    case 3:
        fullReport.sortByGrowth();
        break;
    default:
        fullReport.sortByComposite();
    }

    cout << fullReport;

    cout << "\n--- 3-Month Forecast Demo (Store #1) ---\n";

    Forecast f = dataset[0].getForecast();

    for (int month = 0; month < 3; month++) {
        cout << f++ << "\n";
    }

    cout << "\n--- Polymorphism Demo ---\n";

    Person* p_demo = new Employee("Demo_Employee", 9001, 55000.0);
    Person* m_demo = new Manager("Demo_Manager", 9002, 150000.0, "Operations");

    showPersonDetails(p_demo);
    showPersonDetails(m_demo);

    delete p_demo;
    delete m_demo;

    cout << "\n--- ASCII Sales Visualization ---\n";

    int visCount = (count < 10) ? count : 10;

    Store** topStores = new Store*[visCount];

    for (int i = 0; i < visCount; i++) {
        topStores[i] = &dataset[i];
    }

    VisualReport vr(topStores, visCount, 60);
    vr.renderMenu();

    delete[] topStores;
    delete[] clusters;
    delete[] dataset;

    cout << "\n[Done] UrbanEase Analytics complete.\n";

    return 0;
}