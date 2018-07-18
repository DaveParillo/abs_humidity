#include "util.h"

#include <algorithm>
#include <map>
#include <string>
#include <sstream>
#include <utility>

using std::string;
using std::size_t;

char to_hex(const char code) {
  static char hex[] = "0123456789abcdef";
  return hex[code & 15];
}

char from_hex(const char ch) {
  return isdigit(ch) ? ch - '0' : tolower(ch) - 'a' + 10;
}

string decode(const string& source) {
    // TODO: this would much more efficient if 
    // instead of iterating over every character,
    // jump to each % and +
    std::ostringstream dest;
    for (auto it = source.begin(); it != source.end(); ) {
        if (*it == '%') {
            // if 2 or more  chars away from end of string
            if (std::distance(source.begin(), it) < int(source.length() - 2)) {
                char c = from_hex(*(it+1)) << 4 | from_hex (*(it+2));
                dest << c;
                it += 2;
            }
        } else if (*it == '+') {
            dest << ' ';
        } else {
            dest << *it;
        }
        ++it;
    }
    return dest.str();
}

string json_header () {
    return "Content-type: application/json\n\n";
}

string jsonify(const string& message) {
    return "{\"msg\":\"" + message + "\"}\n"; 
}

bool numeric(string value) {
    auto ss = std::stringstream(value);
    double tmp;
    ss >> tmp;
    return !(!ss || ss.rdbuf()->in_avail() > 0);
}

string jsonify(const string& value, const string& uom) {
    string msg = "{"; 
    if (numeric(value)) {
        msg += "\"value\": " + value + ',';
    } else {
        msg += "\"value\": \"" + value + "\",";
    }
    msg += "\"uom\": \"" + uom + "\"}";
    return msg; 
}

string jsonify(const std::map<string,string>& content) {
    string msg = "{"; 
    string json = msg; 
    for (const auto& pair: content) {
        if (numeric(pair.second)) {
            msg += '"' + pair.first + "\":" + pair.second + ','; 
        } else if (pair.second[0] == json[0] ) {
            msg += '"' + pair.first + "\":" + pair.second + ','; 
        } else {
            msg += '"' + pair.first + "\":\"" + pair.second + "\","; 
        }
    }
    msg.pop_back();
    msg += "}"; 
    return msg;
}

void find_replace(string* source, 
        const string& old_string, 
        const string& new_string) {
    for(std::string::size_type i = 0; (i = source->find(old_string, i)) != string::npos;)
    {
        source->replace(i, old_string.length(), new_string);
        i += new_string.length();
    }
}

