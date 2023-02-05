cmake_minimum_required (VERSION 3.8)

set(CMAKE_TOOLCHAIN_FILE "/vcpkg/scripts/buildsystems/vcpkg.cmake")

project ("main")

add_executable (main "main.cpp"  "bookingsystem.cpp" "bookingsystem.h" "customer.h" "contract.cpp" "contract.h" "location.h" "filehandling.cpp" "filehandling.h")

set(CMAKE_PREFIX_PATH "/vcpkg/installed/x64-windows/share")
find_package(jsoncpp CONFIG REQUIRED)
target_link_libraries(main PRIVATE jsoncpp_lib jsoncpp_object JsonCpp::JsonCpp)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET main PROPERTY CXX_STANDARD 20)
endif()
