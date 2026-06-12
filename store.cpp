#include "store.h"

int Store::getID() const { 
    return storeID; 
}

std::string Store::getName() const { 
    return storeName; 
}

double Store::getLat() const { 
    return loc.getLat(); 
}

double Store::getLon() const { 
    return loc.getLon(); 
}

double Store::getPerformanceScore() const { 
    return stats.getScore(); 
}

const Forecast& Store::getForecast() const { 
    return forecast; 
}

const Analytics& Store::getAnalytics() const { 
    return stats; 
}

void Store::setName(const std::string& n) { 
    storeName = n; 
}

void Store::setID(int id) { 
    storeID = id; 
}

void Store::setLocation(double lat, double lon) { 
    loc.setLat(lat); 
    loc.setLon(lon); 
}

void Store::setSales(int i, double v) { 
    stats.setSales(i, v); 
}

void Store::setCost(int i, double v) { 
    stats.setCost(i, v); 
}

void Store::setCustomers(int i, int v) { 
    stats.setCustomers(i, v); 
}

double Store::getMonthlySales(int i) const { 
    return stats.getMonthlySales(i); 
}

double Store::getMonthlyCost(int i) const { 
    return stats.getMonthlyCost(i); 
}

int Store::getStaffCount() const { 
    return staffCount; 
}

void Store::setManager(const Manager& m) { 
    mgr = m; 
}

const Manager& Store::getManager() const { 
    return mgr; 
}