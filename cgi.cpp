
#include "cgi.h"
#include "util.h"

#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <utility>

using std::string;
using std::size_t;

size_t cgi::read() {
    auto rm = std::getenv("REQUEST_METHOD");
    if (rm == nullptr) {
        std::cout << json_header() << jsonify("CGI Error: No method specified.");
        return 0;
    }
    string method(rm);
    if (method.size() < 3 || method == "(null)") {
        std::cout << "CGI Error: Invalid method specified [" << method << "].\n";
        return 0;
    }
    if (method == "GET") {
        return parse_query_string(string(std::getenv("QUERY_STRING")));
    } else if (method == "POST") {
        std::string line;
        std::string doc;
        constexpr auto limit = 1024u * 10u; 
        while (getline(std::cin, line) && doc.size() < limit) {
            doc += line;
            doc.append(1, '\n');
            line.clear();
        }
        kvp.emplace("POST content", doc);
        return doc.size();

    }
    std::cout << "CGI Error: Unsupported method [" << method << "].\n";
    return 0;
}

size_t cgi::parse_query_string(const string& qs) {
    if (qs.empty()) return 0;

    std::istringstream iss(qs);
    string pair;
    while (std::getline(iss, pair, '&')) { 
        //const string space = " ";
        //std::replace(pair.begin(), pair.end(), '+', ' ');
        //find_replace(&pair, "%20", space);
        auto decoded = decode(pair);
        auto pos = decoded.find('=');
        if (pos != std::string::npos) {
            kvp[decoded.substr(0,pos)] = decoded.substr(pos+1);
        }
    }
    return kvp.size();
}


