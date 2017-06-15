#include <iostream>
#include "FTClient.hh"
#include "types.pb.h"
#include <chrono>
#include <fstream>

using namespace std;
using namespace std::chrono;
using namespace tutorial;

#define COUNT 10000

void printState(FTState &ft_state) {
    for (auto it = ft_state.begin(); it != ft_state.end(); ++it)
        printf("%s: %s\n", it->first.c_str(), it->second.c_str());
}

void copy(FTState& state, PiggyBackState& pstate) {
    for (auto it = state.begin(); it != state.end(); ++it) {
        pstate.add_key(it->first);
        pstate.add_value(it->second);
    }
}

int main(){
    RandomState rs;
    FTState ft_state;
    rs.random_state(20, ft_state);

    PiggyBackState state;
    copy(ft_state, state);

    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    string str;
    for (int i = 0; i < COUNT; ++i) {
        str = state.SerializeAsString();
    }
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    auto avg1 = duration_cast<microseconds>( t2 - t1 ).count() / COUNT;

    t1 = high_resolution_clock::now();
    for (int i = 0; i < COUNT; ++i) {
        state.ParseFromString(str);
    }
    t2 = high_resolution_clock::now();
    auto avg2 = duration_cast<microseconds>( t2 - t1 ).count() / COUNT;

    t1 = high_resolution_clock::now();
    for (int i = 0; i < COUNT; ++i) {
        stringstream ss;
        boost::archive::binary_oarchive oa(ss);
        oa << ft_state;
        ss >> str;
    }
    t2 = high_resolution_clock::now();
    auto avg3 = duration_cast<microseconds>( t2 - t1 ).count() / COUNT;

    t1 = high_resolution_clock::now();
    for (int i = 0; i < COUNT; ++i) {
        stringstream ss;
        ss.str(str);
        boost::archive::binary_iarchive ia(ss);
        FTState sts2;
        ia >> sts2;
    }
    t2 = high_resolution_clock::now();
    auto avg4 = duration_cast<microseconds>( t2 - t1 ).count() / COUNT;


    cout << "protoBuffer: " << avg1 << " " << avg2 << endl;
    cout << "boost:       " << avg3 << " " << avg4 << endl;

    return 0;
}