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
#include "FTTypes.hh"

using namespace std;

void compress(std::string &data, std::string &buffer) {
    buffer.clear();
    boost::iostreams::filtering_streambuf<boost::iostreams::output> out;
    out.push(boost::iostreams::zlib_compressor());
    out.push(boost::iostreams::back_inserter(buffer));
    boost::iostreams::copy(boost::make_iterator_range(data), out);
}

void decompress(std::string &data, std::string &buffer) {
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

void deserializePiggyBacked(stringstream &ss, FTPacketMBPiggyBackedState &piggyBackedStates) {
    cout << "State in deserialize: " << ss.str().size() << endl;
    boost::archive::binary_iarchive ia(ss);
    cout << "after archive" << endl;
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
//    FTPacketMBPiggyBackedState states;
//    FTMBPiggyBackedState mbState;
//
//    FTPiggyBackedState piggyBackedState1;
//    FTPiggyBackedState piggyBackedState2;
//
//    FTState state1;
//    state1["a"] = "Milad";
//    state1["b"] = "Akbar";
//
//    FTState state2;
//    state2["c"] = "Elaheh";
//    state2["d"] = "Batul";
//
//    piggyBackedState1.state = state1;
//    piggyBackedState1.ack = 1;
//    piggyBackedState1.commit = false;
//
//    piggyBackedState2.state = state2;
//    piggyBackedState2.ack = 1;
//    piggyBackedState2.commit = false;
//
//    mbState[0] = piggyBackedState1;
//    mbState[1] = piggyBackedState2;
//
//    states[90] = mbState;
//
//    stringstream ss;
//    serialize(states, ss);
//
//    FTPacketMBPiggyBackedState states2;
//    deserialize(ss, states2);
//
//    FTAppenderElement appenderElement;
//    appenderElement.push(10, NULL);
//    cout << "Milad" << endl;

//    FTStateElement stateElement;
//    string key, value;
//    key = "elaheh";
//    stateElement.getPrimaryState(key, value);

//    string compr, decompr;
//    compr = "x��(R(N-�L�ɬJ,��ϳ�J,J��,KU04Q0�@.";
//    cout << compr << endl;
//    decompress(compr, decompr);
//    cout << decompr << endl;

    stringstream stateSS;
    FTPiggyBackedState pbState;
    pbState.ack = 1;
    pbState.state["khar"] = "olagh";
    pbState.state["gol"] = "Haibo";
    pbState.commit = true;

    FTMBPiggyBackedState mbStates;
    mbStates[0] = pbState;
    mbStates[1] = pbState;
    mbStates[2] = pbState;

    FTPacketMBPiggyBackedState _temp;
    _temp[0] = mbStates;

    stringstream ss;
    serializePiggyBacked(_temp, ss);

    const unsigned char * buffer = reinterpret_cast<const unsigned char*>(ss.str().c_str());
    string states(reinterpret_cast<const char*>(buffer), ss.str().size());

//    string compressed, decompressed;
//    compress(compressed, states);
//    decompress(compressed, decompressed);

    stringstream ss2;
    FTPacketMBPiggyBackedState _temp2;
    ss2 << states;
    deserializePiggyBacked(ss2, _temp2);

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