$ErrorActionPreference = "Stop"

$env:PATH = "C:\msys64\mingw32\bin;$env:PATH"

& "C:\msys64\mingw32\bin\g++.exe" `
    part_e_jet_city_animation.cpp `
    -o part_e_jet_city_animation.exe `
    -static `
    -static-libgcc `
    -static-libstdc++ `
    -I..\part_e_graphics_h\vendor\winbgim\include `
    -L..\part_e_graphics_h\vendor\winbgim\lib `
    -lbgi `
    -lgdi32 `
    -lcomdlg32 `
    -luuid `
    -loleaut32 `
    -lole32

if ($LASTEXITCODE -ne 0) {
    exit $LASTEXITCODE
}

Write-Host "Built part_e_jet_city_animation.exe"
