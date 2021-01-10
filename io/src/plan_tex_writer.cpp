#include "plan_tex_writer.hpp"

#include "recipe_tex_writer.hpp"
#include "tex_writer.hpp"

namespace {
std::vector<std::vector<std::string>> generateTable(recipe::plan const& out, bool filled)
{
  auto eaters = out.eaterList();
  std::function<std::vector<std::string>(recipe::plan_item const&, recipe::recipe const&)>
      createLine = [eaters](recipe::plan_item const& element, recipe::recipe const& recipe) {
        std::vector<std::string> line;
        line.push_back(element.name());
        line.push_back(recipe.title());
        std::transform(std::begin(eaters), std::end(eaters), std::back_inserter(line),
                       [&element](auto const& eater) -> std::string {
                         auto const& subscribers = element.subscribers();
                         if (std::find(std::begin(subscribers), std::end(subscribers), eater) !=
                             std::end(subscribers)) {
                           return "X";
                         }
                         return "";
                       });
        return line;
      };

  if (!filled) {
    createLine = [columns = eaters.size()](recipe::plan_item const& element,
                                           recipe::recipe const& recipe) {
      std::vector<std::string> line(columns + 2, "");
      line[0] = element.name();
      line[1] = recipe.title();
      return line;
    };
  }
  std::vector<std::string> header(eaters.size(), "c");
  header.insert(std::begin(header), "l");
  header.insert(std::begin(header), "r");
  eaters.insert(std::begin(eaters), "Rezept");
  eaters.insert(std::begin(eaters), "Tag");
  std::vector<std::vector<std::string>> table;
  table.push_back(header);
  table.push_back(eaters);
  std::for_each(std::begin(out), std::end(out), [&table, &eaters, createLine](auto const& element) {
    std::for_each(std::begin(element), std::end(element),
                  [&table, element, eaters, createLine](auto const& recipe) {
                    table.push_back(createLine(element, recipe));
                  });
  });
  return table;
}
} // namespace

namespace recipe {
namespace io {

void plan_tex_writer_register::write(plan const& out, std::filesystem::path const& path) const
{
  tex_writer tex;
  auto& output = tex.openDocument(path);

  tex.writeHeader(output);
  output << "\\title{" << out.name() << "}\n\\maketitle\n";
  tex.writeTable(::generateTable(out, false), output, true);
  tex.writeFooter(output);
  output << std::flush;
}

std::optional<plan> plan_tex_writer_register::read(std::filesystem::path const&, finder_t) const
{
  return {};
}

void plan_tex_writer_final::write(plan const& out, std::filesystem::path const& path) const
{
  tex_writer tex;
  auto& output = tex.openDocument(path);

  tex.writeHeader(output);
  output << "\\title{" << out.name() << "}\n\\maketitle\n";
  tex.writeTable(::generateTable(out, true), output, true);
  tex.writeFooter(output);
  output << std::flush;
}

std::optional<plan> plan_tex_writer_final::read(std::filesystem::path const&, finder_t) const
{
  return {};
}

void plan_tex_writer_with_recipes::write(plan const& out, std::filesystem::path const& path) const
{
  tex_writer tex;
  auto& output = tex.openDocument(path);

  tex.writeHeader(output);
  output << "\\title{" << out.name() << "}\n\\maketitle\n";
  tex.writeTable(::generateTable(out, true), output, true);
  output << "\\pagebreak\n";

  recipe_tex_writer recipe_writer;

  std::set<boost::uuids::uuid> written_ids;
  std::for_each(std::begin(out), std::end(out),
                [&output, &recipe_writer, &written_ids, &path](auto const& element) {
                  std::for_each(std::begin(element), std::end(element),
                                [&output, &recipe_writer, &written_ids, &path](auto const& recipe) {
                                  if (written_ids.find(recipe.id()) == written_ids.end()) {
                                    recipe_writer.write(recipe, output,
                                                        path.parent_path() / "recipes");
                                    output << "\\pagebreak\n";
                                    written_ids.insert(recipe.id());
                                  }
                                });
                });

  tex.writeFooter(output);
  output << std::flush;
}

std::optional<plan> plan_tex_writer_with_recipes::read(std::filesystem::path const&, finder_t) const
{
  return {};
}

} // namespace io
} // namespace recipe
