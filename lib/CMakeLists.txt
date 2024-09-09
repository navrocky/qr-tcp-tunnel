set(TARGET qrtt-lib)

# find_package(termcolor REQUIRED)
#find_package(yaml-cpp REQUIRED)
# find_package(CLI11 REQUIRED)
# find_package(ftxui REQUIRED)
find_package(qr-code-generator REQUIRED)
find_package(b64 REQUIRED)

# if(BUILD_CLI_STATIC)
#     add_link_options(-static -static-libgcc -static-libstdc++)
# endif()

if(RELEASE_MODE)
    add_link_options(-s -Os)
endif()

set(SOURCES
    main.cpp
)

set(DEPS
    qr-code-generator::qr-code-generator
    b64::b64
)

add_library(${TARGET} ${SOURCES})

target_link_libraries(${TARGET}
    qr-code-generator::qr-code-generator
    b64::b64
)
