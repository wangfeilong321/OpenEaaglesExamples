rem
call ..\makedefs.bat
rem
%mcpp% configs/glutdisplay.epp > glutdisplay.edl ^
 -I ..\shared\data ^
 -DFONT_PATH="\"../shared/data/fonts/\"" ^
 -DFONT_11_19_PATH="\"../shared/data/fonts/11x19/normal/\"" ^
 -DTEXTURES_PATH="\"../shared/data/textures/\""
