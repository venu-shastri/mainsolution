#[policies CMakeLists.txt
find_package(Threads)
find_package(Execution)
find_library(TBB_LIBRARY NAMES tbb)
add_executable(Policies policies.cpp)
target_link_libraries(Policies PUBLIC std::execution Threads::Threads ${TBB_LIBRARIES})
#]


#[threads CMakeLists.txt
find_package(Threads)
add_executable(Multithreading multithreading.cpp)
target_link_libraries(Multithreading PUBLIC Threads::Threads)
#]

#[find_asio Find ASIO
find_package(Asio 1.21.0 QUIET)
if (NOT Asio_FOUND)
    FetchContent_Declare(asio GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git GIT_TAG asio-1-21-0)
    FetchContent_GetProperties(asio)
    if (NOT asio_POPULATED)
        FetchContent_Populate(asio)
        add_library(asio INTERFACE)
        target_include_directories(asio INTERFACE ${asio_SOURCE_DIR}/asio/include)
        target_compile_definitions(asio INTERFACE ASIO_STANDALONE ASIO_NO_DEPRECATED)
        target_link_libraries(asio INTERFACE Threads::Threads)
    endif ()
endif()
#]

# The executor libraries in ASIO do not have lots of convenience functions
# but they are a good reference for the executor proposal
#[executors CMakeLists.txt
add_executable(Executors executors.cpp)
target_link_libraries(Executors asio)
#]