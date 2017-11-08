#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "breakable_scope.hpp"

TEST_CASE("Let a breakable scope run until it is finished",
          "[breakable_scope]") {
  int value = 0;

  const bool condition = true;
  breakable_scope {
    value = 1;
    if (!condition) {
      break;
    }
    value = 2;
  }
  else {
    value = -1;
  }

  REQUIRE(value == 2);
}

TEST_CASE("Break a breakable scope and evalute the `else` clause",
          "[breakable_scope]") {
  int value = 0;

  const bool condition = false;
  breakable_scope {
    value = 1;
    if (!condition) {
      break;
    }
    value = 2;
  }
  else {
    value = -1;
  }

  REQUIRE(value == -1);
}

TEST_CASE("Break a breakable scope using the `continue` keyword",
          "[breakable_scope]") {
  int value = 0;

  const bool condition = false;
  breakable_scope {
    value = 1;
    if (!condition) {
      continue;
    }
    value = 2;
  }
  else {
    value = -1;
  }

  REQUIRE(value == 1);
}
