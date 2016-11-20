#include <time.h>
#include <unistd.h>
#include <vector>

/*
struct Message {
    int type;
    std::vector<int> events;
};

class MessageHandler {
    time_t start;
    void reloadConfig() {}
    void handleEvent() {
        time_t duration = (time(NULL) - start);
        if (duration > 60) { // Took longer than 60s
            reloadConfig();
        }
    }

public:
    void processMessage(int msgType) {
        // do something that might take a few seconds
    }
    void processEvent(int event) {
        // do something that might take a few seconds
    }

    void handleMessage(const Message& msg) {
        time_t start = time(NULL);
        processMessage(msg.type);
        for (int i = 0; i < msg.events.size(); ++i) {
            processEvent(msg.events[i]);
        }
    }
};
*/

struct Message {
    int type;
    std::vector<int> events;
};

void reloadConfig() { /*...*/ }
void processMessage(int msgType) { /*...*/ }
void processEvent(int event) { /*...*/ }

#define REALOAD_TIMEOUT 60

void handleMessage(const Message& msg) {
    time_t start = time(NULL);
    processMessage(msg.type);
    for (int i = 0; i < msg.events.size(); ++i) {
        processEvent(msg.events[i]);
    }
    time_t duration = (time(NULL) - start);
    if (duration > REALOAD_TIMEOUT) {
        reloadConfig();
    }
}

// TEST:
#include <gtest/gtest.h>
#include "hook.hpp"

time_t fake_time(time_t *) {
    static int called = 0;
    if (called == 0) {
        ++called;
        return 0;
    } else { // When called the 2. time, fake the timeout
        return REALOAD_TIMEOUT + 1;
    }
}

bool reloadConfigCalled = false;
void mock_reloadConfig() { reloadConfigCalled = true; }

TEST(handleMessage, reloadConfig_shall_be_called_on_timeout) {
    SUBSTITUTE(&time, &fake_time);
    SUBSTITUTE(&reloadConfig, &mock_reloadConfig);
    handleMessage(Message());
    EXPECT_EQ(reloadConfigCalled, true);
}

