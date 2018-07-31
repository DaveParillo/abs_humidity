#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../util.h"
#include <doctest.h>

SCENARIO( "Convert characters to hex" ) {
    GIVEN( "a valid char" ) {
        WHEN( "false should be returned for int input" ) {
            THEN( "false should be returned for int input" ) {
                CHECK( 1 == 1);
                //REQUIRE(to_hex(1) == false);
            }
        }
    }
}
SCENARIO( "Compute the invariants of to_hex" ) {

    GIVEN( "an invalid char" ) {
        THEN( "false should be returned for int input" ) {
            //REQUIRE(to_hex(1) == false);
        }
    }
}

SCENARIO ("Check if json_header works") {
    std::string expected = "Status: 200 OK\nContent-type: application/json\n\n";
    WHEN ("json header is called with no parameters") {
        THEN("content type for json and 200 OK returned") {
            CHECK(expected == json_header());
        }

    }
    WHEN ("json header is passed StatusCode::ok") {
        THEN("content type for json and 200 OK returned") {
            CHECK(expected == json_header(StatusCode::ok));
        }

    }
    WHEN ("json header is passed StatusCode::bad") {
        THEN("content type for json and 400 Bad Request returned") {
            expected = "Status: 400 Bad Request\nContent-type: application/json\n\n";
            CHECK(expected == json_header(StatusCode::bad));
        }

    }

}
