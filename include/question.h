#pragma once

#include <string>
#include <vector>
#include <regex>
#include <boost/iostreams/device/mapped_file.hpp>
#include <ostream>
#include <nlohmann/json.hpp>

namespace cards {

extern const char QUESTION_IDENTIFIER_PATTERN[];

extern const char QUESTION_PATTERN[];
extern const size_t QUESTION_GROUP_IDENTIFIER;
extern const size_t QUESTION_GROUP_CORRECT_ANSWER;
extern const size_t QUESTION_GROUP_STATUTE_REFERENCE;
extern const size_t QUESTION_GROUP_CONTENT;
extern const size_t QUESTION_GROUP_ANSWERS;

extern const char ANSWER_PATTERN[];
extern const size_t ANSWER_GROUP_IDENTIFIER;
extern const size_t ANSWER_GROUP_CONTENT;

bool is_question_identifier(const std::string& source);
bool is_question(const std::string& source);
bool is_answer(const std::string& source);
bool has_answer(const std::string& source);

std::regex make_question_identifier_pattern();
std::regex make_question_pattern();
std::regex make_answer_pattern();

class answer {
public:
  char identifier() const;
  answer& identifier(char identifier);

  const std::string& content() const;
  answer& content(std::string content);

private:
  char m_identifier;
  std::string m_content;
};

void to_json(nlohmann::json& j, const answer& a);
void from_json(const nlohmann::json& j, answer& a);

class question {
public:
  const std::string& identifier() const;
  question& identifier(std::string identifier);

  char correct_answer() const;
  question& correct_answer(char correct);

  const std::string& statute_reference() const;
  question& statute_reference(std::string ref);

  const std::string& content() const;
  question& content(std::string content);

  const std::vector<answer>& answers() const;
  question& answers(std::vector<answer> answers);

private:
  std::string m_identifier;
  char m_correct_answer;
  std::string m_statute_reference;
  std::string m_content;
  std::vector<answer> m_answers;
};

void to_json(nlohmann::json& j, const question& q);
void from_Json(const nlohmann::json& j, question& q);

class question_parse_error : public std::exception {
public:
  question_parse_error(std::string what);

  const char* what() const noexcept override;
private:
  std::string m_what;
};

question extract_question(std::cregex_iterator itr);
std::vector<answer> extract_answers(const std::string& answers);

}
