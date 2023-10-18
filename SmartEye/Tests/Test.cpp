#define CATCH_CONFIG_MAIN
#include "catch.hpp"

 TEST_CASE("Example Test Case", "[example]") {
     REQUIRE(2 + 2 == 4);
     REQUIRE(3 * 4 == 12);
 }