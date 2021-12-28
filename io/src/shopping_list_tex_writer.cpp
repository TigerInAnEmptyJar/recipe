#include "shopping_list_tex_writer.hpp"

#include "tex_writer.hpp"

#include <cmath>
#include <ios>
#include <numeric>
#include <sstream>

namespace {
std::string to_string(recipe::amounted_ingredient::amount_t amount)
{
  switch (amount) {
  case recipe::amounted_ingredient::amount_t::liter:
    return "l";
  case recipe::amounted_ingredient::amount_t::milliliter:
    return "ml";
  case recipe::amounted_ingredient::amount_t::cups:
    return "cups";
  case recipe::amounted_ingredient::amount_t::grams:
    return "g";
  case recipe::amounted_ingredient::amount_t::kg:
    return "kg";
  case recipe::amounted_ingredient::amount_t::ounces:
    return "ounces";
  case recipe::amounted_ingredient::amount_t::pounds:
    return "pd";
  case recipe::amounted_ingredient::amount_t::piece:
    return "Stc";
  case recipe::amounted_ingredient::amount_t::bundle:
    return "Bd";
  case recipe::amounted_ingredient::amount_t::can:
    return "Dose";
  case recipe::amounted_ingredient::amount_t::table_spoon:
    return "El";
  case recipe::amounted_ingredient::amount_t::tea_spoon:
    return "Tl";
  case recipe::amounted_ingredient::amount_t::pince:
    return "Priese";
  }
  return {};
}

std::string to_string(recipe::ingredient::category_t category)
{
  switch (category) {
  case recipe::ingredient::category_t::fruit:
    return "Obst";
  case recipe::ingredient::category_t::vegetable:
    return "Gemüse";
  case recipe::ingredient::category_t::meat:
    return "Fleisch";
  case recipe::ingredient::category_t::fish:
    return "Fisch";
  case recipe::ingredient::category_t::spices:
    return "Gewürze";
  case recipe::ingredient::category_t::diary:
    return "Milchprodukte";
  case recipe::ingredient::category_t::noodles:
    return "Reis und Nudeln";
  case recipe::ingredient::category_t::baking:
    return "Backen";
  case recipe::ingredient::category_t::drink:
    return "Getränke";
  case recipe::ingredient::category_t::frozen:
    return "Tiefkühl";
  case recipe::ingredient::category_t::canned:
    return "Dosen";
  }
  return {};
}

std::string to_string(float value)
{
  if (fabs(value) < 0.001) {
    return "0";
  }
  std::stringstream output;
  output << std::setprecision(3) << std::noshowpoint;
  output << value;
  return output.str();
}
} // namespace

namespace recipe::io {

void shopping_tex_writer::write(shopping_list const& out, std::filesystem::path const& path) const
{
  auto result = serialize(out);

  std::ofstream output(path);
  output << result << std::flush;
}

std::optional<shopping_list> shopping_tex_writer::read(std::filesystem::path const&, finder_t) const
{
  return {};
}

std::string shopping_tex_writer::serialize(shopping_list const& out) const
{
  tex_writer tex;
  std::stringstream output;
  tex.writeHeader(output);

  output << "\\begin{multicols}{2}\n";
  std::for_each(std::begin(out), std::end(out), [&output, &tex](auto const& item) {
    output << "\\begin{tabular}{l r l}\n";
    output << item.name() << "\\\\ \n";
    std::map<ingredient::category_t, std::vector<std::string>> sorted;
    std::for_each(std::begin(item), std::end(item),
                  [&sorted, &tex](shopping_day::value_type const& element) {
                    std::string name = tex.toTex(std::get<0>(element).base_ingredient().name());
                    std::string atHome =
                        ::to_string(std::get<2>(element)) + " " + ::to_string(std::get<1>(element));
                    std::string needed = std::accumulate(
                        std::begin(std::get<0>(element)), std::end(std::get<0>(element)),
                        std::string{}, [](auto const& init, auto const& el) {
                          return init + " + " + ::to_string(el.second) + ::to_string(el.first);
                        });
                    if (needed.length() > 0) {
                      needed = needed.substr(3);
                    }
                    if (std::get<3>(element)) {
                      atHome = "";
                      name = "\\st{" + tex.toTex(name) + "}";
                      needed = "\\st{" + needed + "}";
                    }
                    sorted[std::get<0>(element).base_ingredient().category()].push_back(
                        name + " & " + needed + " & " + atHome + " \\\\ \n");
                  });
    std::for_each(std::begin(sorted), std::end(sorted), [&output](auto const& singleList) {
      output << "\\hline \n\\end{tabular}\\\\ \n\\begin{tabular}{l r l}\n \\textbf{"
             << ::to_string(singleList.first) << "} & & \\\\ \n";
      std::for_each(std::begin(singleList.second), std::end(singleList.second),
                    [&output](auto const& element) { output << element; });
    });
    output << "\\end{tabular}\n";
  });
  output << "\\end{multicols}\n";

  tex.writeFooter(output);
  return output.str();
}

std::optional<shopping_list> shopping_tex_writer::deserialize(std::string const&, finder_t) const
{
  return {};
}

} // namespace recipe::io
