# =========================================================
# FindMPFR.cmake
#
# Finds the MPFR library installed by vcpkg.
#
# The current vcpkg MPFR port does not provide a CMake package
# configuration file (MPFRConfig.cmake). This module creates a
# modern imported target from the standard vcpkg directory layout,
# allowing correct Debug/Release library selection with Visual
# Studio multi-configuration generators.
#
# Defines:
#
#   MPFR_FOUND
#   MPFR_INCLUDE_DIR
#
# Creates imported target:
#
#   MPFR::MPFR
#
# =========================================================

find_path(MPFR_INCLUDE_DIR
    NAMES mpfr.h
)

if(MPFR_INCLUDE_DIR)

    # include -> x64-windows-static
    get_filename_component(MPFR_ROOT
        "${MPFR_INCLUDE_DIR}"
        DIRECTORY
    )

    set(MPFR_LIBRARY_RELEASE
        "${MPFR_ROOT}/lib/mpfr.lib"
    )

    set(MPFR_LIBRARY_DEBUG
        "${MPFR_ROOT}/debug/lib/mpfr.lib"
    )

endif()

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(MPFR
    REQUIRED_VARS
        MPFR_INCLUDE_DIR
        MPFR_LIBRARY_RELEASE
        MPFR_LIBRARY_DEBUG
)

if(MPFR_FOUND AND NOT TARGET MPFR::MPFR)

    add_library(MPFR::MPFR UNKNOWN IMPORTED)

set_target_properties(MPFR::MPFR PROPERTIES

    IMPORTED_CONFIGURATIONS
        "Debug;Release;RelWithDebInfo;MinSizeRel"

    IMPORTED_LOCATION_DEBUG
        "${MPFR_LIBRARY_DEBUG}"

    IMPORTED_LOCATION_RELEASE
        "${MPFR_LIBRARY_RELEASE}"

    IMPORTED_LOCATION_RELWITHDEBINFO
        "${MPFR_LIBRARY_RELEASE}"

    IMPORTED_LOCATION_MINSIZEREL
        "${MPFR_LIBRARY_RELEASE}"

    INTERFACE_INCLUDE_DIRECTORIES
        "${MPFR_INCLUDE_DIR}"
)

endif()