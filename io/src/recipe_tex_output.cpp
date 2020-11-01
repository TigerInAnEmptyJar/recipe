#include "recipe_tex_output.h"

#include <algorithm>
#include <fstream>
#include <functional>
#include <map>

namespace {
auto recipeWriter = [](recipe::recipe const& r, std::ostream& o) -> void {
  o << "\\section*{" << r.title() << "}\n";
  o << "\\subsection*{Zutaten}\n";
  o << "\\begin{multicols}{2}\n";
  o << "\\begin{itemize*}\n";
  std::for_each(r.begin(), r.end(), [&o](recipe::amounted_ingredient const& i) {
    o << "\\item " << i.base_ingredient().name() << "\n";
  });
  o << "\\end{itemize*}\n";
  o << "\\columnbreak\n";
  if (!r.image_path().empty()) {
    o << "\\includegraphics[width=0.39\textwidth]{" << r.image_path().native() << "}\n";
  }
  o << "\\end{multicols}\n";
  o << "\\subsection*{Zubereitung}\n";
  o << "\\begin{enumerate*}\n";
  auto i = r.instructions();
  auto start = i.begin();
  auto end = std::find(i.begin(), i.end(), '\n');
  while (end != i.end()) {
    o << "\\item " << std::string{start, end} << "\n";
    start = end + 1;
    end = std::find(start, i.end(), '\n');
  }
  o << "\\item " << std::string{start, end} << "\n";
  o << "\\end{enumerate*}\n";
  o << "\\pagebreak\n";
};

void writeEmptyPlan(recipe::plan const& p, std::ostream& o)
{
  int ncols = std::max_element(p.begin(), p.end(),
                               [](recipe::plan_item const& p1, recipe::plan_item const& p2) {
                                 return p1.subscribers().size() < p2.subscribers().size();
                               })
                  ->subscribers()
                  .size();
  o << "\\begin{tabular}{ r l";
  std::string empty;
  for (int i = 0; i < ncols; i++) {
    o << " c";
  }
  o << " }\n";
  for (int i = 0; i < ncols; i++)
    empty += " &";
  // header
  o << "Tag & Rezept &" << empty << "\\\\\n";
  std::for_each(p.begin(), p.end(), [&o, empty](recipe::plan_item const& p1) {
    o << p1.name() << " & ";
    std::for_each(p1.begin(), p1.end(), [&o](recipe::recipe const& r) { o << r.title() << ", "; });
    o << " & " << empty << "\\\\\n";
  });
  o << "\\end{tabular}\n";
  o.flush();
}

void writePlan(recipe::plan const& p, std::ostream& o)
{
  int ncols = std::max_element(p.begin(), p.end(),
                               [](recipe::plan_item const& p1, recipe::plan_item const& p2) {
                                 return p1.subscribers().size() < p2.subscribers().size();
                               })
                  ->subscribers()
                  .size();
  o << "\\begin{tabular}{ r l";
  std::string empty;
  for (int i = 0; i < ncols; i++) {
    o << " c";
  }
  o << " }\n";
  for (int i = 0; i < ncols; i++)
    empty += " &";
  // header
  o << "Tag & Rezept &" << empty << "\\\\\n";
  std::for_each(p.begin(), p.end(), [&o](recipe::plan_item const& p1) {
    o << p1.name() << " & ";
    std::for_each(p1.begin(), p1.end(), [&o](recipe::recipe const& r) { o << r.title() << ", "; });
    o << " & ";
    std::copy(p1.subscribers().begin(), p1.subscribers().end(),
              std::ostream_iterator<std::string>(o, " & "));
    o << "\\\\\n";
  });
  o << "\\end{tabular}\n";
  o.flush();
}

void writePlanAndRecipes(recipe::plan const& p, std::ostream& o)
{
  writePlan(p, o);
  o << "\\pagebreak\n";
  std::for_each(p.begin(), p.end(), [&o](recipe::plan_item const& pe) {
    std::for_each(pe.begin(), pe.end(), [&o](auto const& r) {
      recipeWriter(r, o);
      o << "\\pagebreak\n";
    });
  });
}

void writeShoppingList(recipe::plan const& p, std::ostream& o)
{
  std::vector<std::vector<recipe::amounted_ingredient>> shoppinglist;

  std::vector<std::vector<recipe::amounted_ingredient>>::iterator current;
  std::for_each(p.begin(), p.end(), [&shoppinglist, &current](recipe::plan_item const& pe) {
    if (pe.shoppingBefore() || shoppinglist.empty()) {
      shoppinglist.push_back({});
      current = shoppinglist.begin();
      std::advance(current, shoppinglist.size() - 1);
    }
    std::for_each(pe.begin(), pe.end(), [&shoppinglist, &current, &pe](recipe::recipe const& r) {
      std::for_each(
          r.begin(), r.end(), [&shoppinglist, &current, &pe, &r](recipe::amounted_ingredient i) {
            i *= pe.subscribers().size() / r.servings();
            if (i.base_ingredient().sectioned()) {
              std::vector<recipe::amounted_ingredient>::iterator it =
                  std::find_if(current->begin(), current->end(), [&i](auto const& element) {
                    return i.base_ingredient().id() == element.base_ingredient().id();
                  });
              if (it != current->end()) {
                it->add(i);
              } else {
                current->push_back(i);
              }
            } else {
              auto it =
                  std::find_if(shoppinglist.begin()->begin(), shoppinglist.begin()->end(),
                               [&i](auto const& element) {
                                 return i.base_ingredient().id() == element.base_ingredient().id();
                               });
              if (it != shoppinglist.begin()->end()) {
                it->add(i);
              } else {
                shoppinglist.begin()->push_back(i);
              }
            }
          });
    });
  });
  std::for_each(shoppinglist.begin(), shoppinglist.end(), [&o](auto const& shop) {
    o << "Shopping:\\\\\n \\begin{tabular}{ l l }\n";
    std::for_each(shop.begin(), shop.end(), [&o](recipe::amounted_ingredient const& ing) {
      std::string toShop;
      std::for_each(ing.begin(), ing.end(), [&toShop](auto const& element) {
        switch (element.first) {
        case recipe::amounted_ingredient::amount_t::liter:
          toShop += std::to_string(element.second) + " l, ";
          break;
        case recipe::amounted_ingredient::amount_t::milliliter:
          toShop += std::to_string(element.second) + " ml, ";
          break;
        case recipe::amounted_ingredient::amount_t::tea_spoon:
          toShop += std::to_string(element.second) + " Tl, ";
          break;
        case recipe::amounted_ingredient::amount_t::table_spoon:
          toShop += std::to_string(element.second) + " El, ";
          break;
        case recipe::amounted_ingredient::amount_t::pince:
          toShop += std::to_string(element.second) + " Priese, ";
          break;
        case recipe::amounted_ingredient::amount_t::cups:
          toShop += std::to_string(element.second) + " cups, ";
          break;
        case recipe::amounted_ingredient::amount_t::grams:
          toShop += std::to_string(element.second) + " g, ";
          break;
        case recipe::amounted_ingredient::amount_t::kg:
          toShop += std::to_string(element.second) + " kg, ";
          break;
        case recipe::amounted_ingredient::amount_t::ounces:
          toShop += std::to_string(element.second) + " unzen, ";
          break;
        case recipe::amounted_ingredient::amount_t::pounds:
          toShop += std::to_string(element.second) + " Pfund, ";
          break;
        case recipe::amounted_ingredient::amount_t::piece:
          toShop += std::to_string(element.second) + " Stc, ";
          break;
        case recipe::amounted_ingredient::amount_t::bundle:
          toShop += std::to_string(element.second) + " Bund, ";
          break;
        case recipe::amounted_ingredient::amount_t::can:
          toShop += std::to_string(element.second) + " Dosen, ";
          break;
        }
      });
      if (!toShop.empty()) {
        o << ing.base_ingredient().name() << " & " << toShop.substr(0, toShop.length() - 2)
          << "\\\\\n";
      }
    });
    o << "\\end{tabular}\n";
  });
  o.flush();
}

std::map<recipe::io::recipe_tex_output::plan_mode,
         std::function<void(recipe::plan const&, std::ostream&)>> const writers = {
    {recipe::io::recipe_tex_output::plan_mode::plan_to_register,
     [](recipe::plan const& p, std::ostream& o) { writeEmptyPlan(p, o); }},
    {recipe::io::recipe_tex_output::plan_mode::plan_final,
     [](recipe::plan const& p, std::ostream& o) { writePlan(p, o); }},
    {recipe::io::recipe_tex_output::plan_mode::plan_with_recipes,
     [](recipe::plan const& p, std::ostream& o) { writePlanAndRecipes(p, o); }},
    {recipe::io::recipe_tex_output::plan_mode::shoppinglist,
     [](recipe::plan const& p, std::ostream& o) { writeShoppingList(p, o); }},
};
} // namespace
namespace recipe {
namespace io {

class recipe_tex_output_impl : public recipe_tex_output
{
public:
  void write(plan const& out, plan_mode mode, std::filesystem::path const& path) const override;
  void write(plan const& out, plan_mode mode, std::ostream& output) const override;
  void write(recipe const& out, std::filesystem::path const& path) const override;
  void write(recipe const& out, std::ostream& output) const override;
  void writeHeader(std::ostream& output) const override;
  void writeFooter(std::ostream& output) const override;
};

void recipe_tex_output_impl::write(plan const& out, plan_mode mode,
                                   std::filesystem::path const& path) const
{
  std::ofstream output;
  output.open(path);
  writeHeader(output);
  write(out, mode, output);
  writeFooter(output);
  output.close();
}

void recipe_tex_output_impl::write(plan const& out, plan_mode mode, std::ostream& output) const
{
  auto plan_writer = ::writers.find(mode);
  if (plan_writer != ::writers.end()) {
    plan_writer->second(out, output);
  }
}

void recipe_tex_output_impl::write(recipe const& out, std::filesystem::path const& path) const
{
  std::ofstream output;
  output.open(path);
  writeHeader(output);
  write(out, output);
  writeFooter(output);
  output.close();
}

void recipe_tex_output_impl::write(recipe const& out, std::ostream& path) const
{
  ::recipeWriter(out, path);
}

void recipe_tex_output_impl::writeHeader(std::ostream& output) const
{
  output << "\\documentclass[a4paper,12pt]{article}\n";
  output << "\\usepackage[german]{babel}\n";
  output << "\\usepackage{geometry}\n";
  output << "\\usepackage{multicol}\n";
  output << "\\usepackage{vwcol}\n";
  output << "\\usepackage{mdwlist}\n";
  output << "\\usepackage{float}\n";
  output << "\\usepackage{graphicx}\n";
  output << "\\usepackage{wrapfig}\n";
  output << "\\usepackage{amsmath}\n";
  output << "\\geometry{bmargin=3cm,tmargin=2cm,lmargin=2cm,rmargin=2cm}\n";
  output << "\\title{Rezepte}\n";
  output << "\\begin{document}\n";
  output.flush();
}

void recipe_tex_output_impl::writeFooter(std::ostream& output) const
{
  output << "\\end{document}" << std::flush;
}

} // namespace io
} // namespace recipe
