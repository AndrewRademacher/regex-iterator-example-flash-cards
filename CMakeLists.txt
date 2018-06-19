cmake_minimum_required(VERSION 3.10)
project(flash_cards)

set(CMAKE_CXX_STANDARD 17)

if(APPLE)
    set(CMAKE_LIBRARY_PATH /usr/local/lib)
    include_directories(/usr/local/include)
endif(APPLE)

find_library(LIB_BOOST_IOSTREAMS boost_iostreams REQUIRED)
find_library(LIB_BOOST_PROGRAM_OPTIONS boost_program_options REQUIRED)

add_subdirectory(vendor/catch)
add_subdirectory(vendor/nlohmann-json)

add_library(cards SHARED src/question.cpp include/question.h)
target_include_directories(cards PUBLIC include)
target_link_libraries(cards PUBLIC nlohmann-json
        ${LIB_BOOST_IOSTREAMS})

add_executable(test_cards test/main.cpp test/identifier_matching.cpp test/mapped_file_iterator.cpp)
target_link_libraries(test_cards cards catch)
add_test(tcards test_cards)

add_executable(flash src/main.cpp)
target_link_libraries(flash cards
        ${LIB_BOOST_PROGRAM_OPTIONS})