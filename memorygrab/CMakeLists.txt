project(memorygrab)

find_package(fmt)
add_executable(memorygrab memorygrab.cpp)
target_link_libraries(memorygrab fmt::fmt)

install(TARGETS memorygrab
    CONFIGURATIONS Release
    RUNTIME_DESTINATION bin)
