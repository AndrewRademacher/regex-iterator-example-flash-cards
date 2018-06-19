#include <catch.hpp>
#include <question.h>
#include <regex>
#include <iostream>

SCENARIO("Regex Matching") {

  WHEN("IS - T1A03 (D)") {
    const std::string source = "T1A03 (D)";
    REQUIRE(cards::is_question_identifier(source));
  }

  WHEN("IS - T1A04 (C) [97.3(a)(23)]") {
    const std::string source = "T1A04 (C) [97.3(a)(23)]";
    REQUIRE(cards::is_question_identifier(source));
  }

  WHEN("IS - T1A05 (A) [97.1 (e)]") {
    const std::string source = "T1A05 (A) [97.1 (e)]";
    REQUIRE(cards::is_question_identifier(source));
  }

  WHEN("FULL - G2C04 (D)") {
    const std::string source =
        "G2C04 (D) \n"
        "What does the Q signal \"QRL?\" mean?\n"
        "A. \"Will you keep the frequency clear?\" \n"
        "B. \"Are you operating full break-in\" or \"Can you operate full break-in?\"\n"
        "C. \"Are you listening only for a specific station?\" \n"
        "D. \"Are you busy?\", or \"Is this frequency in use?\"\n"
        "~~";
    std::smatch qmatch;
    REQUIRE(cards::is_question(source));
    REQUIRE(std::regex_match(source, qmatch, cards::make_question_pattern()));

    REQUIRE(qmatch[cards::QUESTION_GROUP_IDENTIFIER] == "G2C04");
    REQUIRE(qmatch[cards::QUESTION_GROUP_CORRECT_ANSWER] == "D");
    REQUIRE(qmatch[cards::QUESTION_GROUP_STATUTE_REFERENCE] == "");
    REQUIRE(qmatch[cards::QUESTION_GROUP_CONTENT] == "What does the Q signal \"QRL?\" mean?");
    REQUIRE(cards::has_answer(qmatch[cards::QUESTION_GROUP_ANSWERS]));
  }

  WHEN("FULL - G1A07 (C) [97.301(d)]") {
    const std::string source =
        "G1A07 (C) [97.301(d)]\n"
        "Which of the following frequencies is within the General Class portion of the 20-meter phone band?\n"
        "A. 14005 kHz\n"
        "B. 14105 kHz\n"
        "C. 14305 kHz\n"
        "D. 14405 kHz\n"
        "~~";
    std::regex pattern = cards::make_question_pattern();
    std::smatch qmatch;

    REQUIRE(cards::is_question(source));
    REQUIRE(std::regex_match(source, qmatch, pattern));

    REQUIRE(qmatch[cards::QUESTION_GROUP_IDENTIFIER] == "G1A07");
    REQUIRE(qmatch[cards::QUESTION_GROUP_CORRECT_ANSWER] == "C");
    REQUIRE(qmatch[cards::QUESTION_GROUP_STATUTE_REFERENCE] == "97.301(d)");
    REQUIRE(qmatch[cards::QUESTION_GROUP_CONTENT] == "Which of the following frequencies is within the General Class portion of the 20-meter phone band?");
    REQUIRE(cards::has_answer(qmatch[cards::QUESTION_GROUP_ANSWERS]));

  }

  WHEN("FULL - G1A08 (C) [97.301(d)]") {
    const std::string source =
        "G1A08 (C) [97.301(d)]  \n"
        "Which of the following frequencies is within the General Class portion of the 80-meter band?\n"
        "A. 1855 kHz\n"
        "B. 2560 kHz\n"
        "C. 3560 kHz\n"
        "D. 3650 kHz\n"
        "~~";
    std::regex pattern = cards::make_question_pattern();
    std::smatch qmatch;

    REQUIRE(cards::is_question(source));
    REQUIRE(std::regex_match(source, qmatch, pattern));

    REQUIRE(qmatch[cards::QUESTION_GROUP_IDENTIFIER] == "G1A08");
    REQUIRE(qmatch[cards::QUESTION_GROUP_CORRECT_ANSWER] == "C");
    REQUIRE(qmatch[cards::QUESTION_GROUP_STATUTE_REFERENCE] == "97.301(d)");
    REQUIRE(qmatch[cards::QUESTION_GROUP_CONTENT] == "Which of the following frequencies is within the General Class portion of the 80-meter band?");
    REQUIRE(cards::has_answer(qmatch[cards::QUESTION_GROUP_ANSWERS]));
  }

  WHEN("ANS - G2C04 (D)") {
    const std::string source =
        "G2C04 (D) \n"
        "What does the Q signal \"QRL?\" mean?\n"
        "A. \"Will you keep the frequency clear?\" \n"
        "B. \"Are you operating full break-in\" or \"Can you operate full break-in?\"\n"
        "C. \"Are you listening only for a specific station?\" \n"
        "D. \"Are you busy?\", or \"Is this frequency in use?\"\n"
        "~~";
    std::smatch qmatch;
    REQUIRE(std::regex_match(source, qmatch, cards::make_question_pattern()));

    std::regex apattern = cards::make_answer_pattern();
    auto ans_begin = std::sregex_iterator(source.begin(), source.end(), apattern);
    auto ans_end = std::sregex_iterator();
    std::smatch amatch;

    amatch = *ans_begin;
    REQUIRE(amatch[cards::ANSWER_GROUP_IDENTIFIER] == "A");
    REQUIRE(amatch[cards::ANSWER_GROUP_CONTENT] == "\"Will you keep the frequency clear?\" ");

    amatch = *(++ans_begin);
    REQUIRE(amatch[cards::ANSWER_GROUP_IDENTIFIER] == "B");
    REQUIRE(amatch[cards::ANSWER_GROUP_CONTENT] == "\"Are you operating full break-in\" or \"Can you operate full break-in?\"");

    amatch = *(++ans_begin);
    REQUIRE(amatch[cards::ANSWER_GROUP_IDENTIFIER] == "C");
    REQUIRE(amatch[cards::ANSWER_GROUP_CONTENT] == "\"Are you listening only for a specific station?\" ");

    amatch = *(++ans_begin);
    REQUIRE(amatch[cards::ANSWER_GROUP_IDENTIFIER] == "D");
    REQUIRE(amatch[cards::ANSWER_GROUP_CONTENT] == "\"Are you busy?\", or \"Is this frequency in use?\"");
  }
}