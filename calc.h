#pragma once

#include <cmath>
#include <map>
#include <string>

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
// print results on standard out
void calculate (const std::map<std::string, std::string>& query_params);

// validate the query string read in by the program
inline bool isvalid (const std::map<std::string, std::string>& query_params) {
    return !query_params.empty();
}

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
constexpr double vapor_pres_oaml (double air_temp) {
    // OAML / Tetens
    return 6.112 * std::exp((17.269 * air_temp) / (air_temp + 273.16 - 35.86));
}
constexpr double vapor_pres_noaa (double air_temp) {
    // NOAA
    return 6.112 * std::exp((17.62*air_temp) / (243.12 + air_temp));
}
constexpr double vapor_pres_wmo (double air_temp) {
    // USAF/ WMO
    return 6.112 * std::pow(10.0, (7.5*air_temp) / (237.3 + air_temp));
}

// compute absolute humidity in kg / m**3
// using air_temp in celsius.
constexpr double abs_humidity (double air_temp) {
    // WMO
    // convert vapor_pressure from hPa to Pa
    // vapor pressure takes C, denominator takes K
    return 100.0 * vapor_pres_wmo(air_temp) / (R * cvt_c_k(air_temp));
}


