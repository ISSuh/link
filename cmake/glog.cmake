# the following code to fetch googletest
# is inspired by and adapted after https://crascit.com/2015/07/25/cmake-gtest/
# download and unpack googletest at configure time

macro(fetch_glog _download_module_path _download_root)
    set(GLOG_DOWNLOAD_ROOT ${_download_root})
    configure_file(
        ${_download_module_path}/glog-download.cmake
        ${_download_root}/CMakeLists.txt
        @ONLY
        )
    unset(GLOG_DOWNLOAD_ROOT)

    execute_process(
        COMMAND
            "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
        WORKING_DIRECTORY
            ${_download_root}
        )
    execute_process(
        COMMAND
            "${CMAKE_COMMAND}" --build . --target install
        WORKING_DIRECTORY
            ${_download_root}
        )

    # adds the targers: glogl
    add_subdirectory(
        ${_download_root}/glog-src
        ${_download_root}/glog-build
        )
endmacro()
