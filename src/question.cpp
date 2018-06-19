#include <regex>
#include <iostream>
#include "question.h"

extern const char cards::QUESTION_IDENTIFIER_PATTERN[] = "([A-Z0-9]+)+\\s\\(([A-Z])\\)\\s*(\\[([[0-9.a-z\\s\\(\\)]+)\\])*";

extern const char cards::QUESTION_PATTERN[] = "([A-Z0-9]+)+\\s*\\(([A-Z])\\)\\s*(\\[([[0-9.a-z\\s\\(\\)\\-,]+)\\]*)*\\s*([A-Za-z,\\\" 0-9\\-?.\\(\\)\\/\\'\\”\\“\\’:;$µ+ ]+)\\n([A-Za-z\\\",\\s0-9\\-?.\\(\\)\\/\\”\\“\\'\\’:;$µ+– ]+)\\n~+";
extern const size_t cards::QUESTION_GROUP_IDENTIFIER = 1;
extern const size_t cards::QUESTION_GROUP_CORRECT_ANSWER = 2;
extern const size_t cards::QUESTION_GROUP_STATUTE_REFERENCE = 4;
extern const size_t cards::QUESTION_GROUP_CONTENT = 5;
extern const size_t cards::QUESTION_GROUP_ANSWERS = 6;

extern const char cards::ANSWER_PATTERN[] = "([ABCD]).\\s([A-Za-z\\\", 0-9-?.]+)";
extern const size_t cards::ANSWER_GROUP_IDENTIFIER = 1;
extern const size_t cards::ANSWER_GROUP_CONTENT = 2;

bool cards::is_question_identifier(const std::string& source) {
  static const std::regex pattern(QUESTION_IDENTIFIER_PATTERN);
  return std::regex_match(source, pattern);
}

bool cards::is_question(const std::string& source) {
  static const std::regex pattern(QUESTION_PATTERN);
  return std::regex_match(source, pattern);
}

bool cards::is_answer(const std::string& source) {
  static const std::regex pattern(ANSWER_PATTERN);
  return std::regex_match(source, pattern);
}

std::regex cards::make_question_identifier_pattern() {
  return std::regex(QUESTION_IDENTIFIER_PATTERN);
}

std::regex cards::make_question_pattern() {
  return std::regex(QUESTION_PATTERN);
}

std::regex cards::make_answer_pattern() {
  return std::regex(ANSWER_PATTERN);
}

bool cards::has_answer(const std::string& source) {
  static const std::regex pattern(ANSWER_PATTERN);
  return std::regex_search(source, pattern);
}

char cards::answer::identifier() const {
  return m_identifier;
}

cards::answer& cards::answer::identifier(char identifier) {
  m_identifier = identifier;
  return *this;
}

const std::string& cards::answer::content() const {
  return m_content;
}

cards::answer& cards::answer::content(std::string content) {
  m_content = std::move(content);
  return *this;
}

const std::string& cards::question::identifier() const {
  return m_identifier;
}

cards::question& cards::question::identifier(std::string identifier) {
  m_identifier = identifier;
  return *this;
}

char cards::question::correct_answer() const {
  return m_correct_answer;
}

cards::question& cards::question::correct_answer(char correct) {
  m_correct_answer = correct;
  return *this;
}

const std::string& cards::question::statute_reference() const {
  return m_statute_reference;
}

cards::question& cards::question::statute_reference(std::string ref) {
  m_statute_reference = ref;
  return *this;
}

const std::string& cards::question::content() const {
  return m_content;
}

cards::question& cards::question::content(std::string content) {
  m_content = content;
  return *this;
}

const std::vector<cards::answer>& cards::question::answers() const {
  return m_answers;
}

cards::question& cards::question::answers(std::vector<cards::answer> answers) {
  m_answers = std::move(answers);
  return *this;
}

cards::question cards::extract_question(std::cregex_iterator itr) {
  const std::cmatch qmatch = *itr;

  const char* tmpcorrect = qmatch[QUESTION_GROUP_CORRECT_ANSWER].first;

  return question()
      .identifier(qmatch[QUESTION_GROUP_IDENTIFIER].str())
      .correct_answer(*tmpcorrect != '\0' ? *tmpcorrect : throw question_parse_error("missing correct answer"))
      .statute_reference(qmatch[QUESTION_GROUP_STATUTE_REFERENCE].str())
      .content(qmatch[QUESTION_GROUP_CONTENT].str())
      .answers(extract_answers(qmatch[QUESTION_GROUP_ANSWERS].str()));
}

std::vector<cards::answer> cards::extract_answers(const std::string& answers) {
  const static std::regex apattern(cards::ANSWER_PATTERN);
  std::sregex_iterator sitr = std::sregex_iterator(answers.begin(), answers.end(), apattern);
  std::vector<answer> output;

  for (; sitr != std::sregex_iterator(); sitr++) {
    std::smatch amatch = *sitr;

    output.push_back(answer()
                         .identifier(amatch[ANSWER_GROUP_IDENTIFIER].str().front())
                         .content(amatch[ANSWER_GROUP_CONTENT].str()));
  }

  return output;
}

void cards::to_json(nlohmann::json& j, const cards::question& q) {
  j["identifier"] = q.identifier();
  j["correct_answer"] = q.correct_answer();
  j["statute_reference"] = q.statute_reference();
  j["content"] = q.content();
  j["answers"] = q.answers();
}

void cards::from_Json(const nlohmann::json& j, cards::question& q) {
  q.identifier(j.at("identifier").get<std::string>());
  q.correct_answer(j.at("correct_answer").get<char>());
  q.statute_reference(j.at("statute_reference").get<std::string>());
  q.content(j.at("content").get<std::string>());
  q.answers(j.at("answers").get<std::vector<answer>>());
}

void cards::to_json(nlohmann::json& j, const cards::answer& a) {
  j["identifier"] = a.identifier();
  j["content"] = a.content();
}

void cards::from_json(const nlohmann::json& j, cards::answer& a) {
  a.identifier(j.at("identifier").get<char>());
  a.content(j.at("content").get<std::string>());
}

const char* cards::question_parse_error::what() const noexcept {
  return m_what.data();
}

cards::question_parse_error::question_parse_error(std::string what)
  : m_what(std::move(what)) {}