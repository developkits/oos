/*
 * This file is part of OpenObjectStore OOS.
 *
 * OpenObjectStore OOS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OpenObjectStore OOS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OpenObjectStore OOS. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef JSON_VALUE_HPP
#define JSON_VALUE_HPP

#ifdef WIN32
  #ifdef oos_EXPORTS
    #define OOS_API __declspec(dllexport)
    #define EXPIMP_TEMPLATE
  #else
    #define OOS_API __declspec(dllimport)
    #define EXPIMP_TEMPLATE extern
  #endif
  #pragma warning(disable: 4251)
#else
  #define OOS_API
#endif

#include <istream>
#ifdef WIN32
#include <memory>
#else
#include <tr1/memory>
#endif

namespace oos {

class json_object;
class json_bool;
class json_string;
class json_array;
class json_number;
class json_null;
class json_type;

class OOS_API json_value
{
public:
  json_value();
  json_value(const json_value &x);
  json_value(json_type *x);
  json_value(const std::string &x);
  json_value(const char *x);
  json_value(int x);
  json_value(double x);
  json_value(bool x);
  json_value& operator=(const json_value &x);
  json_value& operator=(json_type *x);
  json_value& operator=(const std::string &x);
  json_value& operator=(const char *x);
  json_value& operator=(double x);
  json_value& operator=(bool x);
  ~json_value();

  json_value& operator[](const std::string &key);
  json_value& operator[](size_t index);
  const json_value& operator[](size_t index) const;
  void push_back(const json_value &x);
  size_t size() const;

  static json_value create(std::istream &in);

  friend OOS_API std::istream& operator>>(std::istream &str, json_value &value);
  friend OOS_API std::ostream& operator<<(std::ostream &str, const json_value &value);

  template < class T >
  bool is_type() const
  {
    return dynamic_cast<const T*>(type_.get()) != 0;
  }

  template < class T >
  const T* value_type() const
  {
    return dynamic_cast<const T*>(type_.get());
  }

private:
  std::tr1::shared_ptr<json_type> type_;
};

}

#endif /* JSON_VALUE_HPP */