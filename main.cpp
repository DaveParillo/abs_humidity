//
// A simple cgi program to echo input CGI query string
// Can be used as a CGI template
//
// Compile with:
// c++ -std=c++11 -Wall -Wextra -pedantic *.cpp -o cgi
//
// Example output
// {
//   "in" : {
//     "air_temp": { "value": 20, "uom": "C"}
//   },
//   "out" : {
//     "absolute_humidity": { "value": 0.017293, "uom": "kg/m^3"},
//     "vapor_pressure": { "value": 23.397013, "uom": "hPa"}
//   }
// }
//
//
#include "calc.h"
#include "cgi.h"

#include <cstring>
#include <cstdlib>
#include <iostream>
#include <map>
#include <string>

using std::string;

string version() {
  return "version 1.0.0";
}

static void usage(const char* name) {
  std::cerr << "Usage: " << name << " [-h|--help] [-v] "
#ifdef HAVE_SETENV
            << " [QUERY_STRING=value]\n";
#else
            << '\n';
#endif
}

static void help(const char* name) {
    usage(name);
    std::cerr << "Options:\n"
        << "  -h or --help         Show this text and exit\n"
        << "  -v or --version      Show program version and exit\n"
#ifdef HAVE_SETENV
        << "  QUERY_STRING=value   Set the QUERY_STRING in GET request mode.\n\n";
#else
        << "\n";
#endif
    exit(0);
}

// process command line arguments,
// faking a CGI request, if needed.
void process_args(int argc, char** argv);

int main(int argc, char** argv) {
    cgi in;
    size_t count = 0;

    process_args(argc, argv);
    count = in.read();

    if (isvalid(in.kvp)) {
      calculate(in.kvp);
    } else {
        std::cout << "Content-type: text/html\n\n"
                  << "<html>\n"
                  << "<head>\n"
                  << "<title>Bad Request</title>\n"
                  << "</head>\n"
                  << "<body>\n"
                  << "<H1>Bad Request</H1>\n"
                  << "<p>Error 400: Requires valid input.</p>\n"
                  << "<p>This program is an Absolute Humidity calculator. It calculates the vapor pressure\n"
                  << "and absolute humidity using an input air temperature.</p>\n"
                  << "<p>In order to properly function, the input air temperature must be an air temperature\n"
                  << "in Celsius or Fahrenheit.</p>"
                  << "<p>An example of valid input is shown below:</p>\n"
                  << "<p>air_temp=48&uom=Fahrenheit</p>\n"
                  << "<p>The number is the temperature, and the unit of measure (uom) can either be input as Celsius (C)\n"
                  << "or Fahrenheit (F). Either format works for this program.</p>\n"
                  << "</body>\n"
                  << "</html>\n";
    }
    return count;
}

void process_args(int argc, char** argv) {
    for (int i=1; i < argc; ++i) {
        if (!std::strcmp(argv[i], "-h") || !std::strcmp(argv[i], "--help")) {
            help(argv[0]);
        } else if (!std::strcmp(argv[i], "-v") || !std::strcmp(argv[i], "--version")) {
            std::cout << version() << '\n';
            exit(0);
#ifdef HAVE_SETENV
        } else if (std::strncmp(argv[i], "QUERY_STRING", 12) == 0) {
            // for testing, allow setting query string on the command line
            setenv("REQUEST_METHOD", "GET", true);
            std::string qs(argv[i]);
            qs.erase(qs.begin(), qs.begin()+13);
            setenv("QUERY_STRING", qs.c_str(), true);
#endif
        } else {
            std::cerr << "Unknown argument received, " << argv[i] << '\n';
            usage(argv[0]);
            exit(-1);
        }
    }
}

