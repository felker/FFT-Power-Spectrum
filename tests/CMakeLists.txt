find_library(CPPUNIT REQUIRED)

add_executable(tests TestSuite.cpp TestCore.h TestCore.cpp)

target_link_libraries(tests PUBLIC ${PROJECT_NAME} ${LIBRARY} cppunit)