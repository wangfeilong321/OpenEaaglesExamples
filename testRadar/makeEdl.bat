rem
call ..\makedefs.bat
rem
%mcpp% configs\test1.epp  > test1.edl   -I ..\data -DFONT_PATH="\"../data/fonts/\""  -DFONT_11_19_PATH="\"../data/fonts/11x19/normal/\""
%mcpp% configs\test2a.epp > test2a.edl  -I ..\data -DFONT_PATH="\"../data/fonts/\""  -DFONT_11_19_PATH="\"../data/fonts/11x19/normal/\""
%mcpp% configs\test2b.epp > test2b.edl  -I ..\data -DFONT_PATH="\"../data/fonts/\""  -DFONT_11_19_PATH="\"../data/fonts/11x19/normal/\""
%mcpp% configs\test2c.epp > test2c.edl  -I ..\data -DFONT_PATH="\"../data/fonts/\""  -DFONT_11_19_PATH="\"../data/fonts/11x19/normal/\""
%mcpp% configs\test3a.epp > test3a.edl  -I ..\data -DFONT_PATH="\"../data/fonts/\""  -DFONT_11_19_PATH="\"../data/fonts/11x19/normal/\""
%mcpp% configs\test3b.epp > test3b.edl  -I ..\data -DFONT_PATH="\"../data/fonts/\""  -DFONT_11_19_PATH="\"../data/fonts/11x19/normal/\""
%mcpp% configs\test4a.epp > test4a.edl  -I ..\data -DFONT_PATH="\"../data/fonts/\""  -DFONT_11_19_PATH="\"../data/fonts/11x19/normal/\""
%mcpp% configs\test4b.epp > test4b.edl  -I ..\data -DFONT_PATH="\"../data/fonts/\""  -DFONT_11_19_PATH="\"../data/fonts/11x19/normal/\""
%mcpp% configs\test4c.epp > test4c.edl  -I ..\data -DFONT_PATH="\"../data/fonts/\""  -DFONT_11_19_PATH="\"../data/fonts/11x19/normal/\""

