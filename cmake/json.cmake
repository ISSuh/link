include(${PROJECT_SOURCE_DIR}/cmake/fetching.cmake)

macro(fetch_thridparty thirdparty_dir download_module_path download_root)
  set(THIRD_PARTY_NAME json)
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

  file(GLOB JSON_INCLUDES
    "${download_root}/json-install/include/nlohmann/*.hpp"
  )

  file(COPY ${JSON_INCLUDES} DESTINATION ${thirdparty_dir}/json)
endmacro()
