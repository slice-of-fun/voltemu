# SPDX-FileCopyrightText: Copyright 2026 Eden Emulator Project
# SPDX-License-Identifier: GPL-3.0-or-later

add_executable(volt_room_standalone yuzu_room_standalone.cpp)

set_target_properties(volt_room_standalone PROPERTIES OUTPUT_NAME "volt-room")

target_link_libraries(volt_room_standalone PRIVATE volt-room)

if(UNIX AND NOT APPLE)
    install(TARGETS volt_room_standalone RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/bin")
endif()

if (VOLT_STATIC_ROOM)
    target_link_options(volt_room_standalone PRIVATE "-static")
endif()

create_target_directory_groups(volt_room_standalone)
