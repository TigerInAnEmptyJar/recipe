#pragma once

#include "shopping_list.h"

#include <filesystem>
#include <functional>
#include <optional>
#include <vector>

namespace recipe::io {

//! This is the base class for all io operations for shopping-lists
class shopping_io
{
public:
  //! The function-type for providing access to the stored ingredients.
  using finder_t = std::function<std::optional<ingredient>(boost::uuids::uuid)>;

  /*!
   * \brief write
   * writes a shopping-list to file
   * \param out the shopping list to write
   * \param path the file-path to write to
   */
  virtual void write(shopping_list const& out, std::filesystem::path const& path) const = 0;

  /*!
   * \brief read
   * reads a shopping-list from file. Since the ingredients are in most cases not written
   * directly but referenced, a method to find the referenced ingredients in other structures
   * is needed
   * \param path the file to read from
   * \param ingredient_finder the finder for the referenced ingredients
   * \return the read shopping list, if it could be read.
   */
  virtual std::optional<shopping_list> read(std::filesystem::path const& path,
                                            finder_t ingredient_finder) const = 0;

  /*!
   * \brief serialize
   * serialize the shopping list to a string.
   * \param out the shopping-list to serialize
   * \return a string representation of the shopping list
   */
  virtual std::string serialize(shopping_list const& out) const = 0;

  /*!
   * \brief deserialize
   * deserialize a shopping list from a string. Since the ingredients are in most cases not written
   * directly but referenced, a method to find the referenced ingredients in other structures
   * is needed
   * \param text the string to deserialize from
   * \param ingredient_finder the finder for the referenced ingredients
   * \return the deserialized shopping list, if it could be deserialized
   */
  virtual std::optional<shopping_list> deserialize(std::string const& text,
                                                   finder_t ingredient_finder) const = 0;
};

} // namespace recipe::io
