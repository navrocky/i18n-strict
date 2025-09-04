set(TARGET i18n-strict-test)

find_package(Catch2 REQUIRED)

include_directories(${CMAKE_SOURCE_DIR})

set(SOURCES
    main.cpp
    translation_test.cpp
)

add_executable(${TARGET} ${SOURCES})

target_link_libraries(${TARGET} Catch2::Catch2)
