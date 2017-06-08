#include <iostream>
#include "FTClient.hh"
using namespace std;

void printState(FTState &ft_state) {
    for (auto it = ft_state.begin(); it != ft_state.end(); ++it)
        printf("%s: %s\n", it->first.c_str(), it->second.c_str());
}

int main(){
    RandomState rs;
    FTState ft_state;
    rs.random_state(20, ft_state);

    // Send state
    vector<string> ips;
    vector<uint16_t> ports;

    ips.push_back("127.0.0.1"); ports.push_back(11111);
    ips.push_back("127.0.0.1"); ports.push_back(22222);
    ips.push_back("127.0.0.1"); ports.push_back(33333);

    FTClient client(ips, ports);

    for (int i = 0; i < 10; i++)
        client.send(ft_state);

    printf("State sent is\n");
    printState(ft_state);

    return 0;
}