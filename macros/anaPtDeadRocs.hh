#ifndef anaPtDeadRocs_h
#define anaPtDeadRocs_h

#include "anaPixelTree.hh"

class anaPtDeadRocs: public anaPixelTree {
public :

  anaPtDeadRocs(const char *dir, int ischain);
  virtual ~anaPtDeadRocs();
  virtual void     Loop(int nevt = -1);
};


anaPtDeadRocs::anaPtDeadRocs(const char *dir, int ischain) : anaPixelTree(dir, ischain) {
}

anaPtDeadRocs::~anaPtDeadRocs() {
  cout << "This is the end " << endl;
}

#endif // #ifdef anaPtDeadRocs_cxx
