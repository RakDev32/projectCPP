cmake_minimum_required(VERSION 3.20)
project(InfectiousGraphCells)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Πού θα βρει τα headers
include_directories(include)
include_directories(sgg_lib/include)

# 2. Μάζεψε τα αρχεία σου
file(GLOB SOURCES "src/*.cpp" "main.cpp")
list(APPEND SOURCES "src/DroppedPellet.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})

# 3. LINKING: Εδώ γίνεται το μάγια για Windows/Linux
if(WIN32)
    # Για Windows (Visual Studio)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/sgg_lib/lib/sggd.lib)
else()
    # Για Linux (Laptop)
    target_link_libraries(${PROJECT_NAME} PRIVATE 
        ${CMAKE_SOURCE_DIR}/sgg_lib/lib/libsgg.a
        GL GLEW SDL2 SDL2_mixer SDL2_image freetype pthread
    )
endif()

# 4. Αυτόματη αντιγραφή των Assets στο φάκελο που τρέχει το .exe
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
)
