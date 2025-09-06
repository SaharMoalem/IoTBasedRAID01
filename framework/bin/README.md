# Compile IoT:

# create handleton.so:
gdppw23 -fPIC -shared ../src/handleton.cpp -I ../include/ -o libhandleton.so

# create master:
gdppw23 ../test/TestFramework.cpp ../src/Framework.cpp ../src/dir_monitor.cpp ../src/AsyncInjection.cpp ../src/dll_loader.cpp ../src/FW_TPTask.cpp ../src/reactor.cpp ../src/thread_pool.cpp ../src/listener.cpp ../src/scheduler.cpp ../src/logger.cpp ../../concrete/src/AMessage.cpp ../../concrete/src/ATaskArgs.cpp ../../concrete/src/Commands.cpp ../../concrete/src/MessageReceive.cpp ../../concrete/src/MessageSend.cpp ../../concrete/src/MinionManager.cpp ../../concrete/src/MinionProxy.cpp ../../concrete/src/NBDProxy.cpp ../../concrete/src/NBDReadArgs.cpp ../../concrete/src/NBDWriteArgs.cpp ../../concrete/src/Response.cpp ../../concrete/src/ResponseManager.cpp ../../concrete/src/Ticket.cpp ../../concrete/src/UdpSocket.cpp ../../concrete/src/TcpClientSocket.cpp ../../concrete/src/UID.cpp ../../concrete/src/NBD.cpp  -I ../include/ -I ../../concrete/include/  -L. -Wl,-rpath=. -lhandleton -ldl -o masterFramework

# create minions:
# compile minion framework:
gdppw23 ../test/TestFirstMinion.cpp ../src/Framework.cpp ../src/dir_monitor.cpp ../src/AsyncInjection.cpp ../src/dll_loader.cpp ../src/FW_TPTask.cpp ../src/reactor.cpp ../src/thread_pool.cpp ../src/listener.cpp ../src/scheduler.cpp ../src/logger.cpp  ../../concrete/src/AMessage.cpp ../../concrete/src/ATaskArgs.cpp ../../concrete/src/FileManager.cpp ../../concrete/src/MasterProxy.cpp ../../concrete/src/MessageReceive.cpp ../../concrete/src/MessageSend.cpp ../../concrete/src/MinionArgs.cpp ../../concrete/src/MinionCommands.cpp ../../concrete/src/UdpSocket.cpp ../../concrete/src/UID.cpp -I ../include/ -I ../../concrete/include/ -L. -Wl,-rpath=. -lhandleton -ldl -o minion1

gdppw23 ../test/TestSecondMinion.cpp ../src/Framework.cpp ../src/dir_monitor.cpp ../src/AsyncInjection.cpp ../src/dll_loader.cpp ../src/FW_TPTask.cpp ../src/reactor.cpp ../src/thread_pool.cpp ../src/listener.cpp ../src/scheduler.cpp ../src/logger.cpp  ../../concrete/src/AMessage.cpp ../../concrete/src/ATaskArgs.cpp ../../concrete/src/FileManager.cpp ../../concrete/src/MasterProxy.cpp ../../concrete/src/MessageReceive.cpp ../../concrete/src/MessageSend.cpp ../../concrete/src/MinionArgs.cpp ../../concrete/src/MinionCommands.cpp ../../concrete/src/UdpSocket.cpp ../../concrete/src/UID.cpp -I ../include/ -I ../../concrete/include/ -L. -Wl,-rpath=. -lhandleton -ldl -o minion2

gdppw23 ../test/TestThirdMinion.cpp ../src/Framework.cpp ../src/dir_monitor.cpp ../src/AsyncInjection.cpp ../src/dll_loader.cpp ../src/FW_TPTask.cpp ../src/reactor.cpp ../src/thread_pool.cpp ../src/listener.cpp ../src/scheduler.cpp ../src/logger.cpp  ../../concrete/src/AMessage.cpp ../../concrete/src/ATaskArgs.cpp ../../concrete/src/FileManager.cpp ../../concrete/src/MasterProxy.cpp ../../concrete/src/MessageReceive.cpp ../../concrete/src/MessageSend.cpp ../../concrete/src/MinionArgs.cpp ../../concrete/src/MinionCommands.cpp ../../concrete/src/UdpSocket.cpp ../../concrete/src/UID.cpp -I ../include/ -I ../../concrete/include/ -L. -Wl,-rpath=. -lhandleton -ldl -o minion3

# RASPBERRY:
# compile handleton so:
aarch64-linux-gnu-g++-12 -std=c++23 -pedantic-errors -Wall -Wextra -Werror -g -fPIC -shared ../src/handleton.cpp -I ../include -o librasphandleton.so

# compile minion on RASPBERRY:
aarch64-linux-gnu-g++-12 -std=c++23 -pedantic-errors -Wall -Wextra -Werror -g ../test/TestFirstMinion.cpp ../src/Framework.cpp ../src/dir_monitor.cpp ../src/AsyncInjection.cpp ../src/dll_loader.cpp ../src/FW_TPTask.cpp ../src/reactor.cpp ../src/thread_pool.cpp ../src/listener.cpp ../src/scheduler.cpp ../src/logger.cpp  ../../concrete/src/AMessage.cpp ../../concrete/src/ATaskArgs.cpp ../../concrete/src/FileManager.cpp ../../concrete/src/MasterProxy.cpp ../../concrete/src/MessageReceive.cpp ../../concrete/src/MessageSend.cpp ../../concrete/src/MinionArgs.cpp ../../concrete/src/MinionCommands.cpp ../../concrete/src/UdpSocket.cpp ../../concrete/src/UID.cpp -I ../include/ -I ../../concrete/include/ -L. -Wl,-rpath=. -lrasphandleton -ldl -o rasp_minion.out

# after running master and raspberry minion, mount:

sudo mkfs.ext2 /dev/nbd0
sudo mount /dev/nbd0 ~/nbd_test/mount/