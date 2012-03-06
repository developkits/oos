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

#ifndef SQLITE_TYPES_HPP
#define SQLITE_TYPES_HPP

#include "database/sqlite/sqlite_statement.hpp"

namespace oos {

namespace sqlite {

class sqlite_types
{
public:
  static const char *select_postfix() { return "SELECT"; }
  static const char *char_type_string() { return "INTEGER"; }
  static const char *float_type_string() { return "REAL"; }
  static const char *double_type_string() { return "REAL"; }
  static const char *int_type_string() { return "INTEGER"; }
  static const char *long_type_string() { return "INTEGER"; }
  static const char *unsigned_int_type_string() { return "INTEGER"; }
  static const char *bool_type_string() { return "INTEGER"; }
  static const char *char_ptr_type_string() { return "TEXT"; }
  static const char *string_type_string() { return "TEXT"; }
  static const char *varchar_type_string() { return "VARCHAR"; }
  static const char *object_type_string() { return "INTEGER"; }
  static const char *primary_key_prefix() { return "PRIMARY KEY NOT NULL"; }
};

}

}

#endif /* SQLITE_TYPES_HPP */
