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

  	void setWordNo(int word_no) { word_no_ = word_no; }
  	int getWordNo() const { return word_no_; }

  	void AddDocument(const Document& docuemnt) {
  		documents_.push_back(document);
  	}
  	int getDocuments() const { return documents_.size(); }
  	Document* getMutableDocument(int i) { return &documents_.at(i); }
  	void setDocuments(const vector<Document>& documents) {
  		documents_ = documents;
  	}

  	double getGemMean() const { return gem_mean_; }
  	void setGemMean(double gem_mean) { gem_mean_ = gem_mean; }

  	double getGemScale() const { return gem_sacle; }
  	void setGemScale(double gem_scale) { gem_scale_ = gem_scale; }

  private:
  	// Parameters of the GEM distribution.
  	// gem_mean shows the proportion of the general words
  	// relative to specific words.
  	double gem_mean_;

  	// gem_scale shows how strictly documents should follow
  	// general versus specific word proportions.
  	double gem_scale_;

  	// The number of distinct words in the corpus.
  	int word_no_;

  	// The documents in the corpus.
  	vector<Document> documents_;
};

// This class provides funcionality for reading a corpus
// from a file, computing and updating the GEM distribution
// parameters and permuting the documents in the corpus.
class CorpusUtils {
  public:
  	// Read corpus from a file.
  	static void ReadCorpus(
  		const std::string& filename,
  		Corpus* corpus,
  		int depth);

  	// Corpus level GEM score.
  	static double GemScore(
  		Corpus* corpus);

  	// Update the GEM scale parameter.
  	// The new GEM scale parameter is based on Gaussian random variates.
  	// Repeat REP_NO_GEM number of times.
  	static void UpdateGemScale(Corpus* corpus);

  	// Update the GEM scale parameter.
  	// The new GEM scale parameter is based on Gaussian random variates.
  	// Repeat REP_NO_GEM number of times.
  	static void UpdateGemMean(Corpus* corpus);

  	// Permute the documents in the corpus.
  	void PermuteDocuments(Corpus* corpus);
};

}  // namespace hlda

#endif  // CORPUS_H_