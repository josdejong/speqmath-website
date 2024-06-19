
#include <string>

#include "GeneralFunctions.h"

using namespace std;

class Mimetype
{
public:
  Mimetype();
  Mimetype(const string& filename);
  void set(const string& filename);
  string get();
  bool isKnownType();

private:
  struct MIME
  {
    const char* extension;
    const char* type;
  };
  static MIME Mimetypes_[];
  int mimeCount_;

  int mimeId_;
  string type_;
};
