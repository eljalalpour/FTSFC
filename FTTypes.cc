#include "FTTypes.hh"

FTState random_state(int count) {
    FTState state;
    for (int i = 0; i < count; i++) {
        int k = random();
        int v = random();
        stringstream ss1, ss2;
        ss1 << k;
        ss2 << v;

        state[ss1.str()] = ss2.str();
    }//for
    return state;
}

FTTimestampState random_ts_state(int count) {
    FTTimestampState ts_state;
    ts_state.timestamp = CURRENT_TIMESTAMP;
    ts_state.state = random_state(count);

    return ts_state;
}

FTPiggyBackState random_piggy_back(int count) {
    FTPiggyBackState pb_state;
    pb_state.last_commit = CURRENT_TIMESTAMP;

    FTTimestampState ts_state = random_ts_state(count);
    pb_state.state = ts_state.state;
    pb_state.timestamp = ts_state.timestamp;
    pb_state.ack = 0;

    return pb_state;
}

FTPiggyBackMessage random_message(int mb_count, int state_count) {
    FTPiggyBackMessage msg;

    for (int i = 0; i < mb_count; ++i) {
        msg[i] = random_piggy_back(state_count);
    }//for

    return msg;
}