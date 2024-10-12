#ifndef SPINNER_HPP
#define SPINNER_HPP

#include "./catch.hpp"
#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>

// Spinner function
std::atomic<bool> running(true);

void spinner() {
    const char* spinner_chars = "|/-\\";
    int i = 0;
    while (running) {
        std::cout << "\r" << spinner_chars[i++] << " Running tests..." << std::flush;
        if (i == 4) i = 0;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));  // Control speed
    }
}

// Catch2 listener to manage the spinner
class SpinnerListener : public Catch::TestEventListenerBase {
public:
    using TestEventListenerBase::TestEventListenerBase;

    void testRunStarting(Catch::TestRunInfo const& testRunInfo) override {
        running = true;
        std::thread spinner_thread(spinner);
        spinner_thread.detach();
    }

    void testRunEnded(Catch::TestRunStats const& testRunStats) override {
        running = false;
        std::cout << "\r" << std::flush;  // Clear spinner
    }
};

// Register the listener with Catch2
CATCH_REGISTER_LISTENER(SpinnerListener)

#endif // SPINNER_HPP

