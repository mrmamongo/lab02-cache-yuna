// Copyright 2018 Your Name <your_email>

#ifndef INCLUDE_CACHE_HPP_
#define INCLUDE_CACHE_HPP_

#include <chrono>
#include <iostream>
#include <utility>
#include <vector>
#include <set>
#include <string>

using std::cout;
using std::endl;
using std::string;

static const size_t offset = 4;
static const int MBinB = 1024*1024;
static const int testCount = 1000;
static const std::vector<double> cacheSizes {0.375, 1.5, 9};


struct Experiment{
    enum class Order{
        Straight,
        Back,
        Random
    };
    Order order;

    std::vector<double> time;

    explicit Experiment(Order ord, const size_t& sizes);
    explicit operator string() const;
};

class Cache {
    std::vector<double> experimentSizes;
    std::vector<Experiment> experiments;
public:
    static std::vector<double> GenerateExperiments();
    Cache();

    friend std::ostream& operator<<(std::ostream&, const Cache&);
    void print(std::ostream&, Experiment) const;
};

#endif // INCLUDE_CACHE_HPP_
