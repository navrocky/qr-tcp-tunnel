set(TARGET qrtt-cli)

find_package(termcolor REQUIRED)
#find_package(yaml-cpp REQUIRED)
find_package(CLI11 REQUIRED)
find_package(ftxui REQUIRED)
find_package(qr-code-generator REQUIRED)
find_package(b64 REQUIRED)

if(BUILD_CLI_STATIC)
    add_link_options(-static -static-libgcc -static-libstdc++)
endif()

if(RELEASE_MODE)
    add_link_options(-s -Os)
endif()

include_directories(${CMAKE_CURRENT_BINARY_DIR})

configure_file(config.h.in config.h)

set(SOURCES
    main.cpp
)

add_executable(${TARGET} ${SOURCES})

target_link_libraries(${TARGET}
    termcolor::termcolor
    CLI11::CLI11
    ftxui::ftxui
    qr-code-generator::qr-code-generator
    b64::b64
)

include(GNUInstallDirs)
install(TARGETS ${TARGET}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
