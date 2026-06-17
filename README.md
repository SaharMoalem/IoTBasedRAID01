# IoT Based Drive

A Linux-based user-space program that provides a virtual drive, accessible via the native directory GUI. Although data appears to be stored locally, it is actually distributed across multiple IoT devices (referred to as *minions*) on the same local network.

---

## Motivation

This project is designed to leverage unused storage space on common household IoT devices — such as smart air conditioner controllers, smart plugs, Raspberry Pi devices, and more.

- Data accessible only within the Local Area Network (LAN)
- Implements RAID 01 mirroring across minions for secure, backed-up storage

---

## Introduction

IoT-Based Drive is a user-space application for Linux systems. It allows users to mount a virtual drive using the standard `mount` shell command.

Data is transmitted via TCP from the kernel space to the user space using the Linux **Network Block Device (NBD)** module.

> This project was developed as the final assignment in the Infinity Labs R&D Software Development Program.

---

## Prerequisites

- Linux OS (CMake build and runtime)
- CMake 3.20+
- GCC 12+ with C++23 support
- Root access (for loading kernel modules and mounting NBD)

---

## Build (CMake)

From the repository root:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j
ctest --test-dir build
```

Artifacts are written to:

| Output | Path |
|--------|------|
| Master | `build/bin/masterFramework` |
| Minions | `build/bin/minion1`, `minion2`, `minion3` |
| Handleton shared lib | `build/lib/libhandleton.so` |

### Cross-compile for Raspberry Pi (optional)

```bash
cmake -S . -B build-rpi \
  -DCMAKE_TOOLCHAIN_FILE=cmake/toolchain-aarch64.cmake \
  -DCMAKE_BUILD_TYPE=Release
cmake --build build-rpi -j
```

Requires `aarch64-linux-gnu-g++-12` on the build host.

---

## Run

### 1. Start minions

On each minion device (or locally on different ports for testing):

```bash
./build/bin/minion1   # port 9090
./build/bin/minion2   # port 9091
./build/bin/minion3   # port 9092
```

Each minion creates a `MinionN.dat` file sized to the configured chunk (4 MiB by default).

### 2. Configure master

Edit [`framework/test/TestFramework.cpp`](framework/test/TestFramework.cpp) and set the IP/port of each minion.

### 3. Start master

```bash
sudo ./build/bin/masterFramework
```

### 4. Mount the virtual drive

In another terminal:

```bash
sudo modprobe nbd
echo 4 | sudo tee /sys/block/nbd*/queue/max_sectors_kb
sudo mkfs.ext2 /dev/nbd4
sudo mount /dev/nbd4 ~/iot_drive/mount/
```

> Default master uses `/dev/nbd4` in `TestFramework.cpp`. Adjust if needed.

---

## Tests

Regression tests (no NBD hardware required):

```bash
ctest --test-dir build --output-on-failure
```

| Test | Validates |
|------|-----------|
| `stripe_splitter` | Chunk-boundary splitting and mirror index mapping |
| `ticket_mirror` | RAID 01 read failover and write mirror policy |
| `tcp_eof` | TCP disconnect does not spin forever |
| `udp_receive` | UDP `ReceiveFrom` returns byte count |
| `thread_pool_shutdown` | Thread pool destructor joins workers |
| `dir_monitor_filter` | `.so` plugin filename filter |

---

## Project layout

```
IoTBasedRAID01/
├── CMakeLists.txt          # Root build
├── cmake/                  # Toolchain files
├── framework/              # Reactor, thread pool, scheduler, plugins
│   ├── include/
│   ├── src/
│   └── test/               # Master/minion entry points + framework tests
└── concrete/               # NBD, minions, RAID logic
    ├── include/
    ├── src/
    └── test/               # Concrete unit tests
```

---

## Main components

### Framework (reusable core)

- **Reactor** — monitors file descriptors and dispatches events
- **ThreadPool** — worker threads with priority task queue
- **Factory** — runtime object creation by key
- **AsyncInjection** — periodic condition polling via scheduler
- **Logger** — async file logging
- **DirMonitor / DLLLoader** — hot-load `.so` plugins from a directory
- **Handleton** — singleton manager (separate shared library)

### Concrete (IoT drive)

- **MinionManager** — stripes I/O across minions with RAID 01 mirroring
- **Ticket** — tracks primary/backup minion responses per request
- **NBD / NBDProxy** — kernel block device bridge
- **MinionProxy / MasterProxy** — UDP command channel
- **FileManager** — per-minion backing file

---

## Legacy manual build

Manual `g++` one-liners previously lived in `framework/bin/`. **CMake is the supported build path** going forward.

---
