#include "ingredient.h"

#include <boost/uuid/random_generator.hpp>

namespace recipe {

ingredient::ingredient(std::string const& name, category_t cat, amount_type amount, bool sect,
                       std::filesystem::path const& path)
    : _name(name), _category(cat), _amount(amount), _sectioned(sect), _image_path(path)
{
}

std::string ingredient::name() const { return _name; }

ingredient::category_t ingredient::category() const { return _category; }

ingredient::amount_type ingredient::default_amount() const { return _amount; }

bool ingredient::sectioned() const { return _sectioned; }

std::filesystem::path ingredient::image_path() const { return _image_path; }

void ingredient::name(std::string const& n) { _name = n; }

void ingredient::category(ingredient::category_t c) { _category = c; }

void ingredient::default_amount(ingredient::amount_type a) { _amount = a; }

void ingredient::sectioned(bool s) { _sectioned = s; }

void ingredient::image_path(std::filesystem::path const& p) { _image_path = p; }

boost::uuids::uuid ingredient::id() const { return _id; }

void ingredient::id(boost::uuids::uuid i) { _id = i; }

boost::uuids::uuid ingredient::generateId() { return boost::uuids::random_generator_mt19937{}(); }

} // namespace recipe
