cmake_minimum_required(VERSION 3.8.0)
project(quickbms)

# WINDOWS
if(WIN32)
   set(VCPKG ${CMAKE_SOURCE_DIR}/_vcpkg)

   # install vcpkg
   if (NOT IS_DIRECTORY ${VCPKG})
      message("vcpkg does not exist. installing...")
      execute_process(
         COMMAND git.exe clone https://github.com/Microsoft/vcpkg.git ${VCPKG}
         WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
      )
      execute_process(
         COMMAND bootstrap-vcpkg.bat -disableMetrics
         WORKING_DIRECTORY ${VCPKG}
      )
   endif()

   # install quickbms:x64-windows
   if (NOT IS_DIRECTORY ${VCPKG}/packages/quickbms_x64-windows)
      execute_process(
         COMMAND vcpkg install quickbms:x64-windows --overlay-ports=../custom-ports
         WORKING_DIRECTORY ${VCPKG}
      )
   endif()

endif()

add_library(quickbms SHARED empty.cc ${VCPKG}/packages/quickbms_${BITNESS}-windows/lib/quickbms.def)
target_link_libraries(quickbms ${VCPKG}/packages/quickbms_${BITNESS}-windows/lib/quickbms.lib)