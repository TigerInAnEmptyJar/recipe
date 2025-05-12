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

void recipe_tex_writer::write(std::vector<recipe> const& out, std::ostream & output) const
{
  tex_writer tex;
  std::filesystem::path databasePath = "/home/katharina/Documents/recipes";

  tex.writeHeader(output);
  std::for_each(std::begin(out), std::end(out), [this, &output, databasePath](auto r) {
    write(r, output, databasePath);
    output << "\\pagebreak\n";
  });
  tex.writeFooter(output);
  output << std::flush;
}

void recipe_tex_writer::write(recipe const& r, std::ostream& output,
                              std::filesystem::path const& databasePath) const
{
  output << "\\section*{" << r.title() << "}\n";
  output << "\\subsection*{Zutaten}\n";
  output << "\\begin{multicols}{2}\n";
  output << "\\begin{itemize*}\n";
  auto uniter = [](amounted_ingredient::amount_t a) -> std::string {
    using T = amounted_ingredient::amount_t;
    static std::map<T, std::string> const values{
        {T::liter, "l"},      {T::milliliter, "ml"}, {T::cups, "cups"},      {T::grams, "g"},
        {T::kg, "kg"},        {T::ounces, "ounce"},  {T::pounds, "Pfund"},   {T::piece, "Stc."},
        {T::bundle, "Bund"},  {T::can, "Dosen"},     {T::table_spoon, "El"}, {T::tea_spoon, "Tl"},
        {T::pince, "Priese"},
    };
    auto it = values.find(a);
    if (it != values.end()) {
      return it->second;
    }
    return "";
  };
  auto amounter = [uniter = std::move(uniter)](
                      amounted_ingredient const& ingredient) -> std::optional<std::string> {
    if (ingredient.begin() == ingredient.end()) {
      return std::nullopt;
    }
    if (std::distance(ingredient.begin(), ingredient.end()) == 1) {
      std::stringstream s;
      s << ingredient.begin()->second << " " << uniter(ingredient.begin()->first);
      return s.str();
    }
    return std::nullopt;
  };
  std::for_each(r.begin(), r.end(),
                [&output, amounter = std::move(amounter)](amounted_ingredient const& i) {
                  auto a = amounter(i);
                  if (a) {
                    output << "\\item " << *a << " " << i.base_ingredient().name() << "\n";
                  }
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
  output << "N\\\"ahrwerte (" << static_cast<int>(r.servings()) << "): " << r.calories() << " kcal, " << r.joules()
         << " kJ, Protein: " << static_cast<int>(r.g_proteins())
         << " g, Fett: " << static_cast<int>(r.g_fat())
         << " g, Kohlenhydrate: " << static_cast<int>(r.g_carbohydrates()) << "g\n";
}

} // namespace io
} // namespace recipe
