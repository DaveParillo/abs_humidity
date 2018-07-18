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

PROGS = cgi

all: $(PROGS)

cgi : clean
	${CXX} ${CXXFLAGS} -o $@ util.cpp calc.cpp cgi.cpp main.cpp

clean:
	rm -f $(PROGS)

