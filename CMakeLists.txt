project(tox_extension_sender_timestamp)

find_package(PkgConfig REQUIRED)

pkg_search_module(TOXCORE REQUIRED toxcore)

add_library(tox_extension_sender_timestamp tox_extension_sender_timestamp.c)
target_compile_options(tox_extension_sender_timestamp PRIVATE -Wall -Wextra -pedantic -Werror -std=c11)
target_link_libraries(tox_extension_sender_timestamp ${TOXCORE_LIBRARIES})
set_target_properties(tox_extension_sender_timestamp PROPERTIES PUBLIC_HEADER "tox_extension_sender_timestamp.h")
target_include_directories(tox_extension_sender_timestamp PRIVATE "${TOXCORE_INCLUDEDIR}")

install(TARGETS tox_extension_sender_timestamp ARCHIVE DESTINATION lib PUBLIC_HEADER DESTINATION include)
