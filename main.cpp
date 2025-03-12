#include <iostream>
#include <thread>
#include <algorithm>
#include "Philosopher.h"
#include "Utility.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <number_of_philosophers>\n";
        return 1;
    }

    int n = std::stoi(argv[1]);
    if (n < 5) {
        std::cerr << "Number of philosophers must be at least 5!\n";
        return 1;
    }

    srand(time(0));
    std::vector<Philosopher*> philosophers;
    std::vector<std::mutex> forks(n);
    std::condition_variable cv;
    std::mutex cv_mtx;

    for (int i = 0; i < n; ++i) {
        philosophers.push_back(new Philosopher(i));
    }

    initscr();
    noecho();
    curs_set(0);

    std::vector<std::thread> threads;
    for (int i = 0; i < n; ++i) {
        threads.emplace_back(philosopherThread, philosophers[i], std::ref(forks), std::ref(cv), std::ref(cv_mtx), n, std::cref(philosophers));
    }

    std::thread waitingThread(waitingManager, std::ref(philosophers), std::ref(cv), std::ref(cv_mtx));

    while (std::any_of(philosophers.begin(), philosophers.end(), [](Philosopher* p) { return p->getState() != PhilosopherState::Dead; })) {
        display(philosophers);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    for (auto& t : threads) {
        t.join();
    }

    waitingThread.join();

    endwin();

    for (auto p : philosophers) {
        delete p;
    }

    return 0;
}
//g++ -std=c++17 -o dining_philosophers Philosopher.cpp Utility.cpp main.cpp -lncurses 
