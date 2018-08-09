#pragma once

#include <cmath>
#include <map>
#include <string>

#include <nlohmann/json.hpp>

struct input_data_t {
    double air_temp = -99.0;
    std::string uom = "kg/m**3";
};

// A pair of response values from the calculator
struct response_t {
    bool valid = false;
    bool echo_input = false;
    nlohmann::json doc;
    input_data_t input;
    response_t() = default;
    response_t(bool v, nlohmann::json d) 
        : valid{v}, doc{d} 
    {}
    response_t(const response_t&) = default;
    response_t& operator=(const response_t& other) = default;
};

// Absolute Humidity calculator
//
// Absolute Humidity is defined as:
// - the mass of water vapor contained in a unit volume of air.
//
// Inputs:
//  - Air temperature (deg C)
//  - Dewpoint temperature (deg C)
//
// Output:
//  - Absolute humidity (kg / m**3)
//

// Perform all required calculations and
// return the response structure
response_t calculate (const response_t& response);

// validate the query string read in by the program
response_t validate (const std::map<std::string, std::string>& query_params);

// helpers for validate
void validate_air_temp  (const std::map<std::string, std::string>& query_params, response_t* r);
void validate_input_uom (const std::map<std::string, std::string>& query_params, response_t* r);
void validate_output_uom (const std::map<std::string, std::string>& query_params, response_t* r);


// build a json object for a specific UOM and value pair
nlohmann::json make_json_param(const std::string& uom, const double& value);

// determine if a string can be converted to a numeric type
bool numeric(std::string value);

// Specific gas constant for water vapor
constexpr double R = 461.514;

// convert Celsius to Kelvin
constexpr double cvt_c_k(double c) { return c + 273.16; }

// convert Kelvin to Celsius
constexpr double cvt_k_c(double k) { return k - 273.16; }

// convert Celsius to Fahrenheit
constexpr double cvt_c_f(double c) { return (9.0 / 5.0) * c + 32.0; }

// convert Fahrenheit to Celcius
constexpr double cvt_f_c(double f) { return (5.0 / 9.0) * (f - 32.0); }

// return vapor pressure over liquid water in hPa
// using air_temp in celsius.
inline double vapor_pres_wmo (double air_temp) {
    return 6.112 * std::exp((17.62*air_temp) / (243.12 + air_temp));
}

// compute absolute humidity in kg / m**3
// using air_temp in celsius.
inline double abs_humidity (double air_temp) {
    // WMO
    // convert vapor_pressure from hPa to Pa
    // vapor pressure takes C, denominator takes K
    return 100.0 * vapor_pres_wmo(air_temp) / (R * cvt_c_k(air_temp));
}


