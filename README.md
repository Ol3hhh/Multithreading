# Dining Philosophers Problem – Multithreaded Simulation in C++

This project is a multithreaded simulation of the classic **Dining Philosophers Problem**, implemented in modern **C++** using threads, mutexes, and condition variables.  
It also features a **real-time visualization** using the `ncurses` library.

## Overview

The Dining Philosophers Problem is a classic concurrency problem that illustrates synchronization issues when multiple processes compete for limited shared resources (forks).

Each philosopher alternates between thinking and eating. To eat, a philosopher needs both the left and right fork. The program ensures that the philosophers can operate without deadlock or starvation.

## Features

- Thread-based simulation using `std::thread`
- Synchronization with `std::mutex` and `std::condition_variable`
- Visual state updates (eating, thinking, waiting) using `ncurses`
- Configurable number of philosophers via command-line argument
- Deadlock-aware resource handling

## Requirements

- C++17 or later
- `ncurses` library
- POSIX-compatible terminal (for ncurses)
Building project
```bash
g++ -std=c++17 -lncurses -pthread main.cpp Philosopher.cpp Utility.cpp -o philosophers
```
Running 
```bahs
./philosophers 5
```
