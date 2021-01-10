#pragma once

#include <filesystem>
#include <fstream>
#include <vector>

namespace recipe {
namespace io {

class tex_writer
{
public:
  std::ostream& openDocument(std::filesystem::path const& path);
  void writeHeader(std::ostream& output) const;
  void writeFooter(std::ostream& output) const;
  void writeTable(std::vector<std::vector<std::string>> const& table, std::ostream& output,
                  bool rotatedHeader = false) const;
  std::string toTex(std::string const& line) const;
  std::string rotated(std::string const& value, int rotation) const;

private:
  std::ofstream _output;
};

} // namespace io
} // namespace recipe
