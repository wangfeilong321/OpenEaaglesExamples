rem
call ..\makedefs.bat
rem
%mcpp% configs\test.epp > test.edl -I ..\data -I ..\shared-libs  -DFONT_PATH="\"../data/fonts/\""  -I ..\..\openeaagles\include  -DFONT_11_19_PATH="\"../data/fonts/11x19/normal/\"" -DTEXTURES_PATH="\"../data/textures/\""

