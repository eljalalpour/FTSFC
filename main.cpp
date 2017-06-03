#include <iostream>
#include "FTClient.hh"
using namespace std;

void printState(FTTimestampState &ft_state) {
    printf("Timestamp %llu:\n", ft_state.timestamp);
    for (auto it = ft_state.state.begin(); it != ft_state.state.end(); ++it)
        printf("%s: %s\n", it->first.c_str(), it->second.c_str());
}

int main(){
    RandomState rs;
    FTTimestampState ft_state;
    rs.random_ts_state(20, ft_state);

    // Send state
    vector<string> ips;
    vector<int> ports;

    ips.push_back("127.0.0.1"); ports.push_back(11111);
    ips.push_back("127.0.0.1"); ports.push_back(22222);
    ips.push_back("127.0.0.1"); ports.push_back(33333);

    FTClient client(ips, ports);
    client.send(ft_state);

    printf("State sent is\n");
    printState(ft_state);

    return 0;
}