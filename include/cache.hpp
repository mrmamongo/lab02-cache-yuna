// Copyright 2018 Your Name <your_email>

#ifndef INCLUDE_CACHE_HPP_
#define INCLUDE_CACHE_HPP_

#include <chrono>
#include <iostream>
#include <vector>
#include <set>
#include <string>

using std::cout;
using std::endl;
using std::string;

static const int testCount = 1000;
static std::vector<int> experimentSizes =
{12288, 65536, 131072, 262144, 524288, 884736};

struct Experiment{
    enum class Order{
        Straight,
        Back,
        Random
    };
    Order order;

    std::vector<double> time;
    std::vector<double> wTime;

    explicit Experiment(Order ord);
    friend std::ostream& operator<<(std::ostream&, Experiment);
    explicit operator string() const;
};

class Cache {
    std::vector<Experiment> experiments = std::vector<Experiment>{
            Experiment(Experiment::Order::Straight),
            Experiment(Experiment::Order::Back),
            Experiment(Experiment::Order::Random)
    };
public:
    Cache() = default;

    friend std::ostream& operator<<(std::ostream&, const Cache&);
};

#endif // INCLUDE_CACHE_HPP_
