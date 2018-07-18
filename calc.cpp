#include "calc.h"
#include "util.h"

#include <iostream>
#include <map>
#include <string>

#include <nlohmann/json.hpp>

using std::string;
using json = nlohmann::json;

struct input_data {
    double air_temp = 20.0;
    string uom = "C";
};

void calculate (const std::map<std::string, std::string>& query_params) {
    std::cout << json_header();
    auto it = query_params.find("air_temp");

    input_data input;
    if (it != query_params.end()) {
        input.air_temp = atof(it->second.c_str());
    }
    it= query_params.find("uom");
    if (it != query_params.end()) {
        input.uom = it -> second[0];
        input.uom = toupper(input.uom[0]);
        if (input.uom == "F") {
            input.air_temp = cvt_f_c(input.air_temp);
        }
    }

    json j;
    j["in"]["air_temp"] = input.air_temp;
    j["in"]["uom"] = input.uom;

    j["out"]["vapor_pressure"]["value"] = vapor_pres_wmo (input.air_temp);
    j["out"]["vapor_pressure"]["uom"] = "hPa";

    j["out"]["absolute_humidity"]["value"] = abs_humidity (input.air_temp);
    j["out"]["absolute_humidity"]["uom"] = "kg/m**3";

    std::cout << j.dump(4);
}

