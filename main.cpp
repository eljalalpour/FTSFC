#include <iostream>
//#include "include/FTAppenderElement.hh"
//#include "include/FTStateElement.hh"
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/filter/zlib.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/nvp.hpp>
#include <string>
#include <sstream>
#include <fstream>
#include "FTTypes.hh"

using namespace std;

void compress(std::string data, std::string &buffer) {
    buffer.clear();
    boost::iostreams::filtering_streambuf<boost::iostreams::output> out;
    out.push(boost::iostreams::zlib_compressor());
    out.push(boost::iostreams::back_inserter(buffer));
    boost::iostreams::copy(boost::make_iterator_range(data), out);
}

void decompress(std::string data, std::string &buffer) {
    boost::iostreams::filtering_streambuf<boost::iostreams::input> in;
    in.push(boost::iostreams::zlib_decompressor());
    in.push(boost::make_iterator_range(data));
    buffer.clear();
    boost::iostreams::copy(in, boost::iostreams::back_inserter(buffer));
}

void serializePiggyBacked(FTPacketMBPiggyBackedState &pbStates, stringstream &ss) {
    boost::archive::binary_oarchive oa(ss);
    oa << pbStates;
}

void serializePiggyBacked(FTPacketMBPiggyBackedState &pbStates, ofstream &fss) {
    boost::archive::binary_oarchive oa(fss);
    oa << pbStates;
}

void deserializePiggyBacked(stringstream &ss, FTPacketMBPiggyBackedState &piggyBackedStates) {
    boost::archive::binary_iarchive ia(ss);
    ia >> piggyBackedStates;
}

void deserializePiggyBacked(ifstream &ifs, FTPacketMBPiggyBackedState &piggyBackedStates) {
    boost::archive::binary_iarchive ia(ifs);
    ia >> piggyBackedStates;
}

void serialize(FTMBStates &state, stringstream &ss) {
    boost::archive::binary_oarchive oa(ss);
    oa << state;
}

void deserialize(stringstream &ss, FTMBStates &state) {
    boost::archive::binary_iarchive ia(ss);
    ia >> state;
}

void printState(FTState &state) {
    for (auto it = state.begin(); it != state.end(); ++it) {
        cout << it->first.c_str() << ":" << it->second.c_str() << endl;
    }//for
}

void printState(FTPiggyBackedState &state) {
    printf("Ack is %d, commit is %d\n", state.ack, state.commit);
    printState(state.state);
}

void printState(FTPacketMBPiggyBackedState &state) {
    cout << "In print state:" << endl;
    for (auto i = state.begin(); i != state.end(); ++i) {
        cout << "State of packet " << i->first << endl;
        for (auto j = i->second.begin(); j != i->second.end(); ++j) {
            cout << "State of middlebox " << (uint16_t)(j->first) << endl;
            printState(j->second);
        }//for
    }//for
}

//class Test {
//    friend class boost::serialization::access;
//
//public:
//    uint8_t ack;
//    bool commit;
//    map<string, string> state;
//
//    // template<class Archive>
//    // void serialize(Archive &ar, const unsigned int version);
//
//    // // This function is used by boost serializer to serialize a FTPiggyBackedState instance
//    // static void serialize(FTPiggyBackedState &pbState, stringstream &ss);
//
//    // // This function is used by boost serializer to deserialize a FTPiggyBackedState instance
//    // static void deserialize(stringstream &ss, FTPiggyBackedState &pbState);
//
//    template<class Archive>
//    void serialize(Archive &ar, const unsigned int) {
//        ar & ack & commit & BOOST_SERIALIZATION_NVP(state);
//    }
//
//   static void serialize(Test &pbState, stringstream &ss) {
//        boost::archive::binary_oarchive oa(ss);
//        oa << pbState;
//    }
//
//    static void deserialize(stringstream &ss, Test &pbState) {
//        boost::archive::binary_iarchive ia(ss);
//        ia >> pbState;
//    }
//};

// FTMBStates type represents the state of a MB with id FTMBId
//typedef map<FTMBId, FTPiggyBackedState> FTMBPiggyBackedState;

int main() {
    stringstream stateSS;
    FTPiggyBackedState pbState1;
    pbState1.ack = 10;
    pbState1.state["khar"] = "olagh";
    pbState1.state["gol"] = "Haibo";
    pbState1.commit = false;

    FTPiggyBackedState pbState2;
    pbState2.ack = 9;
    pbState2.state["gav"] = "sag";
    pbState2.state["sag"] = "Eric";
    pbState2.commit = true;

    FTMBPiggyBackedState mbStates;
    mbStates[0] = pbState1;
    mbStates[1] = pbState2;

    FTPacketMBPiggyBackedState _temp;
    _temp[0] = mbStates;

//    stringstream ss;
//    serializePiggyBacked(_temp, ss);

//    const unsigned char * buffer = reinterpret_cast<const unsigned char*>(ss.str().c_str());
//    string states(reinterpret_cast<const char*>(buffer), ss.str().size());

//    ofstream ofile("test.bin", ios::binary);
//    serializePiggyBacked(_temp, ofile);
//    ofile.close();

//    ofstream ofile("test.bin", ios::binary);
//    stringstream sss;
//    serializePiggyBacked(_temp, sss);
//    string buffer;
//    compress(sss.str(), buffer);
//    ofile << buffer;
//    ofile.close();

    std::ifstream input("test.bin", ios::binary);
    std::vector<char> buffer2((std::istreambuf_iterator<char>(input)),
                                (std::istreambuf_iterator<char>()));
    string data (buffer2.begin(), buffer2.end());
    string buffer3;
    decompress(data, buffer3);
    stringstream ss2(string(buffer3.begin(), buffer3.end()));
    FTPacketMBPiggyBackedState _temp2;
    deserializePiggyBacked(ss2, _temp2);
    printState(_temp2);

//    std::ifstream input("test.bin", ios::binary);
//    FTPacketMBPiggyBackedState _temp2;
//    deserializePiggyBacked(input, _temp2);
//    printState(_temp2);

//    Test piggyBackedState1, piggyBackedState2;
//
//    map<string, string> state1;
//    state1["a"] = "Milad";
//    state1["b"] = "Akbar";
//
//    piggyBackedState1.state = state1;
//    piggyBackedState1.ack = 1;
//    piggyBackedState1.commit = false;
//
//    stringstream ss;
//    Test::serialize(piggyBackedState1, ss);
//    const unsigned char * buffer = reinterpret_cast<const unsigned char*>(ss.str().c_str());
//    string states(reinterpret_cast<const char*>(buffer), ss.str().size());
//
//    stringstream ss2;
//    ss2 << states;
//    Test::deserialize(ss2, piggyBackedState2);
//
//    string dd;
//    ss >> dd;
//
//    string comp, decomp;
//    compress(dd, comp);
//
//    unsigned char * buffer = new unsigned char[comp.size()];
//    memcpy(buffer, comp.c_str(), comp.size());
//
//    string states(reinterpret_cast<const char*>(buffer), comp.size());
//
//    decompress(states, decomp);
//
//    stringstream decompSS;
//    decompSS << decomp;
//    Test::deserialize(decompSS, piggyBackedState2);

    return 0;
}