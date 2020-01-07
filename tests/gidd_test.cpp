/**
 * Copyright Xhargh 2016-2020
 */

#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "gidd_utils.hpp"

using namespace std;


TEST_CASE( "Split strings", "[utils]" ) {
  vector<string> t1{"a", "b", "c"};
  REQUIRE( split("/a/b/c", '/') == t1);
  REQUIRE( split("*a*b*c***", '*') == t1);

  vector<string> empty{};
  REQUIRE( split("", '/') == empty);
  REQUIRE( split("/", '/') == empty);
  REQUIRE( split("////", '/') == empty);
  REQUIRE( split("***", '*') == empty);
}

TEST_CASE( "Remove . and .. from paths", "[utils]"){
  REQUIRE( removeDots("/aa/eeee/../b/../c/../../foo.bar") == "/foo.bar" );
  REQUIRE( removeDots("/AAAA/B/C/DDDD/E") == "/AAAA/B/C/DDDD/E" );
  REQUIRE( removeDots("/AA/bbbb/../C/D") == "/AA/C/D" );
  REQUIRE( removeDots("/aa/eeee/../../B/C") == "/B/C" );
  REQUIRE( removeDots("/AA/eeee/../b/../C") == "/AA/C" );
  REQUIRE( removeDots("/aa/eeee/../b/../c/../..") == "" );
  REQUIRE( removeDots("/.") == "" );
  REQUIRE( removeDots("/./A") == "/A" );
  REQUIRE( removeDots("/./A/./B/./C") == "/A/B/C" );
  REQUIRE( removeDots("/apa") == "/apa" );
  REQUIRE( removeDots("") == "" );
  REQUIRE( removeDots("src/foo") == "src/foo" );
  REQUIRE( removeDots("..") == ".." );
  REQUIRE( removeDots("../..") == "../.." );
  REQUIRE( removeDots("../../foo") == "../../foo" );
}
