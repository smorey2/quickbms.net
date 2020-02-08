vcpkg_download_distfile(ARCHIVE
    URLS "https://aluigi.altervista.org/papers/quickbms_src.zip"
    FILENAME "quickbms-current.zip"
    SHA512 f207859ca8f81ce115d2700be0932324b1ad51de7d0571d86fab2d34cc0b3a73c71270edfeb6f16f31962750e18a2ee8906b4a16c242be30db1abd336dd7d3e1
)

vcpkg_extract_source_archive_ex(
    OUT_SOURCE_PATH SOURCE_PATH
    ARCHIVE ${ARCHIVE}
    REF 0.10.1
    PATCHES
       001_quickbms.patch
)
file(INSTALL ${CURRENT_PORT_DIR}/CMakeLists.txt DESTINATION ${SOURCE_PATH})

vcpkg_configure_cmake(
    SOURCE_PATH ${SOURCE_PATH}
    PREFER_NINJA
    OPTIONS
        -D CMAKE_C_COMPILER=gcc -D CMAKE_CXX_COMPILER=g++
)

vcpkg_install_cmake()

# Handle copyright
file(INSTALL ${SOURCE_PATH}/gpl-2.0.txt DESTINATION ${CURRENT_PACKAGES_DIR}/share/${PORT} RENAME copyright)
