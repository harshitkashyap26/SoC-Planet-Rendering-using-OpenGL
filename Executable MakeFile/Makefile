MAINCODE = ..\OpenGLVS\OpenGLVS
GLEW = ..\External Resources\GLEW\include
GLFW = ..\External Resources\GLFW\include
ASSIMP = ..\External Resources\assimp\include
LIB = "..\External Resources\GLFW\lib-vc2019\glfw3.lib" "..\External Resources\GLEW\lib\Release\Win32\glew32s.lib" "..\External Resources\assimp\lib\assimp-vc142-mtd.lib" "opengl32.lib" "kernel32.lib" "user32.lib" "gdi32.lib" "winspool.lib" "comdlg32.lib" "advapi32.lib" "shell32.lib" "ole32.lib" "oleaut32.lib" "uuid.lib" "odbc32.lib" "odbccp32.lib" "msvcrt.lib" "msvcmrt.lib"
OTHER = ..\External Resources

Planet.exe: glew.obj ImageLoader.obj Shader.obj Planet.obj
	link /Machine:x86 /DYNAMICBASE $(LIB) Planet.obj glew.obj ImageLoader.obj Shader.obj
glew.obj: {$(OTHER)}glew.c
	cl /c /I "$(GLEW)" /I "$(GLFW)" /I "$(ASSIMP)" /nologo "$(OTHER)\glew.c"
ImageLoader.obj: {$(OTHER)}ImageLoader.cpp
	cl /c /I "$(GLEW)" /I "$(GLFW)" /I "$(ASSIMP)" /nologo /EHsc "$(OTHER)\ImageLoader.cpp"
Shader.obj: {$(GLFW)}shader_g.h
	cl /c /I "$(GLEW)" /I "$(GLFW)" /I "$(ASSIMP)" /nologo /EHsc "$(GLFW)\shader_g.cpp"
Planet.obj: {$(MAINCODE)}MainLite.cpp
	cl /c /I "$(GLEW)" /I "$(GLFW)" /I "$(ASSIMP)" /nologo /EHsc "$(MAINCODE)\MainLite.cpp"
clean:
	del *.obj
	del Planet.exe