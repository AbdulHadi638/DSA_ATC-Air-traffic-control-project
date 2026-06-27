# ✈️ Air Traffic Control (ATC) Simulation System

**DSA Project — CE-4B FAST NUCES ISLAMABAD**  

---

## What is this?

This is a C++ simulation of an Air Traffic Control system for JFK Airport. It loads real flight data from a 2015 Kaggle dataset and manages which planes land, which take off, and what happens when there's an emergency  all using the data structures we studied in DSA.

We also built a live radar dashboard that runs in your browser and updates automatically as you run the simulation. It shows the planes on a radar screen, the landing queue, takeoff queue, and live stats.

---

## Data Structures Used

- **Min Heap** — manages landing order. Planes closest to JFK land first
- **Queue** — manages takeoff order. First plane to queue is first to take off (FIFO)
- **Hash Table** — stores all flight info so we can search any plane instantly
- **Linked List** — keeps a log of every event that happens during the simulation

---

## Features

- Loads real JFK flight data from a CSV file
- Lands planes in order of proximity using a min heap
- Lets you declare an emergency — that plane immediately jumps to the front
- Allows 1 takeoff after every 3 landings
- Search any flight by ID
- View full event log and daily report
- Live radar dashboard in the browser that syncs with the simulation

---

## Tech Stack — ATC Simulation

- **C++** — core simulation logic
- **Windows Console API** — colored terminal output
- **File I/O (fstream)** — loading flight data from CSV and writing state.json
- **STL (queue, list)** — used for takeoff queue and event log

## Tech Stack — Graphical Interface

- **HTML + CSS** — dashboard layout and styling
- **JavaScript (Canvas API)** — radar drawing, rotating sweep animation, plane blips
- **JSON** — data format written by C++ and read by the browser
- **Node.js** — lightweight local server so Chrome can read the JSON file

---



## How to Run

### C++ Console (Visual Studio)

1. Clone or download this repo
2. Open Visual Studio and create a new empty C++ project
3. Add all `.h` and `.cpp` files to the project
4. flights.csv is not included due to file size.
Download it from Kaggle:
https://www.kaggle.com/datasets/usdot/flight-delays?select=flights.csv
Then place it in the same folder as your source files.
5. Make sure `flights.csv` is in the **same folder as your `.cpp` files**
6. Press **F5** to build and run
7. Use the menu that appears to control the simulation

### Live Radar Dashboard (Browser)

The dashboard reads a file called `state.json` that the C++ program writes automatically. To view it in the browser you need a simple local server.

**Step 1** — Install Node.js from [nodejs.org](https://nodejs.org) (just click Next through the installer)

**Step 2** — Open Command Prompt and go to your project folder:
```
cd "C:\path\to\your\project"
```

**Step 3** — Start the server:
```
npx serve .
```

**Step 4** — Open Chrome and go to:
```
http://localhost:3000/dashboard.html
```

**Step 5** — Keep the Command Prompt open (just minimize it), then use the C++ console normally. The radar updates on its own every 2 seconds after each cycle.

---

## How the Dashboard Works

Every time you run a cycle in the console, the C++ program writes a `state.json` file with the current state of the simulation. The browser reads that file every 2 seconds and redraws the radar. No page refresh needed.

```
C++ runs a cycle → writes state.json → browser reads it → radar updates
```

---


## Authors

- M. Abdul Hadi (24I-6538)
- Abdullah Muhammad (24I-6560)

