#include "calc.h"
#include "util.h"

#include <iostream>
#include <map>
#include <string>

using std::string;

void calculate (const std::map<std::string, std::string>& query_params) {
    std::cout << json_header();
    auto it = query_params.find("air_temp");
    auto air_temp = 20.0;

    std::map<string, string> output;
    if (it != query_params.end()) {
        air_temp = atof(it->second.c_str());
    }
    //***CAMERON EDITS***
    it= query_params.find("uom");
    if (it != query_params.end()) {
        char uom = it -> second[0];
        uom = toupper(uom);
        if (uom == 'F') {
            air_temp = cvt_f_c(air_temp);
        }
    }
    //output.emplace("air_temp", std::to_string(air_temp));
    output.emplace("air_temp", jsonify(std::to_string(air_temp), "C"));
    //output.emplace("air_temp_uom", "C");

    std::cout << "{\n\"in\": " << jsonify(output) << ",";

    output.clear();
    auto e =  vapor_pres_wmo (air_temp);
    output.emplace("vapor_pressure", jsonify(std::to_string(e), "hPa"));

    auto ah =  abs_humidity (air_temp);
    output.emplace("absolute_humidity", jsonify(std::to_string(ah), "kg/m^3"));

    std::cout << "\n\"out\": " << jsonify(output) << "\n}\n";
}

