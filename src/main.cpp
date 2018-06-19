#include <iostream>
#include <question.h>
#include <boost/program_options.hpp>
#include <boost/iostreams/device/mapped_file.hpp>
#include <fstream>

namespace po = boost::program_options;

//void outputJson(const std::string& inputFile, const std::string& outputFile);
//void outputCsv(const std::string& inputFile, const std::string& outputFile);
void output(const std::string& inputFile, const std::string& outputFile, std::function<void(std::ostream&, const cards::question&)> onRecord);
void outputJson(std::ostream& out, const cards::question& q);
void outputCsv(std::ostream& out, const cards::question& q);

int main(int argc, char** argv) {
  po::options_description desc("flash <options> input-files...");
  desc.add_options()
      ("help", "show help message")
      ("json", "output in json")
      ("csv", "comma separated values")
      ("output-file", po::value<std::string>(), "file to dump results to")
      ("input-file", "file to read questions from");

  po::positional_options_description p;
  p.add("input-file", 1);

  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv)
    .options(desc).positional(p).run(), vm);
  po::notify(vm);

  if (vm.count("help")) {
    std::cout << desc << '\n';
    return EXIT_FAILURE;
  }

  if (vm.count("json") && vm.count("csv")) {
    std::cout << "Can only use one output type." << '\n';
    return EXIT_FAILURE;
  }

  try {
    std::string inputFile;
    if (!vm.count("input-file")) throw std::invalid_argument("missing input file path");
    else inputFile = vm["input-file"].as<std::string>();

    std::string outputFile;
    if (vm.count("output-file")) outputFile = vm["output-file"].as<std::string>();
    else outputFile = inputFile + ".output";

    if (vm.count("json")) output(inputFile, outputFile, outputJson);
    else if (vm.count("csv"))
      output(inputFile, outputFile, outputCsv);
    else throw std::invalid_argument("no output format selected");
  } catch (const std::exception& e) {
    std::cout << "FAILURE: " << e.what() << '\n';
    return EXIT_FAILURE;
  }
}

void output(const std::string& inputFile, const std::string& outputFile, std::function<void(std::ostream&, const cards::question&)> onRecord) {
  boost::iostreams::mapped_file_source source(inputFile);
  std::ofstream sink(outputFile);

  std::regex pattern(cards::QUESTION_PATTERN);
  std::cregex_iterator begin(source.begin(), source.end(), pattern);
  std::cregex_iterator end;

  size_t count = 0;

  for(; begin != end; begin++, count++) {
    cards::question q = cards::extract_question(begin);
    onRecord(sink, q);
  }

  std::cout << "Translated " << count << " items." << '\n';
}

void outputJson(std::ostream& out, const cards::question& q) {
  nlohmann::json j = q;
  out << j << '\n';
}

void outputCsv(std::ostream& out, const cards::question& q) {
  out << q.content() << ';' << q.correct_answer() << '\n';
//  out << '"' << '"' << q.content() << '"' << '"' << '\n';
//  out << q.identifier() << ": " << q.content() << '' << q.correct_answer() << '\n';
//  out << '"' << q.content() << '"' << '\t' << q.correct_answer() << '\n';
//  out << '"' << q.identifier() << ":\n" << q.content() << '"';
//  out << ',';
//
//  out << '"';
//  for (auto itr = q.answers().begin(); itr != q.answers().end(); itr++) {
//    const cards::answer& a = *itr;
//    out << "- " << a.content();
//    if (a.identifier() == q.correct_answer()) out << " (x)";
//    if (itr + 1 != q.answers().end()) out << '\n';
//  }
//  out << '"';
//  out << '\n';
}