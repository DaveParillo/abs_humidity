#include "calc.h"
#include "util.h"

#include <iostream>
#include <map>
#include <string>

#include <nlohmann/json.hpp>

using std::string;
using json = nlohmann::json;

//I think that by having a default setting, the program just skips the error screen.
//Because the error screen is only triggered by a lack of input, then by setting there
//to be a default setting of 20 degrees Celsius, the program detects something within
//the query string, so the error/help screen doesn't show up. I tried deleting it,
//but then the output is still the regular output screen but with basically zero for the
//outputs (e.g.

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

    if (input.air_temp >= -45 && input.air_temp <= 60) {
            j["out"]["vapor_pressure"]["value"] = vapor_pres_wmo (input.air_temp);
    }
    //I'm not sure if you want to have it as an html doc, nor do I know how I'd go about properly formatting it, so I plan on
    //using the below message as a placeholder for when you want a more official output to be read. It compiles and works perfectly.
    else {
        std::cout << "Bad Request\n"
                  << "Error 400: Requires valid input.\n"
                  << "The air temperature needs to be between -45 to 60 degrees Celsius in order to compute\n"
                  << "the absolute humidity.\n";
        exit (0);
    }
    j["out"]["vapor_pressure"]["uom"] = "hPa";

    j["out"]["absolute_humidity"]["value"] = abs_humidity (input.air_temp);
    j["out"]["absolute_humidity"]["uom"] = "kg/m**3";

    std::cout << j.dump(4);
}

