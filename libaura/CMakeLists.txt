add_library(aura SHARED src/main.cpp src/utf8.cpp src/version.cpp)
find_package(CURL)
include_directories("${PROJECT_SOURCE_DIR}/libaura/include/libaura")
include_directories(${CURL_INCLUDE_DIRS})
target_link_libraries(aura ${CURL_LIBRARIES})
