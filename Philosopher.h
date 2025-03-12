#pragma once
#include <mutex>
#include <condition_variable>
#include <vector>
#include <string>
#include <chrono>

const int MAX_HUNGER = 10;

enum PhilosopherState {
    Thinking,
    Waiting,
    Eating,
    Dead
};

class Philosopher {
private:
    int id;
    int hunger;
    PhilosopherState state;
    mutable std::mutex mtx;

public:
    Philosopher(int id);

    int getId() const;
    PhilosopherState getState() const;
    int getHunger() const;

    void increaseHunger();
    void resetHunger();
    void setState(PhilosopherState newState);

    void think();
    void eat();
};

bool canEat(Philosopher* philosopher, const std::vector<Philosopher*>& philosophers);
