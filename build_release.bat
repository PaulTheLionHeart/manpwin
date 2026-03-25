@echo off
echo ========================================
echo Configuring ManpWIN with vcpkg...
echo ========================================

cmake -B build_release -S . ^
  -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake ^
  -DVCPKG_TARGET_TRIPLET=x64-windows

if %errorlevel% neq 0 (
    echo CONFIGURE FAILED
    pause
    exit /b %errorlevel%
)

echo ========================================
echo Building ManpWIN...
echo ========================================

cmake --build build_release --config Release

if %errorlevel% neq 0 (
    echo BUILD FAILED
    pause
    exit /b %errorlevel%
)

echo ========================================
echo SUCCESS!
echo ========================================
pause

