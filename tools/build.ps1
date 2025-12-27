# Build using MinGW (g++) and CMake
param(
  [string]$generator = "MinGW Makefiles",
  [string]$buildDir = "build"
)

if (-not (Test-Path $buildDir)) { New-Item -ItemType Directory -Path $buildDir | Out-Null }
Push-Location $buildDir
cmake -G $generator -DCMAKE_BUILD_TYPE=Debug ..
cmake --build . -- -j 4
Pop-Location
