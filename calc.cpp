#include "calc.h"

#include <iostream>
#include <map>
#include <string>
#include <sstream>

#include <nlohmann/json.hpp>

using std::string;
using json = nlohmann::json;

struct input_data_t {
    double air_temp = -99.0;
    string uom = "C";
};

// stores the input air temp required by the model (C)
static input_data_t input;

json make_json_pair(const string& uom, const double& value) {
    return {{"uom", uom}, {"value", value}};
}

bool numeric(string value) {
    auto ss = std::stringstream(value);
    double tmp;
    ss >> tmp;
    return !(!ss || ss.rdbuf()->in_avail() > 0);
}

response_t isvalid (const std::map<std::string, std::string>& query_params) {
    response_t r = {true, {"status","success"}};

    // validate required air temp input
    auto it = query_params.find("air_temp");
    if (it != query_params.end() && !it->second.empty()) {
        if(numeric(it->second)) {
            input.air_temp = atof(it->second.c_str());
        } else {
            r.valid = false;
            r.doc["status"] = "error";
            r.doc["message"] = "Non-numeric value provided for air_temp.";
            r.doc["expected"] = "a floating point value [-45C - 60C]";
            r.doc["actual"] = it->second;
        }
    } else if (it->second.empty()) {
        r.valid = false;
        r.doc["status"] = "error";
        r.doc["message"] = "No value provided for air_temp input parameter.";
        r.doc["expected"] = "a floating point value [-45C - 60C]";
        r.doc["actual"] = it->second;
    } else {
        r.valid = false;
        r.doc["status"] = "error";
        r.doc["message"] = "Required input parameter not specified.";
        r.doc["expected"] = "air_temp";
        r.doc["actual"] = nullptr;
        return r;
    }

    // validate optional uom for air temp
    it = query_params.find("uom");
    if (it != query_params.end()) {
        string uom = it->second;
        uom = toupper(uom[0]);
        if (uom == "F") {
            input.air_temp = cvt_f_c(input.air_temp);
        } else if (uom == "C") {
            // no action required
        } else {
            r.valid = false;
            r.doc["status"] = "error";
            r.doc["message"] = "Unknown unit of measure provided.";
            r.doc["expected"] = "One of 'uom=C' or 'uom=F'.";
            r.doc["actual"] = it->second;
            return r;
        }
    }
    return r;
}

response_t calculate (const response_t& response) {
    auto r = response;

    if (input.air_temp < -45 || input.air_temp > 60) {
        r.doc["status"] = "warn";
        r.doc["message"] =
              "The valid input limits for air temperature are between -45C and 60C";
    }
    r.doc["data"]["vapor_pressure"] = make_json_pair("hPa", vapor_pres_wmo (input.air_temp));

    auto ah = abs_humidity (input.air_temp);
    r.doc["data"]["absolute_humidity"] = make_json_pair("kg/m**3", ah);
    //r.doc["data"]["absolute_humidity"] = make_json_pair("g/m**3", ah/1000.0);

    return r;
}


