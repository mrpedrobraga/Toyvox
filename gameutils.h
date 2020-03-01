#include "tvxutil.h"

template <class T>
class Database {
private:
  std::unordered_map<std::string, T> table;
public:

  void load(std::string file)
  {
    std::string file = load_file_as_string("file");
    //TODO: Import database stuff.
  }
};
