#include <catch.hpp>
#include <question.h>
#include <iostream>

SCENARIO("Mapped file iterator") {

  GIVEN("a mapped question file iterator on a small file") {
    boost::iostreams::mapped_file_source source("../pools/small_test_file.txt");
    std::regex pattern(cards::QUESTION_PATTERN);
    std::cregex_iterator begin(source.begin(), source.end(), pattern);
    std::cregex_iterator end;

    WHEN("reading the first question") {
      cards::question first = cards::extract_question(begin);

      REQUIRE(first.identifier() == "G1A01");
      REQUIRE(first.correct_answer() == 'C');
      REQUIRE(first.statute_reference() == "97.301(d)");
      REQUIRE(first.content() == "On which of the following bands is a General Class license holder granted all amateur frequency privileges?");

      REQUIRE(first.answers()[0].identifier() == 'A');
      REQUIRE(first.answers()[0].content() == "60, 20, 17, and 12 meters");
    }

    WHEN("reading all questions") {
      size_t count = 0;

      for (; begin != end; begin++) {
        cards::question q = cards::extract_question(begin);
        if (!q.identifier().empty()) count++;
      }

      REQUIRE(count == 2);
    }
  }
}