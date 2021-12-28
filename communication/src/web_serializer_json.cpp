#include "web_serializer_json.hpp"

#define RAPIDJSON_HAS_STDSTRING 1
#include <rapidjson/document.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/writer.h>
#include <rapidjson/prettywriter.h>
#include <fstream>

#include <boost/uuid/uuid_io.hpp>

#include <sstream>
#include <iostream>

namespace {
constexpr char const key_recipe_title[] = "title";
constexpr char const key_recipe_id[] = "id";
constexpr char const key_recipe_mealtype[] = "mealType";
constexpr char const key_recipe_servings[] = "servings";
constexpr char const key_recipe_instructions[] = "instructions";
constexpr char const key_recipe_preparation_time[] = "preparation";
constexpr char const key_recipe_source[] = "source";
constexpr char const key_recipe_calories[] = "calories";
constexpr char const key_recipe_joules[] = "joules";
constexpr char const key_recipe_fats[] = "fat";
constexpr char const key_recipe_proteins[] = "protein";
constexpr char const key_recipe_carbohydrates[] = "carbs";
constexpr char const key_recipe_ingredients[] = "ingredients";
constexpr char const key_recipe_ingredient_name[] = "name";
constexpr char const key_recipe_ingredient_id[] = "id";
constexpr char const key_recipe_ingredient_unit[] = "unit";
constexpr char const key_recipe_ingredient_value[] = "value";

constexpr char const key_ingredient_name[] = "name";
constexpr char const key_ingredient_id[] = "id";
constexpr char const key_ingredient_category[] = "category";

constexpr char const key_plan_name[] = "name";
constexpr char const key_plan_id[] = "id";
constexpr char const key_plan_recipes[] = "recipes";
constexpr char const key_plan_recipe_name[] = "name";
constexpr char const key_plan_recipe_id[] = "id";
constexpr char const key_plan_subscribers[] = "subscribers";
constexpr char const key_plan_shopping_before[] = "shoppingBefore";
constexpr char const key_plan_days[] = "days";
constexpr char const key_plan_meals[] = "meals";
constexpr char const key_plan_items[] = "items";
constexpr char const key_plan_eaters[] = "eaters";

constexpr char const key_shopping_name[] = "name";
constexpr char const key_shopping_id[] = "id";
constexpr char const key_shopping_days[] = "days";
constexpr char const key_shopping_day_name[] = "name";
constexpr char const key_shopping_ingredients[] = "ingredients";
constexpr char const key_shopping_ingredient_name[] = "name";
constexpr char const key_shopping_ingredient_id[] = "id";
constexpr char const key_shopping_ingredient_list[] = "list";
constexpr char const key_shopping_ingredient_unit[] = "unit";
constexpr char const key_shopping_ingredient_value[] = "value";
constexpr char const key_shopping_ingredient_has_all[] = "hasAll";
constexpr char const key_shopping_ingredient_at_home_unit[] = "unitAtHome";
constexpr char const key_shopping_ingredient_at_home_value[] = "valueAtHome";

template <typename Writer>
void serialize_recipe(Writer& writer, recipe::recipe const& input)
{
    writer.StartObject();
    writer.String(key_recipe_title);
    writer.String(input.title());

    writer.String(key_recipe_id);
    writer.String(boost::uuids::to_string(input.id()));

    writer.String(key_recipe_mealtype);
    writer.Int(static_cast<int>(input.meal_type()));

    writer.String(key_recipe_servings);
    writer.Int(input.servings());

    writer.String(key_recipe_instructions);
    writer.String(input.instructions());

    writer.String(key_recipe_preparation_time);
    writer.Int(input.preparation_time().count());

    writer.String(key_recipe_source);
    writer.String(input.source());

    writer.String(key_recipe_calories);
    writer.Int(input.calories());

    writer.String(key_recipe_joules);
    writer.Int(input.joules());

    writer.String(key_recipe_fats);
    writer.Int(input.g_fat());

    writer.String(key_recipe_proteins);
    writer.Int(input.g_proteins());

    writer.String(key_recipe_carbohydrates);
    writer.Int(input.g_carbohydrates());

    writer.String(key_recipe_ingredients);
    writer.StartArray();
    std::for_each(std::begin(input), std::end(input), [&writer](auto const& element){
        writer.StartObject();
        writer.String(key_recipe_ingredient_name);
        writer.String(element.base_ingredient().name());
        writer.String(key_recipe_ingredient_id);
        writer.String(boost::uuids::to_string(element.base_ingredient().id()));
        writer.String(key_recipe_ingredient_unit);
        writer.Int(static_cast<int>(element.begin()->first));
        writer.String(key_recipe_ingredient_value);
        writer.Double(element.begin()->second);
        writer.EndObject();
    });
    writer.EndArray();
    writer.EndObject();
}

template <typename Writer>
void serialize_ingredient(Writer& writer, recipe::ingredient const& input)
{
    writer.StartObject();
    writer.String(key_ingredient_name);
    writer.String(input.name());

    writer.String(key_ingredient_id);
    writer.String(boost::uuids::to_string(input.id()));

    writer.String(key_ingredient_category);
    writer.Int(static_cast<int>(input.category()));

    writer.EndObject();
}

template <typename Writer>
void serialize_plan(Writer& writer, recipe::plan const& input)
{
    writer.StartObject();
    writer.String(key_plan_name);
    writer.String(input.name());

    writer.String(key_plan_id);
    writer.String(boost::uuids::to_string(input.id()));

    writer.String(key_plan_days);
    writer.Int( input.days());

    writer.String(key_plan_meals);
    writer.Int(input.meals());

    writer.String(key_plan_eaters);
    writer.StartArray();
    for(auto const& eater: input.eaterList()) {
        writer.String(eater);
    }
    writer.EndArray();

    writer.String(key_plan_items);
    writer.StartArray();
    for(auto const& item: input) {
        writer.StartObject();

        writer.String(key_plan_shopping_before);
        writer.Bool(item.shoppingBefore());

        writer.String(key_plan_name);
        writer.String(item.name());

        writer.String(key_plan_recipes);
        writer.StartArray();
        for(auto const& rec: item) {
            writer.StartObject();
            writer.String(key_plan_recipe_name);
            writer.String(rec.item().title());
            writer.String(key_plan_recipe_id);
            writer.String(boost::uuids::to_string(rec.item().id()));

            writer.String(key_plan_subscribers);
            writer.StartArray();
            for (auto const& eater : rec.subscribers()) {
              writer.String(eater);
            }
            writer.EndArray();

            writer.EndObject();
        }
        writer.EndArray();
        writer.EndObject();
    }
    writer.EndArray();
    writer.EndObject();
}

template <typename Writer>
void serialize_shopping(Writer& writer, recipe::shopping_list const& input)
{
    writer.StartObject();
    writer.String(key_shopping_name);
    writer.String(input.name());

    writer.String(key_shopping_id);
    writer.String(boost::uuids::to_string(input.id()));

    writer.String(key_shopping_days);
    writer.StartArray();
    for(auto const&item: input) {
        writer.StartObject();
        writer.String(key_shopping_day_name);
        writer.String(item.name());

        writer.String(key_shopping_ingredients);
        writer.StartArray();
        for(auto const& ingred: item) {
            writer.StartObject();
            writer.String(key_shopping_ingredient_name);
            writer.String(std::get<0>(ingred).base_ingredient().name());
            writer.String(key_shopping_ingredient_id);
            writer.String(boost::uuids::to_string(std::get<0>(ingred).base_ingredient().id()));
            writer.String(key_shopping_ingredient_has_all);
            writer.Bool(std::get<3>(ingred));
            writer.String(key_shopping_ingredient_at_home_unit);
            writer.Int(static_cast<int>(std::get<1>(ingred)));
            writer.String(key_shopping_ingredient_at_home_value);
            writer.Double(std::get<2>(ingred));

            writer.String(key_shopping_ingredient_list);
            writer.StartArray();
            for(auto const& ing: std::get<0>(ingred)) {
                writer.StartObject();
                writer.String(key_shopping_ingredient_unit);
                writer.Int(static_cast<int>(ing.first));
                writer.String(key_shopping_ingredient_value);
                writer.Double(ing.second);
                writer.EndObject();
            }
            writer.EndArray();

            writer.EndObject();
        }
        writer.EndArray();
        writer.EndObject();
    }
    writer.EndArray();
    writer.EndObject();
}
}

namespace recipe::communication {

std::string web_serializer_json::serialize(recipe const& input) const
{
  std::stringstream output;
  rapidjson::OStreamWrapper wrapper(output);
  rapidjson::Writer<rapidjson::OStreamWrapper> writer(wrapper);
  serialize_recipe<rapidjson::Writer<rapidjson::OStreamWrapper>>(writer, input);

  return output.str();
}

std::string web_serializer_json::serialize(ingredient const & input) const
{
    std::stringstream output;
    rapidjson::OStreamWrapper wrapper(output);
    rapidjson::Writer<rapidjson::OStreamWrapper> writer(wrapper);
    serialize_ingredient<rapidjson::Writer<rapidjson::OStreamWrapper>>(writer, input);

    return output.str();
}

std::string web_serializer_json::serialize(plan const& input) const
{
    std::stringstream output;
    rapidjson::OStreamWrapper wrapper(output);
    rapidjson::Writer<rapidjson::OStreamWrapper> writer(wrapper);
    serialize_plan<rapidjson::Writer<rapidjson::OStreamWrapper>>(writer, input);

    return output.str();
}

std::string web_serializer_json::serialize(shopping_list const& input) const
{
    std::stringstream output;
    rapidjson::OStreamWrapper wrapper(output);
    rapidjson::Writer<rapidjson::OStreamWrapper> writer(wrapper);
    serialize_shopping<rapidjson::Writer<rapidjson::OStreamWrapper>>(writer, input);

    return output.str();
}

std::shared_ptr<web_serializer> create_web_serializer()
{
    return std::make_shared<web_serializer_json>();
}

}
