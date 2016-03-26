TGT=build
INCLUDES = 
CXXFLAGS = -Wall -std=c++11  $(INCLUDES)
LDFLAGS = -Wall -std=c++11 -g
OBJS = $(addprefix $(TGT)/, $(notdir $(SOURCES:.cpp=.o)))

linux:
	mkdir -p $(TGT)/linux
	cd src; make COMPILETO="linux"
	$(CXX) $(LDFLAGS) $(TGT)/linux/*.o -Isrc -o $(TGT)/linux/output.bin -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf
	
win32:
	mkdir -p $(TGT)/win32
	cd src; make COMPILETO="win32"
	$(CXX) $(LDFLAGS) $(TGT)/win32/*.o -o $(TGT)/win32/output.exe -static-libgcc -static-libstdc++ -mconsole -mwindows -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf