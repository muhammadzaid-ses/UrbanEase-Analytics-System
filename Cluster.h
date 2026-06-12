#pragma once
#include "store.h"
#include <cmath>
#include <iostream>

class Centroid {
private:
    double lat, lon;

public:
    Centroid(double la = 0, double lo = 0) : lat(la), lon(lo) {}

    double getLat() const { return lat; }
    double getLon() const { return lon; }

    void set(double la, double lo) {
        lat = la;
        lon = lo;
    }

    bool operator==(const Centroid& other) const {
        return (std::fabs(lat - other.lat) < 1e-6) &&
               (std::fabs(lon - other.lon) < 1e-6);
    }

    bool operator!=(const Centroid& other) const {
        return !(*this == other);
    }
};

class Cluster {
private:
    Centroid centroid;
    Store**  memberStores;
    int      memberCount;
    int      capacity;
    int      clusterID;

public:
    Cluster();
    ~Cluster();

    Cluster operator+(const Cluster& other) const;

    friend std::ostream& operator<<(std::ostream& os, const Cluster& c);

    void     setCentroid(double lat, double lon);
    double   getCentroidLat() const { return centroid.getLat(); }
    double   getCentroidLon() const { return centroid.getLon(); }
    Centroid getCentroid()    const { return centroid; }

    void   addStore(Store* s);
    void   clearMembers();
    int    getMemberCount() const { return memberCount; }
    Store* getMember(int index) const { return memberStores[index]; }

    void setID(int id) { clusterID = id; }
    int  getID()       const { return clusterID; }

    double getTotalRevenue() const;
    double getTotalProfit()  const;
    void   displayCluster()  const;

    static double calculateDistance(double lat1, double lon1,
                                    double lat2, double lon2);

    void performProfitSubClustering(int subClusters);
};