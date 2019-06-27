project(tox_extension_sender_timestamp)

if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)


find_package(ToxExt REQUIRED)

add_library(ToxExtensionSenderTimestamp tox_extension_sender_timestamp.c)
target_compile_options(ToxExtensionSenderTimestamp PRIVATE -Wall -Wextra -Werror -std=gnu11)
target_link_libraries(ToxExtensionSenderTimestamp ToxExt::ToxExt)
set_target_properties(ToxExtensionSenderTimestamp PROPERTIES PUBLIC_HEADER "tox_extension_sender_timestamp.h")
target_include_directories(ToxExtensionSenderTimestamp PRIVATE "${TOXCORE_INCLUDEDIR}")
set_target_properties(ToxExtensionSenderTimestamp PROPERTIES OUTPUT_NAME "tox_extension_sender_timestamp")


install(TARGETS ToxExtensionSenderTimestamp EXPORT ToxExtensionSenderTimestampConfig ARCHIVE DESTINATION lib PUBLIC_HEADER DESTINATION include)
install(EXPORT ToxExtensionSenderTimestampConfig DESTINATION lib/cmake/ToxExtensionSenderTimestamp NAMESPACE ToxExtensionSenderTimestamp::)
