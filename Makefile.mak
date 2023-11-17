cxx = cl.exe
cxxflags = /W3 /std:c++20
cxxdebug = /Zi
link = link.exe
ldebug = /DEBUG

all: setup clean lab03.exe

setup:
	if not exist build mkdir build
	del /Q build

.cpp.obj:
	echo $< $@ $*.cpp
	$(cxx) $(cdebug) $(cflags) /c $*.cpp /Fo$@

lab03.exe: 03lab\main.obj
	$(link) $(ldebug) -out:build\lab03.exe 03lab\main.obj

clean:
	del /f 03lab\*.obj
	del /Q build
