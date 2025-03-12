#pragma once
#include <vector>
#include <ncurses.h>
#include "Philosopher.h"

void display(const std::vector<Philosopher*>& philosophers);
void waitingManager(std::vector<Philosopher*>& philosophers, std::condition_variable& cv, std::mutex& cv_mtx);
void philosopherThread(Philosopher* philosopher, std::vector<std::mutex>& forks, std::condition_variable& cv, std::mutex& cv_mtx, int n, const std::vector<Philosopher*>& philosophers);
