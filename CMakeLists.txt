cmake_minimum_required(VERSION 3.18)

project(kcuckounter)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_TESTS "Build unit tests" ON)
option(COVERAGE "Enable coverage reporting" OFF)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(
        -Og -g -fno-omit-frame-pointer
#        -fsanitize=address,undefined,leak
        -Werror
        -Wall
        -Wextra
        -Wpedantic
        -Wcast-align
        -Wcast-qual
        -Wconversion
        -Wctor-dtor-privacy
        -Wenum-compare
        -Wfloat-equal
        -Wnon-virtual-dtor
        -Wold-style-cast
        -Woverloaded-virtual
        -Wredundant-decls
        -Wsign-conversion
        -Wsign-promo
    )
#    add_link_options(
#            -fsanitize=address,undefined,leak
#    )
elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -mtune=native -DNDEBUG")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s")
endif ()

if (COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(--coverage -O0)
    add_link_options(--coverage)
endif ()

set(QT_MIN_VERSION "6.0.0")
set(KF6_MIN_VERSION "6.0.0")

find_package(ECM 1.0.0 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(FeatureSummary)

find_package(Qt6 ${QT_MIN_VERSION} CONFIG REQUIRED
        COMPONENTS Core Widgets Svg Quick Test
)
find_package(KF6 ${KF6_MIN_VERSION} REQUIRED COMPONENTS
        CoreAddons I18n XmlGui ConfigWidgets WidgetsAddons KIO
)

find_package(KDEGames6 REQUIRED)

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(kcuckounter_HEADERS
        include/mainwindow.hpp
        include/table/table.hpp
        include/table/tableslot.hpp
        include/settings.hpp
        include/strategy/strategyinfo.hpp
        include/strategy/strategy.hpp
        include/widgets/cards.hpp
        include/widgets/carousel.hpp
        include/widgets/base/label.hpp
        include/widgets/base/frame.hpp)

set(kcuckounter_SOURCES
        src/mainwindow.cpp
        src/table/table.cpp
        src/table/tableslot.cpp
        src/settings.cpp
        src/strategy/strategyinfo.cpp
        src/strategy/strategy.cpp
        src/widgets/carousel.cpp
        src/widgets/cards.cpp
        src/widgets/base/label.cpp
        src/widgets/base/frame.cpp)

add_library(kcuckounter_lib STATIC
        ${kcuckounter_HEADERS}
        ${kcuckounter_SOURCES}
        ${kcuckounter_resources})


target_link_libraries(kcuckounter_lib
  PUBLIC
    Qt6::Widgets
    Qt6::Svg
    KF6::CoreAddons
    KF6::I18n
    KF6::XmlGui
    KF6::ConfigWidgets
    KF6::WidgetsAddons
    KF6::KIOCore
    KDEGames6
)

target_include_directories(kcuckounter_lib PUBLIC include)

set_target_properties(kcuckounter_lib PROPERTIES UNITY_BUILD OFF)

add_executable(kcuckounter src/main.cpp)

target_link_libraries(kcuckounter PRIVATE
        kcuckounter_lib)

option(BUILD_TESTS "Build unit tests" ON)
option(COVERAGE "Enable coverage reporting" OFF)

install(TARGETS kcuckounter ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})

if (BUILD_TESTS)
    enable_testing()

    qt_add_executable(unit_tests
            tests/main.cpp
            tests/test_cards.cpp tests/test_strategy.cpp tests/test_table.cpp tests/test_mainwindow.cpp
)
    set_source_files_properties(
            tests/test_cards.cpp tests/test_strategy.cpp tests/test_table.cpp tests/test_mainwindow.cpp
            PROPERTIES HEADER_FILE_ONLY ON)
    target_link_libraries(unit_tests PRIVATE kcuckounter_lib Qt6::Test)
    add_test(NAME unit_tests COMMAND unit_tests)
#    include(CTest)
#    set_tests_properties(unit_tests PROPERTIES ENVIRONMENT "ASAN_OPTIONS=detect_leaks=1")
#
#    find_program(VALGRIND_EXECUTABLE valgrind)
#    if (VALGRIND_EXECUTABLE)
#        add_custom_target(memcheck
#                COMMAND ${VALGRIND_EXECUTABLE} --leak-check=full --track-origins=yes --error-exitcode=1 $<TARGET_FILE:unit_tests>
#                DEPENDS unit_tests
#        )
#    endif ()

    if (COVERAGE)
        message(STATUS "Coverage: ON, compiler is ${CMAKE_CXX_COMPILER_ID}")

        if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
            set(COV_FLAGS --coverage -O0)
            foreach (tgt IN ITEMS kcuckounter_lib unit_tests)
                target_compile_options(${tgt} PUBLIC ${COV_FLAGS})
                target_link_libraries(${tgt} PUBLIC gcov)
            endforeach ()

            # custom target to run gcov/lcov
            find_program(LCOV lcov)
            find_program(GENHTML genhtml)
            add_custom_target(coverage
                    COMMENT "Generating GCC/gcov coverage report"
                    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                    COMMAND ${CMAKE_COMMAND} --build . --target unit_tests
                    COMMAND unit_tests
                    COMMAND ${LCOV} --capture --directory . --output-file coverage.info
                    COMMAND ${GENHTML} coverage.info --output-directory coverage_report
                    DEPENDS unit_tests
            )

        elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
#            find_program(LLVM_PROFDATA llvm-profdata REQUIRED)
#            find_program(LLVM_COV llvm-cov REQUIRED)

            set(COV_FLAGS
                    -fprofile-instr-generate
                    -fcoverage-mapping
                    -O0
            )
            set(PROFRAW_FILE "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.profraw")
            set(PROFDATA_FILE "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.profdata")
            set(COVERAGE_REPORT_DIR "${CMAKE_BINARY_DIR}/../cov")

            foreach (tgt IN ITEMS kcuckounter_lib unit_tests)
                target_compile_options(${tgt} PUBLIC ${COV_FLAGS})
                target_link_options(${tgt} PUBLIC -fprofile-instr-generate)
            endforeach ()


            find_program(LLVM_PROFDATA llvm-profdata)
            find_program(LLVM_COV llvm-cov)
            add_custom_target(coverage
                    BYPRODUCTS ${PROFRAW_FILE} ${PROFDATA_FILE}
                    COMMENT "Generating Clang/LLVM-cov coverage report"
                    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                    COMMAND ${CMAKE_COMMAND} --build . --target unit_tests
                    COMMAND LLVM_PROFILE_FILE=${PROFRAW_FILE} $<TARGET_FILE:unit_tests>
                    COMMAND ${LLVM_PROFDATA} merge -sparse -output=${PROFDATA_FILE} ${PROFRAW_FILE}
                    COMMAND ${LLVM_COV} show $<TARGET_FILE:unit_tests>
                    -instr-profile=${PROFDATA_FILE}
                    -format=html
                    -output-dir=${COVERAGE_REPORT_DIR}
                    -ignore-filename-regex='.*/\(tests|build|include\)/.*'
                    -show-branches=count
                    -show-line-counts
                    -show-regions
                    -show-instantiations
                    -show-expansions
                    -use-color
                    -coverage-watermark=90,60
                    COMMAND ${LLVM_COV} export $<TARGET_FILE:unit_tests>
                    -instr-profile=${PROFDATA_FILE}
                    -format=lcov
                    -ignore-filename-regex='.*/\(tests|build|include\)*/.*' > ${CMAKE_BINARY_DIR}/coverage.info
                    DEPENDS unit_tests kcuckounter_lib
            )
        else ()
            message(WARNING "Coverage is only supported with GCC or Clang")
        endif ()
    endif ()
endif ()
