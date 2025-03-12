#include "Philosopher.h"
#include <thread>
#include <cstdlib>
#include <algorithm>

Philosopher::Philosopher(int id) : id(id), hunger(0), state(PhilosopherState::Thinking) {}

int Philosopher::getId() const { 
    std::lock_guard<std::mutex> lock(mtx);
    return id; 
}

PhilosopherState Philosopher::getState() const {
    std::lock_guard<std::mutex> lock(mtx);
    return state;
}

int Philosopher::getHunger() const {
    std::lock_guard<std::mutex> lock(mtx);
    return hunger;
}

void Philosopher::increaseHunger() {
    std::lock_guard<std::mutex> lock(mtx);
    if (state == PhilosopherState::Waiting) {
        hunger++;
        if (hunger >= MAX_HUNGER) {
            state = PhilosopherState::Dead;
        }
    }
}

void Philosopher::resetHunger() {
    std::lock_guard<std::mutex> lock(mtx);
    hunger = 0;
}

void Philosopher::setState(PhilosopherState newState) {
    std::lock_guard<std::mutex> lock(mtx);
    state = newState;
}

void Philosopher::think() {
    setState(PhilosopherState::Thinking);
    for (int i = 0; i < 5; ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
}

void Philosopher::eat() {
    setState(PhilosopherState::Eating);
    int eatTime = 2000 + rand() % 1000;
    std::this_thread::sleep_for(std::chrono::milliseconds(eatTime));
    resetHunger();
}

bool canEat(Philosopher* philosopher, const std::vector<Philosopher*>& philosophers) {
    for (const auto& p : philosophers) {
        if (p->getState() == PhilosopherState::Waiting && p->getHunger() > philosopher->getHunger()) {
            return false;
        }
    }
    return true;
}
