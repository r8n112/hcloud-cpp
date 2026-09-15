include(FetchContent)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.7.1
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(Catch2)

add_executable(client_test client_test.cpp)
target_link_libraries(client_test PRIVATE hcloud::hcloud_cpp Catch2::Catch2WithMain)

r8n112_enable_warnings(client_test)
r8n112_enable_sanitizers(client_test)

list(APPEND CMAKE_MODULE_PATH "${catch2_SOURCE_DIR}/extras")
include(Catch)
catch_discover_tests(client_test)
