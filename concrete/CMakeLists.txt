set(CONCRETE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(FRAMEWORK_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/framework/include)

set(CONCRETE_COMMON_SOURCES
    src/AMessage.cpp
    src/ATaskArgs.cpp
    src/MessageReceive.cpp
    src/MessageSend.cpp
    src/TcpClientSocket.cpp
    src/UdpSocket.cpp
    src/UID.cpp
)

set(CONCRETE_MINION_SOURCES
    src/FileManager.cpp
    src/MasterProxy.cpp
    src/MinionArgs.cpp
    src/MinionCommands.cpp
)

set(CONCRETE_MASTER_SOURCES
    src/Commands.cpp
    src/MinionManager.cpp
    src/MinionProxy.cpp
    src/NBD.cpp
    src/NBDProxy.cpp
    src/NBDReadArgs.cpp
    src/NBDWriteArgs.cpp
    src/Response.cpp
    src/ResponseManager.cpp
    src/StripeSplitter.cpp
    src/Ticket.cpp
)

add_library(concrete_common STATIC ${CONCRETE_COMMON_SOURCES})
target_include_directories(concrete_common PUBLIC
    ${CONCRETE_INCLUDE_DIR}
    ${FRAMEWORK_INCLUDE_DIR}
)
target_link_libraries(concrete_common PUBLIC pthread)

add_library(concrete_minion STATIC ${CONCRETE_MINION_SOURCES})
target_include_directories(concrete_minion PUBLIC
    ${CONCRETE_INCLUDE_DIR}
    ${FRAMEWORK_INCLUDE_DIR}
)
target_link_libraries(concrete_minion PUBLIC concrete_common)

add_library(concrete_master STATIC ${CONCRETE_MASTER_SOURCES})
target_include_directories(concrete_master PUBLIC
    ${CONCRETE_INCLUDE_DIR}
    ${FRAMEWORK_INCLUDE_DIR}
)
target_link_libraries(concrete_master PUBLIC concrete_common)

add_executable(masterFramework
    ${CMAKE_SOURCE_DIR}/framework/test/TestFramework.cpp
)
target_include_directories(masterFramework PRIVATE
    ${CONCRETE_INCLUDE_DIR}
    ${FRAMEWORK_INCLUDE_DIR}
)
target_link_libraries(masterFramework PRIVATE
    framework_core concrete_master concrete_common handleton dl pthread
)
set_target_properties(masterFramework PROPERTIES
    BUILD_RPATH "\$ORIGIN/../lib"
    INSTALL_RPATH "\$ORIGIN/../lib"
)

add_executable(minion1 ${CMAKE_SOURCE_DIR}/framework/test/TestFirstMinion.cpp)
add_executable(minion2 ${CMAKE_SOURCE_DIR}/framework/test/TestSecondMinion.cpp)
add_executable(minion3 ${CMAKE_SOURCE_DIR}/framework/test/TestThirdMinion.cpp)

foreach(minion_target minion1 minion2 minion3)
    target_include_directories(${minion_target} PRIVATE
        ${CONCRETE_INCLUDE_DIR}
        ${FRAMEWORK_INCLUDE_DIR}
    )
    target_link_libraries(${minion_target} PRIVATE
        framework_core concrete_minion concrete_common handleton dl pthread
    )
    set_target_properties(${minion_target} PROPERTIES
        BUILD_RPATH "\$ORIGIN/../lib"
        INSTALL_RPATH "\$ORIGIN/../lib"
    )
endforeach()

if(BUILD_TESTS)
    add_executable(test_stripe_splitter test/test_stripe_splitter.cpp)
    target_include_directories(test_stripe_splitter PRIVATE
        ${CONCRETE_INCLUDE_DIR} ${FRAMEWORK_INCLUDE_DIR})
    target_link_libraries(test_stripe_splitter PRIVATE concrete_master concrete_common pthread)
    add_test(NAME stripe_splitter COMMAND test_stripe_splitter)

    add_executable(test_ticket test/test_ticket.cpp)
    target_include_directories(test_ticket PRIVATE
        ${CONCRETE_INCLUDE_DIR} ${FRAMEWORK_INCLUDE_DIR})
    target_link_libraries(test_ticket PRIVATE concrete_master concrete_common pthread)
    add_test(NAME ticket_mirror COMMAND test_ticket)

    add_executable(test_tcp_eof test/test_tcp_eof.cpp)
    target_include_directories(test_tcp_eof PRIVATE ${CONCRETE_INCLUDE_DIR})
    target_link_libraries(test_tcp_eof PRIVATE concrete_common pthread)
    add_test(NAME tcp_eof COMMAND test_tcp_eof)

    add_executable(test_udp_socket test/test_udp_socket.cpp)
    target_include_directories(test_udp_socket PRIVATE ${CONCRETE_INCLUDE_DIR})
    target_link_libraries(test_udp_socket PRIVATE concrete_common pthread)
    add_test(NAME udp_receive COMMAND test_udp_socket)
endif()
