#include "Artist.hpp"
#include "Album.hpp"
#include "Track.hpp"

#include "List.hpp"
#include "TablePrinter.hpp"

#include "database/session.hpp"

#include <iostream>

using namespace oos;
using namespace std;

List::List()
  : Command("list")
{}

List::~List()
{}

void List::execute(oos::session &db, const std::vector<std::string> &args)
{
  if (!db.is_open()) {
    throw std::logic_error("database isn't open");
  } else {
    if (args.empty()) {
      throw std::logic_error("missing list argument");
    }

    db.load();

    //TablePrinter printer;
    
    
    if (args[0] == "artist") {
      TablePrinter<Artist> printer;
      printer.print(cout);
    } else if (args[0] == "album") {
      TablePrinter<Album> printer;
      printer.print(cout);
    } else if (args[0] == "track") {
      TablePrinter<Track> printer;
      printer.print(cout);
    } else {
      throw std::logic_error("invalid list type");
    }
  }
}