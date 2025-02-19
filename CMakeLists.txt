#..............................................................
# CMakeList.txt : CMake project for CMakeProject1, include source and define
# project specific logic here.
#..............................................................

cmake_minimum_required (VERSION 3.8)

project ("PeasyCompiler")

# Add source to this project's executable.
add_executable (PeasyCompiler
	"Compilers.h" "Compilers.c"
	"Buffer.h" "Buffer.c" "MainBuffer.c"
	"Scanner.h" "Scanner.c" "MainScanner.c"
	"Parser.h" "Parser.c" "MainParser.c"
)

# TODO: Add tests and install targets if needed.
