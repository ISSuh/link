# the following code to fetch googletest
# is inspired by and adapted after https://crascit.com/2015/07/25/cmake-gtest/
# download and unpack googletest at configure time

macro(fetch_thridparty _third_party_dir _download_module_path _download_root)
  set(ASIO_DOWNLOAD_ROOT ${_download_root})
  configure_file(
    ${_download_module_path}/asio-fetch.cmake
    ${_download_root}/CMakeLists.txt
    @ONLY
  )
  unset(ASIO_DOWNLOAD_ROOT)

  execute_process(
    COMMAND
      "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
    WORKING_DIRECTORY
      ${_download_root}
  )

  execute_process(
    COMMAND
      "${CMAKE_COMMAND}" --build .
    WORKING_DIRECTORY
      ${_download_root}
  )

  file(GLOB ASIO_INCLUDES
    "${_download_root}/asio-install/include/*.hpp"
    "${_download_root}/asio-install/include/asio"
  )

  file(COPY ${ASIO_INCLUDES} DESTINATION ${_third_party_dir}/asio)

  include_directories(
    ${_third_party_dir}/asio
  )

  add_definitions(-DASIO_STANDALONE)
endmacro()
