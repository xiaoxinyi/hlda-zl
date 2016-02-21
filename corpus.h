#ifndef CORPUS_H_
#define CORPUS_H_

#include <string>

#include "document.h"
#include "utils.h"

namespace hlda {

// A corpus containing a number of documents.
// The parameters of the GEM distribution: gem_mean_ 
// and gem_scale_ are also defined at the corpus level.
class Corpus {
  public:
  	Corpus();
  	Corpus(double gem_mean, double gem_scale);
  	~Corpus();

  	Corpus(const Corpus& from);
  	Corpus& operator=(const Corpus& from);
};
}  // namespace hlda

#endif