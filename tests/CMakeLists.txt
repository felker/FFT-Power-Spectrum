find_package(CppUnit)

if (CppUnit_FOUND)
    add_executable(tests TestSuite.cpp TestCore.h TestCore.cpp)
    target_link_libraries(tests PUBLIC ${PROJECT_NAME} ${LIBRARY} cppunit)
endif ()