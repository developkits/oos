#ifndef DATABASE_TEST_UNIT_HPP
#define DATABASE_TEST_UNIT_HPP

#include "object/object_store.hpp"

#include "unit/unit_test.hpp"

class DatabaseTestUnit : public oos::unit_test
{
public:
  DatabaseTestUnit();
  virtual ~DatabaseTestUnit();

  virtual void initialize();
  virtual void finalize();

  void basic();

private:
  oos::object_store ostore_;
};

#endif /* DATABASE_TEST_UNIT_HPP */
