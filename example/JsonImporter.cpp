#include "JsonImporter.hpp"

#include "database/session.hpp"

#include "object/object_store.hpp"
#include "object/object.hpp"

#ifdef WIN32
  #pragma warning(disable: 4355)
#endif

JsonImporter::JsonImporter(oos::session &db)
  : generic_json_parser<JsonImporter>(this)
  , db_(db)
  , step_(unknown)
{}

JsonImporter::~JsonImporter()
{}

void JsonImporter::parse(std::istream &in)
{
  step_ = unknown;

  this->parse_json(in);
}

void JsonImporter::on_begin_object()
{
  switch (step_) {
    case unknown:
      throw std::logic_error("invalid json format");
    case artist:
      obj_ = db_.ostore().insert(db_.ostore().create("artist"));
    case album:
      obj_ = db_.ostore().insert(db_.ostore().create("album"));
    case track:
      obj_ = db_.ostore().insert(db_.ostore().create("track"));
    default:
      throw std::logic_error("invalid json format");
  }
}

void JsonImporter::on_object_key(const std::string &key)
{
  switch (step_) {
    case unknown:
      throw std::logic_error("invalid json format");
    case artist:
    case album:
    case track:
      // Todo: check if object contains a field with the given key
      key_ = key;
    default:
      throw std::logic_error("invalid json format");
  }

}

void JsonImporter::on_end_object()
{
}

void JsonImporter::on_begin_array()
{
  switch (step_) {
    case unknown:
      step_ = artist;
      break;
    case artist:
      step_ = album;
      break;
    case album:
      step_ = track;
      break;
    case track:
    default:
      throw std::logic_error("invalid json format");
  }
}

void JsonImporter::on_end_array()
{
  switch (step_) {
    case unknown:
      throw std::logic_error("invalid json format");
    case artist:
      step_ = unknown;
      break;
    case album:
      step_ = artist;
      break;
    case track:
      step_ = album;
      break;
    default:
      throw std::logic_error("invalid json format");
  }
}

void JsonImporter::on_string(const std::string &value)
{
  obj_->set(key_, value);
}

void JsonImporter::on_number(double value)
{
//  obj_->set(key_, value);
}

void JsonImporter::on_bool(bool value)
{
  obj_->set(key_, value);
}

void JsonImporter::on_null()
{
}