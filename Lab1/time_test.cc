#include "catch.hpp"
#include "Time.hpp"
#include <sstream>

using namespace std;

TEST_CASE("Constructors and getters")
{
   SECTION("Default")
   {
      Time empty{};
      CHECK(empty.get_hour() == 0);
      CHECK(empty.get_minute() == 0);
      CHECK(empty.get_second() == 0);
   }

   SECTION("Integer")
   {
      Time t0{0,0,0};
      Time t1{12,30,30};
      Time t2{23,59,59};

      // Out of bounds
      CHECK_THROWS( Time{13,35,60} );
      CHECK_THROWS( Time{13,60,35} );
      CHECK_THROWS( Time{24,35,35} );
      // Does not even compile:
      // CHECK_THROWS( Time{-1,35,35} );
      // CHECK_THROWS( Time{13,-1,35} );
      // CHECK_THROWS( Time{13,35,-1} );

      CHECK( t0.get_hour()   == 0 );
      CHECK( t0.get_minute() == 0 );
      CHECK( t0.get_second() == 0 );
      CHECK( t1.get_hour()   == 12 );
      CHECK( t1.get_minute() == 30 );
      CHECK( t1.get_second() == 30 );
      CHECK( t2.get_hour()   == 23 );
      CHECK( t2.get_minute() == 59 );
      CHECK( t2.get_second() == 59 );
   }

   SECTION("Integer + Millisecond")
   {
   Time t0{0,0,0,0};
   Time t1{12,30,30,222};
   Time t2{23,59,59,999};

   // Out of bounds:
   CHECK_THROWS(Time{13, 35, 60, 100});
   CHECK_THROWS(Time{13, 60, 35, 100});
   CHECK_THROWS(Time{24, 35, 35, 100});
   CHECK_THROWS(Time{13, 35, 35, 1000});
   CHECK_THROWS(Time{13, 35, 35, 1000});
   CHECK_THROWS(Time{13, 35, 35, 1000});
   // Does not even compile:
   // CHECK_THROWS(Time{-1, 35, 35, 100});
   // CHECK_THROWS(Time{35, -1, 35, 100});
   // CHECK_THROWS(Time{35, 35, -1, 100});
   // CHECK_THROWS(Time{35, 35, 35, -1});

   CHECK(t0.get_hour() == 0);
   CHECK(t0.get_minute() == 0);
   CHECK(t0.get_second() == 0);
   CHECK(t0.get_millisecond() == 0);
   CHECK(t1.get_hour() == 12);
   CHECK(t1.get_minute() == 30);
   CHECK(t1.get_second() == 30);
   CHECK(t1.get_millisecond() == 222);
   CHECK(t2.get_hour() == 23);
   CHECK(t2.get_minute() == 59);
   CHECK(t2.get_second() == 59);
   CHECK(t2.get_millisecond() == 999);
   }

   SECTION("String")
   {
      Time t0{"00:00:00"};
      Time t1{"12:30:30"};
      Time t2{"23:59:59"};

      // Out of bounds:
      CHECK_THROWS( Time{"13:35:60"} );
      CHECK_THROWS( Time{"13:60:35"} );
      CHECK_THROWS( Time{"24:35:35"} );
      CHECK_THROWS( Time{"-13:35:35"} );
      CHECK_THROWS( Time{"13:-35:35"} );
      CHECK_THROWS( Time{"13:35:-35"} );

      // Wrong format:
      CHECK_THROWS( Time{"123::456"} );
      CHECK_THROWS( Time{"12:34"} );
      CHECK_THROWS( Time{"12.34.56"} );

      CHECK( t0.get_hour()   == 0 );
      CHECK( t0.get_minute() == 0 );
      CHECK( t0.get_second() == 0 );
      CHECK( t1.get_hour()   == 12 );
      CHECK( t1.get_minute() == 30 );
      CHECK( t1.get_second() == 30 );
      CHECK( t2.get_hour()   == 23 );
      CHECK( t2.get_minute() == 59 );
      CHECK( t2.get_second() == 59 );
   }

   SECTION("String + Millisecond")
   {
      Time t0{"00:00:00.000"};
      Time t1{"12:30:30.222"};
      Time t2{"23:59:59.999"};
      Time t3{"1:2:3.4"};
      Time t4{"01:02:03.04"};
      Time t5{"01:02:03.004"};

      // Out of bounds:
      CHECK_THROWS(Time{"13:35:60.100"});
      CHECK_THROWS(Time{"13:60:35.100"});
      CHECK_THROWS(Time{"24:35:35.100"});
      CHECK_THROWS(Time{"13:35:35.1000"});
      CHECK_THROWS(Time{"13:35:35.1000"});
      CHECK_THROWS(Time{"13:35:35.1000"});
      CHECK_THROWS(Time{"-13:35:35.100"});
      CHECK_THROWS(Time{"13:-35:35.100"});
      CHECK_THROWS(Time{"13:35:-35.100"});
      CHECK_THROWS(Time{"13:35:35.-100"});

      // Wrong format:
      CHECK_THROWS(Time{"123::456.100"});
      CHECK_THROWS(Time{"12:34:56:100"});
      CHECK_THROWS(Time{"12.34.56.100"});
      CHECK_THROWS(Time{"12.34.56:100"});
      CHECK_THROWS(Time{"12:3456.100"});

      CHECK(t0.get_hour() == 0);
      CHECK(t0.get_minute() == 0);
      CHECK(t0.get_second() == 0);
      CHECK(t0.get_millisecond() == 0);
      CHECK(t1.get_hour() == 12);
      CHECK(t1.get_minute() == 30);
      CHECK(t1.get_second() == 30);
      CHECK(t1.get_millisecond() == 222);
      CHECK(t2.get_hour() == 23);
      CHECK(t2.get_minute() == 59);
      CHECK(t2.get_second() == 59);
      CHECK(t2.get_millisecond() == 999);
      CHECK(t3.get_hour() == 1);
      CHECK(t3.get_minute() == 2);
      CHECK(t3.get_second() == 3);
      CHECK(t3.get_millisecond() == 4);
      CHECK(t4.get_hour() == 1);
      CHECK(t4.get_minute() == 2);
      CHECK(t4.get_second() == 3);
      CHECK(t4.get_millisecond() == 4);
      CHECK(t5.get_hour() == 1);
      CHECK(t5.get_minute() == 2);
      CHECK(t5.get_second() == 3);
      CHECK(t5.get_millisecond() == 4);
   }
}

TEST_CASE("operator++")
{
   Time t0{};
   Time t1{11, 59, 59};
   Time t2{12, 0, 0};
   Time t3{13, 0, 0};
   Time t4{23, 59, 59};
   SECTION("Prefix increment")
   {
      CHECK((++t0).to_string() == "00:00:01");
      CHECK((++t1).to_string() == "12:00:00");
      CHECK((++t2).to_string() == "12:00:01");
      CHECK((++t3).to_string() == "13:00:01");
      CHECK((++t4).to_string() == "00:00:00");
   }
   SECTION("Postfix increment")
   {
      CHECK((t0++).to_string() == "00:00:00");
      CHECK((t1++).to_string() == "11:59:59");
      CHECK((t2++).to_string() == "12:00:00");
      CHECK((t3++).to_string() == "13:00:00");
      CHECK((t4++).to_string() == "23:59:59");

      CHECK((t0).to_string() == "00:00:01");
      CHECK((t1).to_string() == "12:00:00");
      CHECK((t2).to_string() == "12:00:01");
      CHECK((t3).to_string() == "13:00:01");
      CHECK((t4).to_string() == "00:00:00");
   }
}

TEST_CASE("to_string")
{
   Time t0{};
   Time t1{11, 59, 59};
   Time t2{12, 0, 0};
   Time t3{13, 0, 0};
   Time t4{23, 59, 59};
   Time t5{12, 34, 56, 78};
   Time t6{12, 34, 56, 0};
   Time t7{"12:34:56.78"};
   Time t8{"12:34:56.0"};
   SECTION("24 hour format no argument")
   {
      CHECK(t0.to_string() == "00:00:00");
      CHECK(t1.to_string() == "11:59:59");
      CHECK(t2.to_string() == "12:00:00");
      CHECK(t3.to_string() == "13:00:00");
      CHECK(t4.to_string() == "23:59:59");
      CHECK(t5.to_string() == "12:34:56.078");
      CHECK(t6.to_string() == "12:34:56");
      CHECK(t7.to_string() == "12:34:56.078");
      CHECK(t8.to_string() == "12:34:56");
   }

   SECTION("24 hour format with argument")
   {
      CHECK(t0.to_string(false) == "00:00:00");
      CHECK(t1.to_string(false) == "11:59:59");
      CHECK(t2.to_string(false) == "12:00:00");
      CHECK(t3.to_string(false) == "13:00:00");
      CHECK(t4.to_string(false) == "23:59:59");
      CHECK(t5.to_string(false) == "12:34:56.078");
      CHECK(t6.to_string(false) == "12:34:56");
      CHECK(t7.to_string(false) == "12:34:56.078");
      CHECK(t8.to_string(false) == "12:34:56");
   }

   SECTION("12 hour format")
   {
      CHECK(t0.to_string(true) == "12:00:00am");
      CHECK(t1.to_string(true) == "11:59:59am");
      CHECK(t2.to_string(true) == "12:00:00pm");
      CHECK(t3.to_string(true) == "01:00:00pm");
      CHECK(t4.to_string(true) == "11:59:59pm");
      CHECK(t5.to_string(true) == "12:34:56.078pm");
      CHECK(t6.to_string(true) == "12:34:56pm");
      CHECK(t7.to_string(true) == "12:34:56.078pm");
      CHECK(t8.to_string(true) == "12:34:56pm");
   }
}

TEST_CASE ("is_am")
{
   Time t0{"05:00:00"};
   Time t1{"14:00:00"};
   Time t2{11,59,59,999};
   Time t3{12,00,00,000};
   CHECK       ( t0.is_am() );
   CHECK_FALSE ( t1.is_am() );
   CHECK       ( t2.is_am() );
   CHECK_FALSE ( t3.is_am() );
}

TEST_CASE("GetSeconds")
{
   Time t0{};
   Time t1{0,0,0,123};
   Time t2{0,0,1,123};
   Time t3{0,1,1,123};
   Time t4{1,1,1,123};
   CHECK( t0.GetSeconds() == 0.0f );
   CHECK( t1.GetSeconds() == 0.123f );
   CHECK( t2.GetSeconds() == 1.0f + 0.123f );
   CHECK( t3.GetSeconds() == 60.0f + 1.0f + 0.123f );
   CHECK( t4.GetSeconds() == 60.0f * 60.0f + 60.0f + 1.0f + 0.123f );
}

TEST_CASE("Comparison operators")
{
   Time t0{};
   Time t1{0,0,0,1};
   Time t2{"23:59:59.999"};
   Time t3{"00:00:00.000"};
   Time t4{1,2,3};
   Time t5{1,2,3,0};
   SECTION("operator<")
   {
      CHECK(t0 < t1);
      CHECK_FALSE(t2 < t3);
      CHECK_FALSE(t4 < t5);
   }
   SECTION("operator>")
   {
      CHECK_FALSE(t0 > t1);
      CHECK(t2 > t3);
      CHECK_FALSE(t4 > t5);
   }
   SECTION("operator<=")
   {
      CHECK(t0 <= t1);
      CHECK_FALSE(t2 <= t3);
      CHECK(t4 <= t5);
   }
   SECTION("operator>=")
   {
      CHECK_FALSE(t0 >= t1);
      CHECK(t2 >= t3);
      CHECK(t4 >= t5);
   }
   SECTION("operator==")
   {
      CHECK_FALSE(t0 == t1);
      CHECK_FALSE(t2 == t3);
      CHECK(t4 == t5);
   }
   SECTION("operator!=")
   {
      CHECK(t0 != t1);
      CHECK(t2 != t3);
      CHECK_FALSE(t4 != t5);
   }
}

TEST_CASE("operator<<")
{
   Time t0{};
   Time t1{1,2,3};
   Time t2{1,2,3,4};
   ostringstream oss{};
   SECTION("Test 1")
   {
      oss << t0;
      CHECK(oss.str() == t0.to_string());
   }
   SECTION("Test 2")
   {
      oss << t1;
      CHECK(oss.str() == t1.to_string());
   }
   SECTION("Test 3")
   {
      oss << t2;
      CHECK(oss.str() == t2.to_string());
   }
}

TEST_CASE("operator-")
{
   Time t0{};
   Time t1{1, 2, 3};
   Time t2{1, 2, 3, 250};
   Time t3{1, 2, 4, 500};
   Time t4{1, 3, 4, 500};
   Time t5{2, 3, 4, 500};
   CHECK(t1 - t0 == 60.0f * 60.0f + 2 * 60.0f + 3.0f);
   CHECK(t2 - t1 == 0.25f);
   CHECK(t4 - t3 == 60.0f);
   CHECK(t5 - t4 == 60.0f * 60.0f);
}