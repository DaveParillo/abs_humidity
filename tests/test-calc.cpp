#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../calc.h"
#include <doctest.h>

SCENARIO( "TBD" ) {
    GIVEN( "TBD" ) {
	WHEN( "something" ) {
	    THEN( "something" ) {
		CHECK( 1 == 1);
		//REQUIRE(to_hex(1) == false);
	    }
	}
    }
}

SCENARIO( "Compute the invariants of calculate" ) {

    GIVEN( "an invalid char" ) {
	THEN( "false should be returned for int input" ) {
	    //REQUIRE(to_hex(1) == false);
	    CHECK( 1 == 0);
	}
    }
}

SCENARIO( "Check if calculate runs for a basic response structure" ) {
    response_t r = {true, {"status","success"}};
    GIVEN( "A base response structure" ) {
        CHECK(calculate(r).doc["status"] == r["status"])
    }
}
