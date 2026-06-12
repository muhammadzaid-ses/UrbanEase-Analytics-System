#pragma once
#include "task0.h"
#include <fstream>
#include <string>

class Store {
private:
    std::string storeName;
    int storeID;
    Location loc;
    Analytics stats;
    Forecast forecast;
    Manager mgr;
    Employee* staff;
    int staffCount;

public:
    Store();
    Store(const Store& other);
    ~Store();

    Store& operator=(const Store& other);
    bool operator==(const Store& other) const;
    bool operator>(const Store& other) const;
    bool operator<(const Store& other) const;

    friend std::ostream& operator<<(std::ostream& os, const Store& s);
    friend std::istream& operator>>(std::istream& is, Store& s);

    void save(std::ofstream& out) const;
    void load(std::ifstream& in);

    int getID() const;
    std::string getName() const;
    double getLat() const;
    double getLon() const;
    double getPerformanceScore() const;
    const Forecast& getForecast() const;
    const Analytics& getAnalytics() const;

    void setName(const std::string& n);
    void setID(int id);
    void setLocation(double lat, double lon);

    void updatePerformance();
    void runForecast();

    void setSales(int i, double v);
    void setCost(int i, double v);
    void setCustomers(int i, int v);
    double getMonthlySales(int i) const;
    double getMonthlyCost(int i) const;

    void allocateStaff(int count);
    void setStaffMember(int i, const Employee& e);
    int getStaffCount() const;

    void setManager(const Manager& m);
    const Manager& getManager() const;
};