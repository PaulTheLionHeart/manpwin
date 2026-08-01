# =========================================================
# FindGMP.cmake
#
# Finds the GMP library installed by vcpkg.
#
# The current vcpkg GMP port does not provide a CMake package
# configuration file (GMPConfig.cmake). This module creates a
# modern imported target from the standard vcpkg directory layout,
# allowing correct Debug/Release library selection with Visual
# Studio multi-configuration generators.
#
# Defines:
#
#   GMP_FOUND
#   GMP_INCLUDE_DIR
#
# Creates imported target:
#
#   GMP::GMP
#
# =========================================================

find_path(GMP_INCLUDE_DIR
    NAMES gmp.h
)

if(GMP_INCLUDE_DIR)

    # include -> x64-windows-static
    get_filename_component(GMP_ROOT
        "${GMP_INCLUDE_DIR}"
        DIRECTORY
    )

    set(GMP_LIBRARY_RELEASE
        "${GMP_ROOT}/lib/gmp.lib"
    )

    set(GMP_LIBRARY_DEBUG
        "${GMP_ROOT}/debug/lib/gmp.lib"
    )

endif()

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(GMP
    REQUIRED_VARS
        GMP_INCLUDE_DIR
        GMP_LIBRARY_RELEASE
        GMP_LIBRARY_DEBUG
)

if(GMP_FOUND AND NOT TARGET GMP::GMP)

    add_library(GMP::GMP UNKNOWN IMPORTED)

set_target_properties(GMP::GMP PROPERTIES

    IMPORTED_CONFIGURATIONS
        "Debug;Release;RelWithDebInfo;MinSizeRel"

    IMPORTED_LOCATION_DEBUG
        "${GMP_LIBRARY_DEBUG}"

    IMPORTED_LOCATION_RELEASE
        "${GMP_LIBRARY_RELEASE}"

    IMPORTED_LOCATION_RELWITHDEBINFO
        "${GMP_LIBRARY_RELEASE}"

    IMPORTED_LOCATION_MINSIZEREL
        "${GMP_LIBRARY_RELEASE}"

    INTERFACE_INCLUDE_DIRECTORIES
        "${GMP_INCLUDE_DIR}"
)

endif()