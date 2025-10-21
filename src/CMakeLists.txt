file(GLOB_RECURSE app_sources ${CMAKE_CURRENT_SOURCE_DIR}/*.c ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
list(FILTER app_sources EXCLUDE REGEX "*./examples/.*")
idf_component_register(
        SRCS ${app_sources}
        REQUIRES driver
)