#include "calc.h"

//#include <iostream>
#include <algorithm>
#include <locale>
#include <map>
#include <string>
#include <sstream>

#include <nlohmann/json.hpp>

using std::string;
using json = nlohmann::json;
using kvp_map = std::map<std::string, std::string>;

json make_json_pair(const string& uom, const double& value) {
    return {{"uom", uom}, {"value", value}};
}

bool numeric(string value) {
    auto ss = std::stringstream(value);
    double tmp;
    ss >> tmp;
    return !(!ss || ss.rdbuf()->in_avail() > 0);
}

void validate_air_temp (const kvp_map& query_params, response_t* r) {
    auto it = query_params.find("air_temp");
    if (it != query_params.end() && !it->second.empty()) {
        if(numeric(it->second)) {
            r->input.air_temp = atof(it->second.c_str());
        } else {
            r->valid = false;
            r->doc["status"] = "error";
            r->doc["message"] = "Non-numeric value provided for air_temp.";
            r->doc["expected"] = "a floating point value [-45C - 60C]";
            r->doc["actual"] = it->second;
        }
    } else if (it->second.empty()) {
        r->valid = false;
        r->doc["status"] = "error";
        r->doc["message"] = "No value provided for air_temp input parameter.";
        r->doc["expected"] = "a floating point value [-45C - 60C]";
        r->doc["actual"] = it->second;
    } else {
        r->valid = false;
        r->doc["status"] = "error";
        r->doc["message"] = "Required input parameter not specified.";
        r->doc["expected"] = "air_temp";
        r->doc["actual"] = nullptr;
    }
}

void validate_input_uom (const kvp_map& query_params, response_t* r) {
    auto it = query_params.find("uom");
    if (it != query_params.end()) {
        string uom = it->second;
        uom = toupper(uom[0]);
        if (uom == "F") {
            r->input.air_temp = cvt_f_c(r->input.air_temp);
        } else if (uom == "C") {
            // no action required
        } else {
            r->valid = false;
            r->doc["status"] = "error";
            r->doc["message"] = "Unknown unit of measure provided.";
            r->doc["expected"] = "One of 'uom=C' or 'uom=F'.";
            r->doc["actual"] = it->second;
        }
    }
}

void validate_output_uom (const kvp_map& query_params, response_t* r) {
    auto it = query_params.find("output_uom");
    if (it != query_params.end()) {
        string uom = it->second;
        const string default_uom = "kg/m**3";
        const string user_uom = "g/m**3";
        std::transform(uom.begin(), uom.end(), uom.begin(), 
                [](char x) {return std::tolower(x, std::locale());});
        if (uom == user_uom) {
            r->input.uom = user_uom;
        } else if (uom == default_uom) {
            // no action required
        } else {
            r->valid = false;
            r->doc["status"] = "error";
            r->doc["message"] = "Unknown unit of measure provided.";
            r->doc["expected"] = "One of 'output_uom=kg/m**3' or 'output_uom=g/m**3'.";
            r->doc["actual"] = it->second;
        }
    }
    if (r->input.air_temp < -100 || r->input.air_temp > 100) {
        r->valid = false;
        r->doc["status"] = "error";
        r->doc["message"] =
              "The maximum input limits for air temperature are between -100C and 100C";
    }
}


response_t validate (const kvp_map& query_params) {
    response_t response = {true, json::object({ {"status","success"} })};

    validate_air_temp(query_params, &response);
    if (!response.valid) return response;

    validate_input_uom(query_params, &response);
    if (!response.valid) return response;

    validate_output_uom(query_params, &response);
    return response;
}

response_t calculate (const response_t& response) {
    auto r = response;

    if (r.input.air_temp < -45 || r.input.air_temp > 60) {
        r.doc["status"] = "warn";
        r.doc["message"] =
              "The valid input limits for air temperature are between -45C and 60C";
    }
    r.doc["data"]["vapor_pressure"] = make_json_pair("hPa", vapor_pres_wmo (r.input.air_temp));

    const string user_uom = "g/m**3";
    auto ah = abs_humidity (r.input.air_temp);
    if(r.input.uom == user_uom) {
        ah = ah * 1000.0;
    }
    r.doc["data"]["absolute_humidity"] = make_json_pair(r.input.uom, ah);

    return r;
}


