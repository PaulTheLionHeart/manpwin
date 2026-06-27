@echo off

echo ========================================
echo Cleaning previous build (optional)...
echo ========================================

if exist build_release (
    echo Removing old build_release folder...
    rmdir /s /q build_release
)

echo ========================================
echo Configuring ManpWIN (RELEASE) with vcpkg...
echo ========================================

cmake -B build_release -S . ^
  -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake ^
  -DVCPKG_TARGET_TRIPLET=x64-windows-static

if %errorlevel% neq 0 (
    echo CONFIGURE FAILED
    pause
    exit /b %errorlevel%
)

echo ========================================
echo Building ManpWIN (RELEASE)...
echo ========================================

cmake --build build_release --config Release

if %errorlevel% neq 0 (
    echo BUILD FAILED
    pause
    exit /b %errorlevel%
)

echo ========================================
echo RELEASE BUILD SUCCESS!
echo ========================================
pause

