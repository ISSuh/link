include(${PROJECT_SOURCE_DIR}/cmake/fetching.cmake)

macro(fetch_thridparty thirdparty_dir download_module_path download_root)
  set(THIRD_PARTY_NAME cpp-base64)
  set(THIRD_PARTY_DOWNLOAD_ROOT ${download_root})
  set(THIRD_PARTY_DOWNLOAD_SRC_DIR ${THIRD_PARTY_DOWNLOAD_ROOT}/${THIRD_PARTY_NAME}-src)
  set(THIRD_PARTY_DOWNLOAD_BUILD_DIR ${THIRD_PARTY_DOWNLOAD_ROOT}/${THIRD_PARTY_NAME}-build)
  set(THIRD_PARTY_DOWNLOAD_INSTALL_DIR ${THIRD_PARTY_DOWNLOAD_ROOT}/${THIRD_PARTY_NAME}-install)

  if(NOT EXISTS ${THIRD_PARTY_DOWNLOAD_INSTALL_DIR})
    fetcing(
      ${THIRD_PARTY_NAME}
      ${download_module_path}
      ${download_root}
    )
  endif()

  file(GLOB CPP_BASE64_INCLUDES
    "${THIRD_PARTY_DOWNLOAD_INSTALL_DIR}/include/*.h"
  )

  file(
    COPY
      ${CPP_BASE64_INCLUDES}
    DESTINATION
      ${thirdparty_dir}/${THIRD_PARTY_NAME}
  )

  include_directories(
    ${thirdparty_dir}/${THIRD_PARTY_NAME}
  )

  link_directories(
    ${THIRD_PARTY_DOWNLOAD_INSTALL_DIR}/lib
  )
endmacro()
