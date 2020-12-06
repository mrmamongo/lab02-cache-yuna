// Copyright 2018 Your Name <your_email>

#include <cache.hpp>

auto generateArray(uint64_t size){
    auto array = new long double [size];

    for (size_t i = 0; i < size; i+=4) {
        array[i] = random();
    }
    return array;
}

double straightExperiment(size_t size) {
    auto array = generateArray(size);
    long double k = 0;

    for (size_t i = 0; i < size; ++i) { // WARMING
        k += array[i];
    }
    k = 0;

    auto start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < size * testCount; i+=4) { // EXPERIMENT
        k += array[i % size];
    }
    auto end = std::chrono::high_resolution_clock::now();
    delete[] array;
    return
    static_cast<double>\
    (std::chrono::nanoseconds(end - start).count())/1000;
}
double backExperiment (size_t size) {
    auto array = generateArray(size);
    long double k = 0;

    for (size_t i = 0; i < size; ++i) { // WARMING
        k += array[i];
    }
    k = 0;

    auto start = std::chrono::high_resolution_clock::now();
    for (size_t i = size * testCount; i > 0; i-=4) { // EXPERIMENT
        k += array[i % size];
    }
    auto end = std::chrono::high_resolution_clock::now();
    delete[] array;
    return
    static_cast<double>\
    (std::chrono::nanoseconds(end - start).count())/1000;
}
double randomExperiment (size_t size) {
    auto array = generateArray(size);
    long double k = 0;

    for (size_t i = 0; i < size; ++i) { // WARMING
        k += array[i];
    }
    k = 0;

    auto start = std::chrono::high_resolution_clock::now();
    std::set<size_t> used;
    size_t l;
    for (size_t i = 0; i < size*testCount; i+=4) {
        l = (random() % size) % 4;

        while (used.find(l) != used.end()) {l = (random() % size) % 4;}
        k += array[l % size];
    }

    auto end = std::chrono::high_resolution_clock::now();
    delete[] array;
    return
    static_cast<double>(std::chrono::nanoseconds(end - start).count())/1000;
}

Experiment::Experiment(Experiment::Order ord, const size_t& sizes) {
    order = ord;
    size_t size = cacheSizes[0] * 1024 * 1024;
    for (size_t j = 0; j < sizes; ++j) {
        switch (ord) {
            case Order::Straight:{
                time.push_back(straightExperiment(size));
            }
                break;
            case Order::Back: {
                time.push_back(backExperiment(size));
            } break;
            case Order::Random: {
                time.push_back(randomExperiment(size));
            }
                break;
        }
    }
}


void Cache::print(std::ostream& os, Experiment experiment) const {
    os << "investigation:" << endl
    << " travel_order: " << string(experiment) << endl
    << "  experiments:" << endl;
    for (size_t i = 0; i < experimentSizes.size(); ++i){
        os << "  - experiment:" << endl
        << "     number: " << i+1 << endl
        << "      input_data:" << endl
        //<< "\t\tbuffer_size: \""
        //<< experimentSizes[i]/(1024*1024) << " MB\"\n"
        << "        results:\n         experiment_duration: \""
        << experiment.time[i] << " ns\"\n";
    }
}

Experiment::operator string() const {
    string out;
    switch (order) {
        case Order::Straight:
            out = "straight";
            break;
        case Order::Back:
            out = "back";
            break;
        case Order::Random:
            out = "random";
            break;
    }
    return out;
}



std::ostream &operator<<(std::ostream & os, const Cache& cache) {
    for (const auto& experiment : cache.experiments){
        cache.print(os, experiment);
    }
    return os;
}

Cache::Cache() {
    experimentSizes = GenerateExperiments();

    experiments = std::vector<Experiment>{
            Experiment(Experiment::Order::Straight, experimentSizes.size()),
            Experiment(Experiment::Order::Back, experimentSizes.size()),
            Experiment(Experiment::Order::Random, experimentSizes.size())};
}

std::vector<double> Cache::GenerateExperiments() {
    double value = 1.0;
    std::vector<double> out {cacheSizes[0] * 0.5 * 1024 * 1024};
    while (value < 1.5 * cacheSizes[2]) {
        out.push_back(value * 1024 * 1024);
        value *= 2;
    }
    out.push_back(cacheSizes[2] * 1.5 * 1024 * 1024);
    return out;
}
