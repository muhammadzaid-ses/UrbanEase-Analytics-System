#pragma once
#include <iostream>
#include <string>

class Person {
private:
    std::string name;
    int id;
public:
    Person(std::string n = "", int i = 0);
    virtual ~Person();
    virtual void display() const;
    std::string getName() const;
    int getId() const;
    void setName(const std::string& n);
    void setId(int i);
    // Stream operators for file I/O
    friend std::ostream& operator<<(std::ostream& os, const Person& p);
    friend std::istream& operator>>(std::istream& is, Person& p);
};

class Employee : public Person {
private:
    double salary;
public:
    Employee(std::string n = "", int i = 0, double s = 0.0);
    void display() const override;
    double getSalary() const;
    void setSalary(double s);
    friend std::ostream& operator<<(std::ostream& os, const Employee& e);
    friend std::istream& operator>>(std::istream& is, Employee& e);
};

class Manager : public Employee {
private:
    std::string department;
public:
    Manager(std::string n = "", int i = 0, double s = 0.0, std::string d = "");
    void display() const override;
    std::string getDepartment() const;
    void setDepartment(const std::string& d);
    friend std::ostream& operator<<(std::ostream& os, const Manager& m);
    friend std::istream& operator>>(std::istream& is, Manager& m);
};

class Location {
private:
    double latitude, longitude;
public:
    Location(double lat = 0, double lon = 0);
    double getLat() const;
    double getLon() const;
    void setLat(double lat);
    void setLon(double lon);
};

class Analytics {
private:
    double sales[24];
    double costs[24];
    int    customers[24];
    double compositeScore;
public:
    Analytics();
    // Task 1: Subscript operator for indexed monthly access
    double& operator[](int i);
    double getMonthlySales(int i) const;
    double getMonthlyCost(int i)  const;
    int getMonthlyCustomers(int i) const;
    void setSales(int i, double v);
    void setCost(int i, double v);
    void setCustomers(int i, int v);
    // Task 3: Comparison by composite score
    bool operator>(const Analytics& other) const;
    bool operator<(const Analytics& other) const;
    // Task 3: Combine two analytics objects (cluster totals)
    Analytics operator+(const Analytics& other) const;
    // Pre/Post increment simulate 5% growth
    Analytics& operator++();
    Analytics operator++(int);
    Analytics& operator--();
    Analytics operator--(int);

    void calculateScore();
    double getScore() const;

    // Helper to get total profit
    double getTotalProfit() const;
    double getGrowthRate() const;
};
// task 04
class Forecast {
private:
    double expectedSales;
    double confidenceLevel;
    bool warningFlag;
    int projectionMonth;
public:
    Forecast();

    // Combine two forecasts (e.g. cluster-wide forecast)
    Forecast operator+(const Forecast& other) const;

    // Post-increment: advance prediction one month
    Forecast operator++(int);

    void calculate(const double sales[], const double costs[]);

    double getExpected() const { return expectedSales; }
    double getConfidence() const { return confidenceLevel; }
    bool getWarning() const { return warningFlag; }
    int getMonth() const { return projectionMonth; }

    friend std::ostream& operator<<(std::ostream& os, const Forecast& f);
};