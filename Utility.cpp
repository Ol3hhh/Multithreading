#include "Utility.h"
#include <thread>
#include <algorithm>
#include <chrono>

void display(const std::vector<Philosopher*>& philosophers) {
    clear();
    mvprintw(0, 0, "ID\tState\t\tHunger");
    for (size_t i = 0; i < philosophers.size(); ++i) {
        std::string state;
        switch (philosophers[i]->getState()) {
            case PhilosopherState::Thinking: state = "Thinking"; break;
            case PhilosopherState::Waiting: state = "Waiting"; break;
            case PhilosopherState::Eating: state = "Eating"; break;
            case PhilosopherState::Dead: state = "Dead"; break;
        }
        mvprintw(i + 1, 0, "%d\t%-10s\t%d",
                 philosophers[i]->getId(), state.c_str(), philosophers[i]->getHunger());
    }
    refresh();
}

void waitingManager(std::vector<Philosopher*>& philosophers, std::condition_variable& cv, std::mutex& cv_mtx) {
    while (std::any_of(philosophers.begin(), philosophers.end(), [](Philosopher* p) { return p->getState() != PhilosopherState::Dead; })) {
        std::this_thread::sleep_for(std::chrono::seconds(1));

        for (auto& philosopher : philosophers) {
            if (philosopher->getState() == PhilosopherState::Waiting) {
                philosopher->increaseHunger();
            }
        }

        {
            std::unique_lock<std::mutex> lock(cv_mtx);
            cv.notify_all();
        }
    }
}

void philosopherThread(Philosopher* philosopher, std::vector<std::mutex>& forks, std::condition_variable& cv, std::mutex& cv_mtx, int n, const std::vector<Philosopher*>& philosophers) {
    while (true) {
        if (philosopher->getState() == PhilosopherState::Dead) {
            break;
        }

        philosopher->think();

        int left = philosopher->getId() + 1;
        int right = philosopher->getId();
        if (left == n) {
            left = 0;
        }
    

        {
            std::unique_lock<std::mutex> lock(cv_mtx);
            philosopher->setState(PhilosopherState::Waiting);
            cv.notify_all();
        }

        std::unique_lock<std::mutex> leftFork(forks[left], std::defer_lock);
        std::unique_lock<std::mutex> rightFork(forks[right], std::defer_lock);

        {
            std::unique_lock<std::mutex> lock(cv_mtx);
            cv.wait(lock, [&]() { return canEat(philosopher, philosophers); });
        }

        std::lock(leftFork, rightFork);

        if (philosopher->getState() == PhilosopherState::Dead) {
            break;
        }

        philosopher->eat();

        {
            std::unique_lock<std::mutex> lock(cv_mtx);
            cv.notify_all();
        }
    }
}
