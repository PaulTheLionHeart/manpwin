@echo off
echo ========================================
echo Cleaning previous build (optional)...
echo ========================================

if exist build (
    echo Removing old build folder...
    rmdir /s /q build
)

echo ========================================
echo Configuring ManpWIN (DEBUG) with vcpkg...
echo ========================================

cmake -B build_debug -S . ^
  -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake ^
  -DVCPKG_TARGET_TRIPLET=x64-windows

if %errorlevel% neq 0 (
    echo CONFIGURE FAILED
    pause
    exit /b %errorlevel%
)

echo ========================================
echo Building ManpWIN (DEBUG)...
echo ========================================

cmake --build build_debug --config Debug

if %errorlevel% neq 0 (
    echo BUILD FAILED
    pause
    exit /b %errorlevel%
)

echo ========================================
echo DEBUG BUILD SUCCESS!
echo ========================================
pause