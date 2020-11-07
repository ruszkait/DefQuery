cmake_minimum_required(VERSION 3.1)

project(DefQuery CXX)

include(cmake/conan.cmake)
conan_cmake_run(REQUIRES gtest/1.10.0
				BASIC_SETUP CMAKE_TARGETS
				BUILD missing)


set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# testing
option(ENABLE_TESTING "Enable test builds" ON)
if (ENABLE_TESTING)
set(PROJECT_TEST_NAME ${PROJECT_NAME}Test)

add_executable(${PROJECT_TEST_NAME})

target_sources(${PROJECT_TEST_NAME}
PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/test/AggregateTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/test/TakeWhileTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/test/VectorTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/test/WhereTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/test/AllTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/test/AnyTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/test/ContainsTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/test/CountTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/test/DecayTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/test/FirstTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/test/FromTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/test/GeneratorTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/test/GroupByTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/test/JoinTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/test/LifecycleTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/test/ListTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/test/OrderByTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/test/RangeTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/test/RegexpTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/test/ReverseTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/test/SelectManyTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/test/SelectTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/test/StlRangeTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/test/StoreTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/test/TakeTest.cpp
	)
	

	
target_link_libraries(${PROJECT_TEST_NAME} CONAN_PKG::gtest ${PROJECT_NAME})

enable_testing()

add_test(NAME ${PROJECT_TEST_NAME} COMMAND ${PROJECT_BINARY_DIR}/bin/${PROJECT_TEST_NAME})
endif()