#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "breakable_scope.hpp"

TEST_CASE("Let a breakable scope run until it is finished",
          "[breakable_scope]") {
  int value = 0;

  breakable_scope { value = 1; }
  else {
    value = 2;
  }

  REQUIRE(value == 1);
}

TEST_CASE("Break a breakable scope using `break` and evalute the `else` clause",
          "[breakable_scope]") {
  int value = 0;

  breakable_scope {
    value = 1;
    break;
    value = 2;
  }
  else {
    value = 3;
  }

  REQUIRE(value == 3);
}

TEST_CASE("Break a breakable scope using the `continue` keyword",
          "[breakable_scope]") {
  int value = 0;

  breakable_scope {
    value = 1;
    continue;
    value = 2;
  }
  else {
    value = 3;
  }

  REQUIRE(value == 1);
}

TEST_CASE("Nest breakable scopes", "[breakable_scope]") {
  int value = 0;

  breakable_scope {
    value = 1;
    breakable_scope {
      value = 2;
      break;
      value = 3;
    }
  }
  else {
    value = 4;
  }

  REQUIRE(value == 2);
}
