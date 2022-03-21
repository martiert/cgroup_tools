project(cpuburn)

find_package(fmt)
find_package(Threads)
add_executable(cpuburn cpuburn.cpp)
target_link_libraries(cpuburn fmt::fmt ${CMAKE_THREAD_LIBS_INIT})


install(TARGETS cpuburn
    CONFIGURATIONS Release
    RUNTIME_DESTINATION bin)
