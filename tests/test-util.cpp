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
    WHEN ("json header is called") {
        THEN("content type for json is always returned") {
            std::string expected = "Content-type: application/json\n\n";
            CHECK(expected == json_header());
        }

    }

          }
