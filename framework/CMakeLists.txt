set(FRAMEWORK_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(handleton SHARED src/handleton.cpp)
target_include_directories(handleton PUBLIC ${FRAMEWORK_INCLUDE_DIR})
set_target_properties(handleton PROPERTIES OUTPUT_NAME handleton)

set(FRAMEWORK_CORE_SOURCES
    src/AsyncInjection.cpp
    src/dir_monitor.cpp
    src/dll_loader.cpp
    src/Framework.cpp
    src/FW_TPTask.cpp
    src/listener.cpp
    src/logger.cpp
    src/reactor.cpp
    src/scheduler.cpp
    src/thread_pool.cpp
)

add_library(framework_core STATIC ${FRAMEWORK_CORE_SOURCES})
target_include_directories(framework_core PUBLIC ${FRAMEWORK_INCLUDE_DIR})
target_link_libraries(framework_core PUBLIC pthread dl)

if(BUILD_TESTS)
    add_executable(test_thread_pool_shutdown test/test_thread_pool_shutdown.cpp)
    target_include_directories(test_thread_pool_shutdown PRIVATE ${FRAMEWORK_INCLUDE_DIR})
    target_link_libraries(test_thread_pool_shutdown PRIVATE framework_core pthread)
    add_test(NAME thread_pool_shutdown COMMAND test_thread_pool_shutdown)

    add_executable(test_dir_monitor_filter test/test_dir_monitor_filter.cpp)
    target_include_directories(test_dir_monitor_filter PRIVATE ${FRAMEWORK_INCLUDE_DIR})
    target_link_libraries(test_dir_monitor_filter PRIVATE framework_core pthread)
    add_test(NAME dir_monitor_filter COMMAND test_dir_monitor_filter)

    add_executable(test_thread_pool test/test_thread_pool.cpp)
    target_include_directories(test_thread_pool PRIVATE ${FRAMEWORK_INCLUDE_DIR})
    target_link_libraries(test_thread_pool PRIVATE framework_core pthread)
    set_tests_properties(test_thread_pool PROPERTIES DISABLED TRUE)
endif()
