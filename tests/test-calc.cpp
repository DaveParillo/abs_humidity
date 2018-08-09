#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../calc.h"
#include <doctest.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

SCENARIO( "Check if calculate runs with validated inputs" ) {
    GIVEN( "A simple valid response structure" ) {
        response_t expected = {true, json::object({ {"status","success"} })};

        expected.input.air_temp = 22;
        auto actual = calculate(expected);
        REQUIRE(actual.doc["status"] == expected.doc["status"]);
        REQUIRE(actual.valid == expected.valid);
    }
    GIVEN( "input air temp out of bounds for vapor pressure" ) {
        response_t expected = {true, json::object({ {"status","success"} })};

        expected.input.air_temp = 60.1;
        auto actual = calculate(expected);
        REQUIRE(actual.doc["status"] == std::string("warn"));
        REQUIRE(actual.valid == true);
    }
}


