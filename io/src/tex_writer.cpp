#include "tex_writer.hpp"

#include <filesystem>
#include <iterator>
#include <map>

namespace {
template <typename T>
void joinOnStream(std::ostream& output, std::vector<T> const& line, std::string const& separator)
{
  auto count = line.size();
  std::for_each(std::begin(line), std::end(line), [&output, &count, separator](auto item) {
    output << item;
    count--;
    if (count > 0) {
      output << separator;
    }
  });
}
} // namespace
namespace recipe {
namespace io {

std::ostream& tex_writer::openDocument(std::filesystem::path const& path)
{
  _output.open(path.native());
  return _output;
}

void tex_writer::writeHeader(std::ostream& output) const
{
  output << "\\documentclass[a4paper,12pt]{article}\n";
  output << "\\usepackage[german]{babel}\n";
  output << "\\usepackage{geometry}\n";
  output << "\\usepackage{multicol}\n";
  output << "\\usepackage{rotating}\n";
  output << "\\usepackage{mdwlist}\n";
  output << "\\usepackage{float}\n";
  output << "\\usepackage{graphicx}\n";
  output << "\\usepackage{amsmath}\n";
  output << "\\usepackage{soul}\n";
  output << "\\setlength\\columnsep{50pt}\n";
  output << "\\geometry{bmargin=2cm,tmargin=2cm,lmargin=2cm,rmargin=2cm}\n";
  output << "\\begin{document}\n";
  output.flush();
}

void tex_writer::writeFooter(std::ostream& output) const
{
  output << "\\end{document}" << std::flush;
}

void tex_writer::writeTable(const std::vector<std::vector<std::string>>& table,
                            std::ostream& output, bool rotatedHeader) const
{
  output << "\\begin{tabular}{|";
  ::joinOnStream(output, table[0], "|");
  output << "|}\n\\hline \n";

  auto start = (std::begin(table) + 1);
  if (rotatedHeader) {
    start++;
    std::vector<std::string> tex_line;
    std::transform(std::begin(table[1]), std::end(table[1]), std::back_inserter(tex_line),
                   [this](auto const& element) { return rotated(toTex(element), 90); });
    ::joinOnStream(output, tex_line, "&");
    output << "\\\\ \\hline \n";
  }
  std::for_each(start, std::end(table), [&output, this](auto const& line) {
    std::vector<std::string> tex_line;
    std::transform(std::begin(line), std::end(line), std::back_inserter(tex_line),
                   [this](auto const& element) { return toTex(element); });
    ::joinOnStream(output, tex_line, "&");
    output << "\\\\ \\hline \n";
  });
  output << "\\end{tabular}\n";
}

std::string tex_writer::toTex(std::string const& line) const
{
  std::string result;
  static std::map<std::string, std::string> const replacement{
      std::make_pair("\\", "\\\\"), std::make_pair("ä", "\\\"a"),  std::make_pair("Ä", "\\\"A"),
      std::make_pair("ö", "\\\"o"), std::make_pair("Ö", "\\\"O"),  std::make_pair("ü", "\\\"u"),
      std::make_pair("Ü", "\\\"U"), std::make_pair("ß", "\\ss{}"), std::make_pair("é", "\\´e"),
      std::make_pair("è", "\\`e"),  std::make_pair("î", "\\^i"),
  };

  std::for_each(std::begin(line), std::end(line), [&result](auto item) {
    auto found = replacement.find(std::string{item});
    if (found != std::end(replacement)) {
      result.append(found->second);
    }
    result.append(std::string{item});
  });
  return result;
}

std::string tex_writer::rotated(std::string const& value, int rotation) const
{
  return "\\begin{turn}{" + std::to_string(rotation) + "}" + value + "\\end{turn}";
}

} // namespace io
} // namespace recipe
