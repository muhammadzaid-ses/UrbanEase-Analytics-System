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

Person::Person(string n, int i) : name(n), id(i) {}
Person::~Person() {}
void Person::display() const {
     cout << "ID: " << id << " | Name: " << name;
}
string Person::getName() const {
     return name; 
}
int Person::getId() const {
     return id; 
}
void Person::setName(const string& n) {
     name = n;
}
void Person::setId(int i) {
     id = i; 
}
ostream& operator<<(ostream& os, const Person& p) {
    os << p.id << " " << p.name;
    return os;
}

istream& operator>>(istream& is, Person& p) {
    is >> p.id >> p.name;
    return is;
}

Employee::Employee(string n, int i, double s) : Person(n, i), salary(s) {}
void Employee::display() const {
    Person::display();
    cout << " | Salary: " << salary;
}
double Employee::getSalary() const { return salary; }
void Employee::setSalary(double s) { salary = s; }

ostream& operator<<(ostream& os, const Employee& e) {
    os << static_cast<const Person&>(e) << " " << e.salary;
    return os;
}

istream& operator>>(istream& is, Employee& e) {
    is >> static_cast<Person&>(e) >> e.salary;
    return is;
}

Manager::Manager(string n, int i, double s, string d)
    : Employee(n, i, s), department(d) {}

void Manager::display() const {
    Employee::display();
    cout << " | Dept: " << department;
}

string Manager::getDepartment() const { 
    return department;
}
void Manager::setDepartment(const string& d) { 
    department = d; 
}
ostream& operator<<(ostream& os, const Manager& m) {
    os << static_cast<const Employee&>(m) << " " << m.department;
    return os;
}

istream& operator>>(istream& is, Manager& m) {
    is >> static_cast<Employee&>(m) >> m.department;
    return is;
}

Location::Location(double lat, double lon) : latitude(lat), longitude(lon) {}
double Location::getLat() const {
     return latitude;
}
double Location::getLon() const {
     return longitude; 
}
void Location::setLat(double lat) { 
    latitude = lat;
}
void Location::setLon(double lon) {
     longitude = lon; 
}

Analytics::Analytics() : compositeScore(0) {
    for (int i = 0; i < 24; i++) {
        sales[i] = 0;
        costs[i] = 0;
        customers[i] = 0;
    }
}

double& Analytics::operator[](int i) { 
    return sales[i]; 
}
double Analytics::getMonthlySales(int i) const {
     return sales[i]; 
}
double Analytics::getMonthlyCost(int i) const { 
    return costs[i]; 
}
int Analytics::getMonthlyCustomers(int i) const {
     return customers[i]; 
}
void Analytics::setSales(int i, double v) { 
    sales[i] = v; 
}
void Analytics::setCost(int i, double v) { 
    costs[i] = v; 
}
void Analytics::setCustomers(int i, int v) { 
    customers[i] = v; 
}

void Analytics::calculateScore() {
    double totalSales = 0, totalCosts = 0;
    double growthSum = 0;
    for (int i = 0; i < 24; i++) {
        totalSales += sales[i];
        totalCosts += costs[i];
    }
    for (int i = 13; i < 24; i++)
        growthSum += (sales[i] - sales[i - 1]);

    double avgGrowth = growthSum / 11.0;
    double profit = totalSales - totalCosts;
    compositeScore = (profit * 0.50) + (totalSales * 0.30) + (avgGrowth * 0.20);
}

double Analytics::getTotalProfit() const {
    double total = 0;
    for (int i = 0; i < 24; i++) total += (sales[i] - costs[i]);
    return total;
}

double Analytics::getGrowthRate() const {
    double firstHalf = 0, secondHalf = 0;
    for (int i = 0; i < 12; i++) {
        firstHalf += sales[i];
        secondHalf += sales[i + 12];
    }
    if (firstHalf == 0) return 0;
    return ((secondHalf - firstHalf) / firstHalf) * 100;
}

double Analytics::getScore() const {
     return compositeScore; 
}

bool Analytics::operator>(const Analytics& o) const {
     return compositeScore > o.compositeScore; 
}
bool Analytics::operator<(const Analytics& o) const {
     return compositeScore < o.compositeScore; 
}

Analytics Analytics::operator+(const Analytics& o) const {
    Analytics result;
    for (int i = 0; i < 24; i++) {
        result.sales[i] = sales[i] + o.sales[i];
        result.costs[i] = costs[i] + o.costs[i];
        result.customers[i] = customers[i] + o.customers[i];
    }
    result.calculateScore();
    return result;
}

Analytics& Analytics::operator++() {
    for (int i = 0; i < 24; i++) sales[i] *= 1.05;
    calculateScore();
    return *this;
}

Analytics Analytics::operator++(int) {
    Analytics temp = *this;
    ++(*this);
    return temp;
}

Analytics& Analytics::operator--() {
    for (int i = 0; i < 24; i++) sales[i] *= 0.95;
    calculateScore();
    return *this;
}

Analytics Analytics::operator--(int) {
    Analytics temp = *this;
    --(*this);
    return temp;
}

Forecast::Forecast() : expectedSales(0), confidenceLevel(0), warningFlag(false), projectionMonth(25) {}

Forecast Forecast::operator+(const Forecast& o) const {
    Forecast f;
    f.expectedSales = expectedSales + o.expectedSales;
    f.confidenceLevel = (confidenceLevel + o.confidenceLevel) / 2.0;
    f.warningFlag = warningFlag || o.warningFlag;
    f.projectionMonth = projectionMonth;
    return f;
}

Forecast Forecast::operator++(int) {
    Forecast temp = *this;
    projectionMonth++;
    expectedSales *= 1.02;
    return temp;
}

void Forecast::calculate(const double sales[], const double costs[]) {
    double total = 0;
    for (int i = 0; i < 24; i++) total += sales[i];
    double avg = total / 24.0;
    double recent = (sales[21] + sales[22] + sales[23]) / 3.0;
    expectedSales = (avg * 0.30) + (recent * 0.70);

    double variance = 0;
    for (int i = 21; i < 24; i++) variance += pow(sales[i] - recent, 2);
    variance /= 3.0;
    double stddev = sqrt(variance);
    double cv = (recent > 0) ? (stddev / recent) : 1.0;
    confidenceLevel = (cv < 0.05) ? 90.0 : (cv < 0.15) ? 75.0 : (cv < 0.30) ? 60.0 : 45.0;

    if (recent > avg) {
        confidenceLevel += 10.0;
        if (confidenceLevel > 95.0) confidenceLevel = 95.0;
    }
    warningFlag = (expectedSales < sales[23]);
}

ostream& operator<<(ostream& os, const Forecast& f) {
    os << "Projected (M" << f.projectionMonth << "): PKR " << f.expectedSales
       << " | Confidence: " << f.confidenceLevel << "%";
    if (f.warningFlag) os << " [!] WARNING: DECLINE EXPECTED";
    return os;
}

Store::Store() : storeID(0), staff(nullptr), staffCount(0) {}

Store::Store(const Store& other) {
    storeName = other.storeName;
    storeID = other.storeID;
    loc = other.loc;
    stats = other.stats;
    forecast = other.forecast;
    mgr = other.mgr;
    staffCount = other.staffCount;
    if (other.staff && staffCount > 0) {
        staff = new Employee[staffCount];
        for (int i = 0; i < staffCount; i++) staff[i] = other.staff[i];
    } else {
        staff = nullptr;
    }
}

Store::~Store() { delete[] staff; }

Store& Store::operator=(const Store& other) {
    if (this == &other) return *this;
    delete[] staff;
    storeName = other.storeName;
    storeID = other.storeID;
    loc = other.loc;
    stats = other.stats;
    forecast = other.forecast;
    mgr = other.mgr;
    staffCount = other.staffCount;
    if (staffCount > 0) {
        staff = new Employee[staffCount];
        for (int i = 0; i < staffCount; i++) staff[i] = other.staff[i];
    } else {
        staff = nullptr;
    }
    return *this;
}

bool Store::operator==(const Store& o) const {
     return storeID == o.storeID; 
}
bool Store::operator>(const Store& o) const {
     return stats > o.stats; 
    }
bool Store::operator<(const Store& o) const { 
    return stats < o.stats; 
}

ostream& operator<<(ostream& os, const Store& s) {
    os << "Store[" << s.storeID << "] " << s.storeName
       << " @ (" << s.loc.getLat() << ", " << s.loc.getLon() << ")";
    return os;
}

void Store::save(ofstream& out) const {
    out << storeID << "\n" << storeName << "\n"
        << loc.getLat() << " " << loc.getLon() << "\n";
    out << mgr << "\n";
    out << staffCount << "\n";
    for (int i = 0; i < staffCount; i++) out << staff[i] << "\n";
    for (int i = 0; i < 24; i++) out << stats.getMonthlySales(i) << " ";
    out << "\n";
    for (int i = 0; i < 24; i++) out << stats.getMonthlyCost(i) << " ";
    out << "\n";
    for (int i = 0; i < 24; i++) out << stats.getMonthlyCustomers(i) << " ";
    out << "\n";
}

void Store::load(ifstream& in) {
    double lt, ln;
    in >> storeID >> storeName >> lt >> ln;
    loc.setLat(lt);
    loc.setLon(ln);
    in >> mgr;
    in >> staffCount;
    delete[] staff;
    if (staffCount > 0) {
        staff = new Employee[staffCount];
        for (int i = 0; i < staffCount; i++) in >> staff[i];
    } else {
        staff = nullptr;
    }
    for (int i = 0; i < 24; i++) {
        double v;
        in >> v;
        stats.setSales(i, v);
    }
    for (int i = 0; i < 24; i++) {
        double v;
        in >> v;
        stats.setCost(i, v);
    }
    for (int i = 0; i < 24; i++) {
        int v;
        in >> v;
        stats.setCustomers(i, v);
    }
}

void Store::updatePerformance() {
     stats.calculateScore();
}

void Store::runForecast() {
    double sales[24], costs[24];
    for (int i = 0; i < 24; i++) {
        sales[i] = stats.getMonthlySales(i);
        costs[i] = stats.getMonthlyCost(i);
    }
    forecast.calculate(sales, costs);
}

void Store::allocateStaff(int count) {
    delete[] staff;
    staffCount = count;
    staff = new Employee[staffCount];
}

void Store::setStaffMember(int i, const Employee& e) {
    if (i >= 0 && i < staffCount) staff[i] = e;
}

Report::Report(const string& t, int cap) : count(0), capacity(cap), title(t) {
    stores = new Store*[capacity];
}

Report::~Report() { delete[] stores; }

void Report::addStore(Store* s) {
    if (count < capacity) stores[count++] = s;
}

void Report::sortByComposite() {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (*stores[j] < *stores[j + 1]) {
                Store* tmp = stores[j];
                stores[j] = stores[j + 1];
                stores[j + 1] = tmp;
            }
        }
    }
}

void Report::sortByProfit() {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            double profitJ = stores[j]->getAnalytics().getTotalProfit();
            double profitJ1 = stores[j + 1]->getAnalytics().getTotalProfit();
            if (profitJ < profitJ1) {
                Store* tmp = stores[j];
                stores[j] = stores[j + 1];
                stores[j + 1] = tmp;
            }
        }
    }
}

void Report::sortByGrowth() {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            double growthJ = stores[j]->getAnalytics().getGrowthRate();
            double growthJ1 = stores[j + 1]->getAnalytics().getGrowthRate();
            if (growthJ < growthJ1) {
                Store* tmp = stores[j];
                stores[j] = stores[j + 1];
                stores[j + 1] = tmp;
            }
        }
    }
}

ostream& operator<<(ostream& os, const Report& r) {
    os << "\n========== " << r.title << " ==========\n";
    int rank = 0;
    for (int i = 0; i < r.count && i < 20; i++) {
        double profit = r.stores[i]->getAnalytics().getTotalProfit();
        double growth = r.stores[i]->getAnalytics().getGrowthRate();
        os << ++rank << ". " << *r.stores[i]
           << " | Score: " << r.stores[i]->getPerformanceScore()
           << " | Profit: PKR " << (long long)profit
           << " | Growth: " << growth << "%\n";
    }
    os << "=========================================\n";
    return os;
}

VisualReport::VisualReport(Store** s, int n, int width)
    : count(n), consoleWidth(width) {
    stores = new Store*[n];
    for (int i = 0; i < n; i++) stores[i] = s[i];
}

VisualReport::~VisualReport() { delete[] stores; }

void VisualReport::renderSalesTrend(int idx) const {
    if (idx < 0 || idx >= count) return;
    Store* s = stores[idx];
    double maxSales = 1;
    for (int m = 0; m < 24; m++)
        if (s->getMonthlySales(m) > maxSales) maxSales = s->getMonthlySales(m);

    int barArea = consoleWidth - 12;
    cout << "\n=== Sales Trend: " << *s << " ===\n";
    for (int m = 0; m < 24; m++) {
        int barLen = (int)((s->getMonthlySales(m) / maxSales) * barArea);
        cout << "M" << (m < 9 ? "0" : "") << (m + 1) << " | ";
        for (int b = 0; b < barLen; b++) cout << "#";
        cout << " " << (int)s->getMonthlySales(m) << "\n";
    }
}

void VisualReport::renderComparative(int topN) const {
    if (topN > count) topN = count;
    double maxProfit = 1;
    for (int i = 0; i < topN; i++) {
        double total = stores[i]->getAnalytics().getTotalProfit();
        if (total > maxProfit) maxProfit = total;
    }
    int barArea = consoleWidth - 20;
    cout << "\n=== Comparative Store Performance (Top " << topN << ") ===\n";
    for (int i = 0; i < topN; i++) {
        double total = stores[i]->getAnalytics().getTotalProfit();
        int barLen = (int)((total / maxProfit) * barArea);
        cout << "S" << stores[i]->getID() << " | ";
        for (int b = 0; b < barLen; b++) cout << "=";
        cout << " PKR " << (long long)total << "\n";
    }
}

void VisualReport::renderMenu() const {
    cout << "\n===== VISUALIZATION MENU =====\n";
    cout << "Available stores (Top " << count << "):\n";
    for (int i = 0; i < count && i < 10; i++) {
        cout << "  " << i << ". Store " << stores[i]->getID()
             << " - " << stores[i]->getName() << "\n";
    }
    cout << "Enter store index to view sales trend (-1 to skip): ";
    int choice;
    cin >> choice;
    if (choice >= 0 && choice < count) {
        renderSalesTrend(choice);
    }
    int compLimit = count;
    if (compLimit > 5) compLimit = 5;
    renderComparative(compLimit);
}

Cluster::Cluster() : memberCount(0), capacity(1000), clusterID(0) {
    memberStores = new Store*[capacity];
}

Cluster::~Cluster() {
    delete[] memberStores;
}

void Cluster::setCentroid(double lat, double lon) { centroid.set(lat, lon); }

double Cluster::getTotalRevenue() const {
    double total = 0;
    for (int i = 0; i < memberCount; i++)
        for (int m = 0; m < 24; m++)
            total += memberStores[i]->getMonthlySales(m);
    return total;
}

double Cluster::getTotalProfit() const {
    double total = 0;
    for (int i = 0; i < memberCount; i++)
        total += memberStores[i]->getAnalytics().getTotalProfit();
    return total;
}

Cluster Cluster::operator+(const Cluster& other) const {
    Cluster merged;
    merged.setCentroid(
        (centroid.getLat() * memberCount + other.centroid.getLat() * other.memberCount) /
            (memberCount + other.memberCount),
        (centroid.getLon() * memberCount + other.centroid.getLon() * other.memberCount) /
            (memberCount + other.memberCount));
    for (int i = 0; i < memberCount; i++) merged.addStore(memberStores[i]);
    for (int i = 0; i < other.memberCount; i++) merged.addStore(other.memberStores[i]);
    return merged;
}

ostream& operator<<(ostream& os, const Cluster& c) {
    os << "--- Cluster " << c.clusterID << " ---\n"
       << "  Centroid : (" << c.centroid.getLat() << ", " << c.centroid.getLon() << ")\n"
       << "  Stores   : " << c.memberCount << "\n"
       << "  Revenue  : PKR " << (long long)c.getTotalRevenue() << "\n"
       << "  Profit   : PKR " << (long long)c.getTotalProfit() << "\n";
    return os;
}

void Cluster::addStore(Store* s) {
    if (memberCount < capacity) memberStores[memberCount++] = s;
}

void Cluster::clearMembers() { memberCount = 0; }

double Cluster::calculateDistance(double lat1, double lon1, double lat2, double lon2) {
    return sqrt(pow(lat2 - lat1, 2) + pow(lon2 - lon1, 2));
}

void Cluster::displayCluster() const { cout << *this; }

void Cluster::performProfitSubClustering(int subClusters) {
    if (memberCount == 0) return;
    cout << "\n--- Sub-clustering Cluster " << clusterID << " by Profitability ---\n";

    for (int i = 0; i < memberCount - 1; i++) {
        for (int j = 0; j < memberCount - i - 1; j++) {
            double profitJ = memberStores[j]->getAnalytics().getTotalProfit();
            double profitJ1 = memberStores[j + 1]->getAnalytics().getTotalProfit();
            if (profitJ < profitJ1) {
                Store* tmp = memberStores[j];
                memberStores[j] = memberStores[j + 1];
                memberStores[j + 1] = tmp;
            }
        }
    }

    int subSize = memberCount / subClusters;
    if (subSize < 1) subSize = 1;

    for (int s = 0; s < subClusters && s * subSize < memberCount; s++) {
        cout << "  Sub-Cluster " << s + 1 << " (High to Medium Profit):\n";
        int start = s * subSize;
        int end = start + subSize;
        if (end > memberCount) end = memberCount;
        for (int i = start; i < end; i++) {
            cout << "    - Store " << memberStores[i]->getID()
                 << " | Profit: PKR " << (long long)memberStores[i]->getAnalytics().getTotalProfit() << "\n";
        }
    }
}