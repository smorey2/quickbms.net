set(VCPKG_LIBRARY_LINKAGE dynamic)

if(VCPKG_TARGET_ARCHITECTURE STREQUAL "x64")
    set(ARCH_PATH "win64")
elseif(VCPKG_TARGET_ARCHITECTURE STREQUAL "x86")
    set(ARCH_PATH "win32")
else()
    message(FATAL_ERROR "Package only supports x64 and x86 windows.")
endif()

if(VCPKG_CMAKE_SYSTEM_NAME)
    message(FATAL_ERROR "Package only supports windows desktop.")
endif()

vcpkg_download_distfile(ARCHIVE
    URLS "https://aluigi.altervista.org/papers/quickbms-src-0.10.1.zip"
    FILENAME "quickbms-src-0.10.1.zip"
    SHA512 f207859ca8f81ce115d2700be0932324b1ad51de7d0571d86fab2d34cc0b3a73c71270edfeb6f16f31962750e18a2ee8906b4a16c242be30db1abd336dd7d3e1
)
vcpkg_download_distfile(ARCHIVE_DEP
    URLS "http://aluigi.org/papers/bms/quickbms_dependencies.zip"
    FILENAME "quickbms_dependencies.zip"
    SHA512 6cc370c36913732ddb9d923d949efa42e66f6df92fe0dd7490e3aaecdf0428e2fbd9ead489e42935dcef55caee630e9e5a0c8a26921b35cca6fc2211329229b0
)

vcpkg_extract_source_archive_ex(
    OUT_SOURCE_PATH SOURCE_PATH
    ARCHIVE ${ARCHIVE}
    REF 0.10.1
    PATCHES
       001_quickbms.patch
       002_myenc.patch
)
vcpkg_extract_source_archive_ex(
    OUT_SOURCE_PATH SOURCE_PATH_DEP
    ARCHIVE ${ARCHIVE_DEP}
    REF 0.0.0
    NO_REMOVE_ONE_LEVEL
    WORKING_DIRECTORY ${SOURCE_PATH}
)
file(INSTALL ${CURRENT_PORT_DIR}/CMakeLists.txt DESTINATION ${SOURCE_PATH})

vcpkg_configure_cmake(
    SOURCE_PATH ${SOURCE_PATH}
    PREFER_NINJA
    OPTIONS -D CMAKE_C_COMPILER=gcc -D CMAKE_CXX_COMPILER=g++
)

vcpkg_install_cmake()

# Handle copyright
file(INSTALL ${SOURCE_PATH}/gpl-2.0.txt DESTINATION ${CURRENT_PACKAGES_DIR}/share/${PORT} RENAME copyright)

#file(MAKE_DIRECTORY
#    ${CURRENT_PACKAGES_DIR}/lib
#    ${CURRENT_PACKAGES_DIR}/bin
#)