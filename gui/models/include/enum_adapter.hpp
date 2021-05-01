#pragma once
#include <QStringList>
#include <QVariant>

#include <optional>

namespace recipe {
namespace gui {
/*!
 * \brief Adapter class to enums for UI use
 * This class defines an adapter object for the use in Qt-UIs.
 * It converts between the enum it hosts and QStrings as well as integers including a limited bounds
 * check.
 * The "magic" of the object is done in the constructor, which has to be implemented for each enum
 * as a template specialization itself. In the source file for the enum_adapter, specializations for
 * meal_t, ingredient::amount_type, ingredient::category_t and amounted_ingredient::amount_t are
 * already provided. Here the member-fields of the object must be filled with sensible values.
 * This makes it possible to do the implementation for the rest of the methods here in the header
 * of this template class.
 */
template <typename T>
class enum_adapter
{
public:
  /*!
   * \brief Constructor.
   * If you want to use the class for an other enum than the four enums named in the general
   * description, you need to implement the constructor as template specialization for your enum.
   * You have to fill the fields: _map, _biggest and _smallest. Don't forget to call the private
   * method fillAll in the end.
   */
  enum_adapter();

  /*!
   * \brief Converts a string to the enum
   * \param value the string to convert
   * \return if the provided string is an exact match to the value provided for one of the enum
   *         values, the enum value is returned. Else a null-optional is returned.
   */
  std::optional<T> to_enum(QString const& value) const
  {
    auto it = std::find_if(_map.begin(), _map.end(),
                           [value](auto const& item) { return item.second == value; });
    if (it != _map.end()) {
      return it->first;
    }
    return {};
  }

  /*!
   * \brief Converts a QVariant to the enum
   * This is a convenience method, checking the value inside the QVariant for it's type before
   * conversion.
   * \param value the QVariant to convert
   * \return if the provided value corresponds to one of the enums values, the enum value is
   *         returned, else a null-optional is returned.
   */
  std::optional<T> to_enum(QVariant const& value) const
  {
    switch (value.type()) {
    case QVariant::Type::Int:
      return to_enum(value.toInt());
    case QVariant::Type::UInt:
      return to_enum(value.toUInt());
    case QVariant::Type::LongLong:
      return to_enum(value.toLongLong());
    case QVariant::Type::ULongLong:
      return to_enum(value.toULongLong());
    case QVariant::Type::String:
      return to_enum(value.toString());
    case QVariant::Type::ByteArray:
      return to_enum(QString::fromStdString(value.toByteArray().toStdString()));
    default:
      break;
    }
    return {};
  }

  /*!
   * \brief Converts an integer to the enum
   * In this method the limited bounds check is performed. It works for continuous enums, but is
   * only of limited use for enums that leave out values.
   * \param value the integer to convert
   * \return if the value is within bounds, the corresponding enum is returned, else a null-optional
   *         is returned.
   */
  std::optional<T> to_enum(int value) const
  {
    if (value < static_cast<int>(_smallest) || value > static_cast<int>(_biggest)) {
      return {};
    }
    return static_cast<T>(value);
  }

  /*!
   * \brief Get a list of all defined enum values.
   * \return A list of string representations of all enum values.
   */
  QStringList all() const { return _all; }

  /*!
   * \brief Converts an enum value to the corresponding string representation.
   * \note if the list provided with the constructor is not up to date, the method will return an
   * empty string upon unknown enum values.
   * \param value the value to convert
   * \return a string representation of the provided enum value.
   */
  QString to_string(T value) const
  {
    auto it = _map.find(value);
    if (it != _map.end()) {
      return it->second;
    }
    return {};
  }

private:
  /*!
   * \brief fillAll
   * Fills the list of all string representations of the enum-values. Should be called at the end
   * of the constructor.
   */
  void fillAll()
  {
    _all.clear();
    std::for_each(_map.begin(), _map.end(), [this](auto const& item) { _all.append(item.second); });
  }

  using map_t = std::map<T, QString>;
  /*!
   * \brief stores the defined enum values along with their string representations.
   */
  map_t _map;

  /*!
   * \brief for bounds check: stores the biggest possible enum value.
   */
  T _biggest;
  /*!
   * \brief for bounds check: stores the smallest possible enum value.
   */
  T _smallest;

  QStringList _all;
};
} // namespace gui
} // namespace recipe
