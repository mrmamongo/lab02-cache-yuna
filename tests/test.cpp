// Copyright 2018 Your Name <your_email>

#include <gtest/gtest.h>
#include <cache.hpp>

TEST(Test, Main) {
    Cache cache;
    cout << cache;
}
/*TEST(Test, debug){
    Experiment experiment(Experiment::Order::Straight, std::vector<double> { 196608, 1.04858e+06, 2.09715e+06, 4.1943e+06, 8.38861e+06, 1.41558e+07 });
    for (const auto& time: experiment.time){
        cout << "time:\t" << time << endl;
    }
}*/

TEST(Test, Generator_Test) {
    auto count = Cache::GenerateExperiments();
    std::stringstream ss;
    for ( const auto & a : count ) {
        ss << a << " ";
    }
    string ref
    {"196608 1.04858e+06 2.09715e+06 4.1943e+06 8.38861e+06 1.41558e+07 "};
    EXPECT_EQ(ss.str(), ref);
}
