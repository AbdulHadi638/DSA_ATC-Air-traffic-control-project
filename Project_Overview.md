# Air Traffic Control (ATC) Simulation System

## Overview

The Air Traffic Control (ATC) Simulation System is a C++ project that simulates airport traffic management using Data Structures and Algorithms (DSA). The system manages aircraft landings and takeoffs while ensuring efficient scheduling, fair resource allocation, and emergency handling.

## Features

- Priority-based aircraft landing using a Min Heap
- FIFO takeoff scheduling using a Queue
- Fast flight lookup using a Hash Table
- Event logging using a Linked List
- Emergency aircraft handling with highest landing priority
- File-based airport data loading
- Flight search functionality
- Daily operational reports
- Cycle-based simulation of airport operations

## Data Structures Used

### Min Heap
Used to manage aircraft waiting to land. Planes closer to the runway receive higher priority and are cleared for landing first.

### Queue
Used to manage aircraft waiting for takeoff. Implements First-In-First-Out (FIFO) scheduling to ensure fairness.

### Hash Table
Stores flight information and enables fast lookup of aircraft using their flight IDs.

### Linked List
Maintains a chronological log of all system events, including landings, takeoffs, and emergencies.

## Scheduling Rules

- Aircraft land according to runway proximity.
- Emergency aircraft receive immediate priority.
- Takeoffs follow FIFO order.
- One takeoff is allowed after every three landings.
- Flight IDs must be unique.

## Input Format

```text
AIR
PK301 5
QR202 3
EK500 8

GROUND
GT101
GT102
GT103
```

## Main Operations

1. Run a simulation cycle
2. Run multiple simulation cycles
3. Declare an emergency
4. Add airborne aircraft
5. Add ground aircraft
6. View airspace status
7. View takeoff queue
8. Search aircraft by Flight ID
9. View flight event log
10. Generate daily report

## Emergency Handling

When an aircraft declares an emergency:

- Its priority is updated to `0`
- It moves to the top of the landing queue
- It is selected for landing in the next simulation cycle

## Technologies

- C++
- Object-Oriented Programming (OOP)
- Data Structures & Algorithms

## Documentation

- 📄 [Project Proposal](ATC_Project_Proposal.pdf)
- 📄 [Project Report](ATC_Project_Report.pdf)

## Authors

- M. Abdul Hadi (24I-6538)
- Abdullah Muhammad (24I-6560)

## Future Improvements

- Multiple runway support
- Real-time flight updates
- Graphical User Interface (GUI)
- Integration with real-world flight datasets
- Concurrent landing and takeoff operations
