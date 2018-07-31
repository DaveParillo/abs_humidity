CXX = c++
ifndef CXXFLAGS
	CXXFLAGS = -Wall -Wextra -pedantic -std=c++11
endif
ifndef CFLAGS
	CFLAGS = -Wall -Wextra -pedantic -std=c11
endif

ifneq ($(OS),Windows_NT)
	CXXFLAGS += -DHAVE_SETENV
endif

INCLUDES=-I../include/json/single_include

PROGS = abs_humidity

all: $(PROGS)

abs_humidity : clean
	${CXX} ${CXXFLAGS} ${INCLUDES}  -o $@ util.cpp calc.cpp cgi.cpp main.cpp

clean:
	rm -f $(PROGS)

