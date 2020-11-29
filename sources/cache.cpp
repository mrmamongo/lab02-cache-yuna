// Copyright 2018 Your Name <your_email>

#include <cache.hpp>

auto generateArray(uint64_t size){
    auto array = new long double [size];

    for (size_t i = 0; i < size; i+=4) {
        array[i] = random();
    }
    return array;
}

Experiment::Experiment(Experiment::Order ord, const std::vector<double> &sizes) {
    order = ord;
    auto array= generateArray(static_cast<uint64_t>(1.5 * cacheSizes[2] * 1024 * 1024));
    for (const auto& size : sizes) {

        switch (ord) {

            case Order::Straight:{
                long double k = 0;

                for (size_t i = 0; i < size; ++i) { // WARMING
                    k += array[i];
                }
                k = 0;

                auto start = std::chrono::high_resolution_clock::now();
                for (size_t i = 0; i < size * testCount; i+=4) { // EXPERIMENT
                    k += array[i % static_cast<uint64_t>(size)];
                }
                auto end = std::chrono::high_resolution_clock::now();

                time.push_back(
                        std::chrono::nanoseconds(end - start).count()/1000
                        );
            }
                break;
            case Order::Back: {
                long double k = 0;

                for (size_t i = 0; i < size; ++i) { // WARMING
                    k += array[i];
                }
                k = 0;

                auto start = std::chrono::high_resolution_clock::now();
                for (size_t i = size * testCount; i > 0; i-=4) { // EXPERIMENT
                    k += array[i % static_cast<uint64_t>(size)];
                }
                auto end = std::chrono::high_resolution_clock::now();

                time.push_back(
                        std::chrono::nanoseconds(end - start).count()/1000
                );
            } break;
            case Order::Random: {
                long double k = 0;

                for (size_t i = 0; i < size; ++i) { // WARMING
                    k += array[i];
                }
                k = 0;

                auto start = std::chrono::high_resolution_clock::now();
                std::set<size_t> used;
                size_t j;
                for (size_t i = 0; i < size*testCount; i+=4) {
                    j = random() % static_cast<uint64_t>(size);

                    while (used.find(j) != used.end()) {
                        j = random() % static_cast<uint64_t>(size);
                    }
                    k += array[j % static_cast<uint64_t>(size)];
                }

                auto end = std::chrono::high_resolution_clock::now();

                time.push_back(
                        std::chrono::nanoseconds(end - start).count()/1000
                );
            }
                break;
        }

    }
    delete[] array;
}



void Cache::print(std::ostream& os, Experiment experiment) const {
    os << "investigation:" << endl
    << " travel_order: " << string(experiment) << endl
    << "  experiments:" << endl;
    for (size_t i = 0; i < experimentSizes.size(); ++i){
        os << "  - experiment:" << endl
        << "\t number: " << i+1 << endl
        << "\t  input_data:" << endl
        << "\t\tbuffer_size: \""
        << experimentSizes[i]/(1024*1024) << " MB\"\n"
        << "\t\tresults:\n\t\t experiment_duration: \""
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
            Experiment(Experiment::Order::Straight, experimentSizes),
            Experiment(Experiment::Order::Back, experimentSizes),
            Experiment(Experiment::Order::Random, experimentSizes)};
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
