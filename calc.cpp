#include "calc.h"

#include <iostream>
#include <map>
#include <string>

#include <nlohmann/json.hpp>

using std::string;
using json = nlohmann::json;

struct input_data_t {
    double air_temp = -99.0;
    string uom = "C";
};

response_t isvalid (const std::map<std::string, std::string>& query_params) {
    json j;
    response_t r {true, j};
    auto it = query_params.find("air_temp");
    input_data_t input;
    if (it != query_params.end()) {
        input.air_temp = atof(it->second.c_str());
        r.doc["in"]["air_temp"]["C"] = input.air_temp;
    } else {
        r.valid = false;
        r.doc["message"] = "Service Error: Required input parameter not specified.";
        r.doc["expected"] = "air_temp";
        r.doc["actual"] = nullptr;
    }
    it = query_params.find("uom");
    if (it != query_params.end()) {
        string uom = it->second;
        uom = toupper(uom[0]);
        if (uom == "F") {
            r.doc["in"]["air_temp"]["C"] = cvt_f_c(input.air_temp);
            r.doc["in"]["air_temp"]["F"] =  input.air_temp;
        } else if (uom == "C") {
            //r.doc["in"]["air_temp"]["uom"] = uom;
        } else {
            r.valid = false;
            r.doc["message"] = "Service Error: Unknown unit of measure provided.";
            r.doc["expected"] = "One of 'uom=C' or 'uom=F'.";
            r.doc["actual"] = it->second;
            r.doc.erase("in");
            return r;
        }
    }

    return r;
}

response_t calculate (const response_t& response) {
    auto r = response;
    input_data_t input;
    input.air_temp = r.doc["in"]["air_temp"]["C"];

    if (input.air_temp < -45 || input.air_temp > 60) {
        r.doc["out"]["absolute_humidity"]["alert"] = 
            "The valid input limits for air temperature are between -45C and 60C";
    }
    r.doc["out"]["vapor_pressure"]["hPa"] = vapor_pres_wmo (input.air_temp);

    auto ah = abs_humidity (input.air_temp);
    r.doc["out"]["absolute_humidity"]["kg/m**3"] = ah;
    r.doc["out"]["absolute_humidity"]["g/m**3"] = ah/1000;

    return r;
}

