// Copyright 2018 Your Name <your_email>

#include <cache.hpp>

Experiment::Experiment(Order ord): order(ord) {
    switch (ord) {
        case Order::Straight: {
            for (int experimentSize : experimentSizes) {
                long double k = 0;
                auto arr = new long double[experimentSize];
                for (int j = 0; j < experimentSize; j += 4) { // WARMING
                    arr[j] = random();
                }
                std::chrono::system_clock::time_point start =
                        std::chrono::high_resolution_clock::now();
                for (int j = 0;
                     j < experimentSize * testCount;
                     j += 4)
                { // EXPERIMENT
                    k += arr[j % experimentSize];
                }
                std::chrono::system_clock::time_point end =
                        std::chrono::high_resolution_clock::now();
                time.push_back(
                        static_cast<double>\
                    ((std::chrono::nanoseconds(end - start)).count()));
                delete arr;
            }
        }
            break;
        case Order::Back: {
            for (int experimentSize : experimentSizes) {
                long double k = 0;
                auto arr = new long double[experimentSize];
                for (int j = 0; j < experimentSize; j += 4) { // WARMING
                    arr[j] = random();
                }
                std::chrono::system_clock::time_point start =
                        std::chrono::high_resolution_clock::now();
                for (
                    int j = experimentSize * testCount;
                    j > 0;
                    j -= 4)
                { // EXPERIMENT
                    k += arr[j % experimentSize];
                }
                std::chrono::system_clock::time_point end =
                        std::chrono::high_resolution_clock::now();
                time.push_back(
                        static_cast<double>\
                    ((std::chrono::nanoseconds(end - start)).count()));
                delete arr;
            }
        }
            break;
        case Order::Random: {
            for ( int experimentSize : experimentSizes ) {
                long double k = 0;
                auto arr = new long double[experimentSize];
                for (int j = 0; j < experimentSize; j += 4) {
                    arr[j] = random();
                }
                std::set<int> used;
                std::chrono::system_clock::time_point start =
                        std::chrono::high_resolution_clock::now();
                int j = 0;
                for (int i = 0;
                     i < experimentSize * testCount;
                     i += 4)
                {
                    j = random() % experimentSize;
                    while (used.find(j) != used.end()) {
                        break;
                    }
                    k += arr[j % experimentSize];
                }
                std::chrono::system_clock::time_point end =
                        std::chrono::high_resolution_clock::now();
                time.push_back(
                        static_cast<double>\
                    ((std::chrono::nanoseconds(end - start)).count()));
                delete[] arr;
            }
        }
            break;
    }
} // Commiting

std::ostream &operator<<(std::ostream & os, Experiment experiment) {
    os << "investigation:" << endl
    << " travel_order: " << string(experiment) << endl
    << "  experiments:" << endl;
    for (size_t i = 0; i < experimentSizes.size(); ++i){
        os << "  - experiment:" << endl
        << "\t number: " << i+1 << endl
        << "\t  input_data:" << endl
        << "\t\tbuffer_size: \"";
        if (i == 0) {
            os << experimentSizes[i]/(1024.0/16.0) << " KB\""
                << endl;
        } else {
                os << experimentSizes[i]/(1024.0*1024.0/16.0)
                    << " MB\"" << endl;
        }
        os <<
            "\t\tresults:\n\t\t experiment_duration: \""
            << experiment.time[i] << " ns\"\n";
    }
    return os;
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
        os << experiment;
    }
    return os;
}
