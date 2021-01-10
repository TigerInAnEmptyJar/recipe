#include "recipe_tex_writer.hpp"

#include "tex_writer.hpp"

namespace recipe {
namespace io {

void recipe_tex_writer::write(std::vector<recipe> const& out,
                              std::filesystem::path const& path) const
{
  tex_writer tex;
  auto& output = tex.openDocument(path);
  auto databasePath = path.parent_path() / "recipes";

  tex.writeHeader(output);
  std::for_each(std::begin(out), std::end(out), [this, &output, databasePath](auto r) {
    write(r, output, databasePath);
    output << "\\pagebreak\n";
  });
  tex.writeFooter(output);
  output << std::flush;
}

std::optional<std::vector<recipe>> recipe_tex_writer::read(std::filesystem::path const&,
                                                           finder_t) const
{
  return {};
}

void recipe_tex_writer::write(recipe const& r, std::ostream& output,
                              std::filesystem::path const& databasePath) const
{
  output << "\\section*{" << r.title() << "}\n";
  output << "\\subsection*{Zutaten}\n";
  output << "\\begin{multicols}{2}\n";
  output << "\\begin{itemize*}\n";
  std::for_each(r.begin(), r.end(), [&output](amounted_ingredient const& i) {
    output << "\\item " << i.base_ingredient().name() << "\n";
  });
  output << "\\end{itemize*}\n";
  output << "\\columnbreak\n";
  if (!r.image_path().empty()) {
    output << "\\includegraphics[width=0.39\\textwidth]{"
           << (databasePath / r.image_path()).native() << "}\n";
  }
  output << "\\end{multicols}\n";
  output << "\\subsection*{Zubereitung}\n";
  output << "\\begin{enumerate*}\n";
  auto i = r.instructions();
  auto start = i.begin();
  auto end = std::find(i.begin(), i.end(), '\n');
  while (end != i.end()) {
    output << "\\item " << std::string{start, end} << "\n";
    start = end + 1;
    end = std::find(start, i.end(), '\n');
  }
  output << "\\item " << std::string{start, end} << "\n";
  output << "\\end{enumerate*}\n";
}

} // namespace io
} // namespace recipe
