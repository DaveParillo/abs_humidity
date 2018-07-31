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

json make_json_pair(const string& param, const string& uom, const double& value) {
    return {{param, { {"uom", uom}, {"value", value} } }};
}

response_t isvalid (const std::map<std::string, std::string>& query_params) {
    json j;
    response_t r {true, j};
    // validate required air temp input
    auto it = query_params.find("air_temp");
    if (it != query_params.end()) {
        input.air_temp = atof(it->second.c_str());
    } else {
        r.valid = false;
        r.doc["status"] = "error";
        r.doc["message"] = "Service Error: Required input parameter not specified.";
        r.doc["expected"] = "air_temp";
        r.doc["actual"] = nullptr;
        return r;
    }

    j = make_json_pair ("C", input.air_temp); 

    // validate optional uom for air temp
    it = query_params.find("uom");
    if (it != query_params.end()) {
        string uom = it->second;
        uom = toupper(uom[0]);
        if (uom == "F") {
            j = make_json_pair ("F", input.air_temp); 
        } else {
            r.valid = false;
            r.doc["status"] = "error";
            r.doc["message"] = "Service Error: Unknown unit of measure provided.";
            r.doc["expected"] = "One of 'uom=C' or 'uom=F'.";
            r.doc["actual"] = it->second;
            return r;
        }
    }
    if (!r.echo_input) {
        //json in = {{"in", j}};
        //r.doc = in;
        r.doc["in"]["air_temp"] = j;
    }
    return r;
}

response_t calculate (const response_t& response) {
    auto r = response;
    r.doc["status"] = "success";

    if (input.air_temp < -45 || input.air_temp > 60) {
        r.doc["status"] = "warn";
        r.doc["message"] = 
              "The valid input limits for air temperature are between -45C and 60C";
    }
    r.doc["out"]["vapor_pressure"] = make_json_pair("hPa", vapor_pres_wmo (input.air_temp));

    auto ah = abs_humidity (input.air_temp);
    r.doc["out"]["absolute_humidity"] = make_json_pair("kg/m**3", ah);
//    r.doc["out"]["absolute_humidity"]["g/m**3"] = ah/1000;

    return r;
}

