find_package(GTest REQUIRED)

set(UTEST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/quote.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

add_executable(${UTEST_EXEC} ${UTEST_SOURCES})

target_include_directories(${UTEST_EXEC} PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_include_directories(${UTEST_EXEC} PRIVATE ${GTEST_INCLUDE_DIRS})
target_link_libraries(${UTEST_EXEC} PRIVATE ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES})
install(TARGETS ${UTEST_EXEC} DESTINATION ${CMAKE_SOURCE_DIR}/bin)
