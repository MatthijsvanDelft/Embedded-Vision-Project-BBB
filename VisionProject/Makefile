# Make
AppName = VisionEngiRacing

# Sources
Sources += ${wildcard ./Sources/*.cpp}

# Headers
Headers += ${wildcard ./Headers/*.hpp}

# Compiler
CXX=g++
CXXFLAGS = ${foreach d, $(IncludePaths), -I$d} -Wall -Wextra -std=c++11
CXXFLAGS+=`pkg-config --cflags opencv`
LDFLAGS+=`pkg-config --libs opencv libv4l2`
#LDFLAGS = -L/usr/local/lib

# Find binaries
Binaries = ${Sources:.cpp=.o}
#Binaries = ${BinariesTemp:.c=.o}

Executable = $(AppName)

.PHONY: all
all: ${Executable}

# Build

$(Executable): $(Binaries) buildnum
	$(CXX) $(CXXFLAGS) $(Binaries) $(LDFLAGS) -o $@
	@echo "-- Build: " $$(cat ./Build/buildnumber.num)

# depend: $(Sources)
# 	$(CXX) $(CXXFLAGS) -MM  $^ > $@

# -include depend

buildnum: $(Binaries)
	@if ! test -f ./Build/buildnumber.num; then echo 0 > ./Build/buildnumber.num; fi
	@echo $$(($$(cat ./Build/buildnumber.num)+1)) > ./Build/buildnumber.num

.PHONY: clean
clean:
	$(RM) $(Executable) $(Binaries)

