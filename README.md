=======
# IoT Based Drive

A Linux-based user-space program that provides a virtual drive, accessible via the native directory GUI. Although data appears to be stored locally, it is actually distributed across multiple IoT devices (referred to as *minions*) on the same local network.

---

## 🚀 Motivation

This project is designed to leverage unused storage space on common household IoT devices — such as smart air conditioner controllers, smart plugs, Raspberry Pi devices, and more.

- Data Accessible only within the Local Area Network (LAN)
- Implement RAID 01, therefore the system ensures thata data is:
  - **Secure**
  - **Backed up**
---

## 📚 Introduction

IoT-Based Drive is a user-space application for Linux systems. It allows users to mount a virtual drive using the standard `mount` shell command.

Data is transmitted via TCP from the kernel space to the user space using the Linux **Network Block Device (NBD)** module.

> 🛠 This project was developed as the final assignment in the Infinity Labs R&D Software Development Program.

---

## ✅ Prerequisites

- Linux OS
- Root access (for loading kernel modules and mounting NBD)

---

## ⚙️ Setup & Usage

```bash
cd framework/bin/
```

> Compiled executables will be in the `bin/` directory.

---

### 🔧 Minion Side

Each *minion* device must run its corresponding executable. To compile a minion, run the following:

```bash
g++ -std=c++23 -Wall -Wextra -g ../test/TestFirstMinion.cpp ../src/Framework.cpp ../src/dir_monitor.cpp ../src/AsyncInjection.cpp ../src/dll_loader.cpp ../src/FW_TPTask.cpp ../src/reactor.cpp ../src/thread_pool.cpp ../src/listener.cpp ../src/scheduler.cpp ../src/logger.cpp  ../../concrete/src/AMessage.cpp ../../concrete/src/ATaskArgs.cpp ../../concrete/src/FileManager.cpp ../../concrete/src/MasterProxy.cpp ../../concrete/src/MessageReceive.cpp ../../concrete/src/MessageSend.cpp ../../concrete/src/MinionArgs.cpp ../../concrete/src/MinionCommands.cpp ../../concrete/src/UdpSocket.cpp ../../concrete/src/UID.cpp -I ../include/ -I ../../concrete/include/ -L. -Wl,-rpath=. -lhandleton -ldl -o minion1
```
> Each minion has a corresponding test file, simply command again with ../test/TestSecondMinion.cpp, -o minion2 and so on.

### 👑 Master Side

1. Ensure all minions are up and running.
2. Edit `TestFramework.cpp` to include the IP and port of each minion device.
3. Compile the master -

```bash
g++ -std=c++23 -Wall -Wextra -g ../test/TestFramework.cpp ../src/Framework.cpp ../src/dir_monitor.cpp ../src/AsyncInjection.cpp ../src/dll_loader.cpp ../src/FW_TPTask.cpp ../src/reactor.cpp ../src/thread_pool.cpp ../src/listener.cpp ../src/scheduler.cpp ../src/logger.cpp ../../concrete/src/AMessage.cpp ../../concrete/src/ATaskArgs.cpp ../../concrete/src/Commands.cpp ../../concrete/src/MessageReceive.cpp ../../concrete/src/MessageSend.cpp ../../concrete/src/MinionManager.cpp ../../concrete/src/MinionProxy.cpp ../../concrete/src/NBDProxy.cpp ../../concrete/src/NBDReadArgs.cpp ../../concrete/src/NBDWriteArgs.cpp ../../concrete/src/Response.cpp ../../concrete/src/ResponseManager.cpp ../../concrete/src/Ticket.cpp ../../concrete/src/UdpSocket.cpp ../../concrete/src/TcpClientSocket.cpp ../../concrete/src/UID.cpp ../../concrete/src/NBD.cpp  -I ../include/ -I ../../concrete/include/  -L. -Wl,-rpath=. -lhandleton -ldl -o masterFramework
```

---

#### 🔌 Mounting the Drive

1. Load the NBD kernel module:

```bash
sudo modprobe nbd
echo 4 | sudo tee /sys/block/nbd*/queue/max_sectors_kb
```

2. In a **second terminal**, run the master executable:

```bash
sudo ./masterFramework
```

3. Back in the **first terminal**, format and mount the virtual drive:

```bash
sudo mkfs.ext2 /dev/nbd0
sudo mount /dev/nbd0 ~/iot_drive/mount/
```

> Your drive is now mounted. Any file operations inside `~/iot_drive/mount/` will be distributed across the minions.

---

## 🧩 Main Components

### 🧱 Framework (Reusable Core Modules)
#### 🔁 Reactor
Monitors file descriptors and triggers events when ready to read.

#### 🧵 ThreadPool
Handles multithreading via a waitable task queue and worker threads.
#### 🏭 Factory
Dynamically generates objects on-command, mainly being used in this project for creating read/write commands.
#### ⏳ Async Injection
Repeatedly checks a condition and self-destructs if met.
#### 📝 Logger
Writes system events and errors to a log file.

#### 📁 DirMonitor
Watches a directory for new Dynamic Linked Libraries (DLLs).

#### 🧬 DLLLoader
Loads DLLs at runtime using `dlopen()`.

#### 🧩 Handleton
A Singleton-like object manager with heap allocation for runtime flexibility.

##### Interfaces

- `IInputProxy` — declares `GetTaskArgs()`
- `ITaskArgs` — declares `GetKey()`
- `ICommand` — declares `Run()`

---

### 🔌 Concrete Components (IoT-Drive-Specific)
#### 📦 Minion Manager
Distributes read/write commands to minions and manages data sharding.

#### 🔁 Minion Proxy & Master Proxy
Facilitate communication between master and minions.

#### 🎫 Ticket
Tracks execution status of commands and routes results.

#### 📬 Response Manager
Handles responses and initiates appropriate follow-up actions.

#### 🔗 NBD
Handles Linux NBD communication between kernel and user space.

#### 📂 FileManager
Handles physical read/write operations on the minion devices.

---

