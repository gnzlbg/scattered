# - Try to find pnetcdf
# Once done this will define
#  LIBCXX_FOUND - System has LibXml2

find_package(PkgConfig)

find_path(LIBCXX_INCLUDE_DIR algorithm
          HINTS ${PC_LIBCXX_INCLUDEDIR})

find_library(LIBCXX_LIBRARY NAMES algorithm
             HINTS ${PC_LIBCXX_LIBDIR})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LIBCXX DEFAULT_MSG
                                  LIBCXX_LIBRARY LIBCXX_INCLUDE_DIR)

mark_as_advanced(LIBCXX_INCLUDE_DIR LIBCXX_LIBRARY)
