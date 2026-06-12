#pragma once
#include "store.h"
#include <iostream>

class Report {
private:
    Store** stores;
    int count;
    int capacity;
    std::string title;

public:
    Report(const std::string& t = "Performance Report", int cap = 500);
    ~Report();

    void addStore(Store* s);

    void sortByComposite();
    void sortByProfit();
    void sortByGrowth();

    friend std::ostream& operator<<(std::ostream& os, const Report& r);

    int getCount() const { return count; }
    Store* getStore(int i) const { return stores[i]; }
};

class VisualReport {
private:
    Store** stores;
    int count;
    int consoleWidth;

public:
    VisualReport(Store** s, int n, int width = 60);
    ~VisualReport();

    void renderSalesTrend(int storeIndex) const;
    void renderComparative(int topN) const;
    void renderMenu() const;

    friend std::ostream& operator<<(std::ostream& os, const VisualReport& vr);
};