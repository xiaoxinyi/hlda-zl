
#ifndef DOCUMENT_H_
#define DOCUMENT_H_

#include <string>
#include <vector>

#include "topic.h"

namespace hlda {

	// A word having an id, a word count and the level
	// in the tree the is assigned to.
	class Word {
	public:
		Word(int id, int count, int level);
		~Word();

		Word(const Word& from);
		Word& operator=(const Word& from);

		void setLevel(int level) { level_ = level; }
		void updateLevel(int value) { level_ += value; }
		int getLevel() const { return level_; }

		void setId(int id) { id_ = id; }
		int getId() const { return id_; }

	private:
		// Word id.
		int id_;
		// Word count in the document.
		int count_;
		// Each word has assigned a level in the tree.
		int level_;
	};

	// The document containing a number of words.
	// A document has an id, a document score, 
	// a topic path from the root of the tree to leaf 
	// and statistics for words assigned to different
	// levels in the tree - level counts and logprobilities
	// for the levels.
	class Document {
	public:
		 Document();
		 Document(int id, int depth);
		~ Document();

		Document(const Document& from);
		Document& operator=(const Document& from);

		int getWords() const { return words_.size(); }
		void addWord(int word_id, int word_count, int level);
		Word* getMutableWord(int i) { return &words_.at(i); }
		void setWords(int i, const Word& word) { words_.at(i) = word; }

		const vector<Word>& getCstWordVector() const { return words_; }
		void setWords(const vector<Word>& words) { words_ = words; }

		double getScore() const { return score_; }
		void setScore(double score) { score_ = score; }

		void setPathTopic(int level, Topic* topic) { path_[level] = topic; }
		Topic* getMutablePathTopic(int level) const { return path_[level]; }

		int getLevelCounts(int level) const { return level_counts_[level]; }
		int getSumLevelCounts(int depth) const;
		void updateLevelCounts(int level, int value) {
			level_counts_[level] += value;
		}

		double getLogPrLevel(int depth) const { return log_pr_level_[depth]; }
		void computeLogPrLevel(double gem_mean, double gem_scale, int level);

		int getId() const { return id_; }

		// Set the word level counts and 
		// log probabilities for the level
		// (level_counts_ and log_pr_level_) to 0.
		void initLevelCounts(int depth);

	private:
		// Document id.
		int id_;

		// The words in the document
		vector<Word> words_;

		// Topic path from the root of the tree to a leaf.
		vector<Topic*> path_;

		// Depth of the tree.
		int depth_;

		// Level counts.
		int* level_counts_;

		// Log p(level) which is unormalized.
		double* log_pr_level_;

		// Document score.
		double score_;
	};

	// This class provides functionality for sampling the 
	// word levels (assigning a level in the tree for the
	// words) for a given document.
	// and permuting the words in a document.
	class DocumentUtils {
	public:
		// Sample the word levels for a given document,
		// for the current path assignments of the document.
		// Sampling can be with (permute = 1) or without
		// (permute != 1) permuting the words in document.
		// Words can or cannot be removed from levels in 
		// the tree (set/unset the bool remove varible).
		// The GEM distuibution mean and scale parameters 
		// determined at corpus level are provided as input.
		static void SampleLevels(
			Document* document,
			int permute_words,
			bool remove,
			double gem_mean,
			double gem_scale);

		// Permute the words in a document.
		static void PermuteWords(Document* document);
		
	};

	// This class provides functionality for sampling 
	// the path in tree for document,
	// for removing a path assigned to a document from 
	// a tree and updating the tree topic given a document.
	class DocumentTreeUtils {
	public:
		// Sample the path of a document starting from 
		// a particular level.
		// Given the level allocation, sample the path
		// associated with each document conditioned on 
		// all other paths and the observed words.
		static void SampleDocumentPath(
			Tree* tree,
			Document* document,
			bool remove,
			int start_level);

		// Remove a path assigned to a document from a tree,
		// given a particular start level.
		static void RemoveDocumentFromPath(
			Tree* tree,
			Document* document,
			int start_level);

		// Update the topic from a document beginning at
		// a specific level, by updating the word and 
		// document counts.
		static void UpdateTreeFromDocument(
			Document* document,
			int update,
			int start_level);

	};

	// This class provides functionality for building the
	// topic path for a document, and for computing path
	// probabilities.
	class DocumentTopicUtils {
	public:
		// Fill in topic path for this document.
		static void AddPathToDocument(
			Topic* topic,
			Document* document,
			int start_level);

		// Compute path probilities by traversing the tree
		// depth-first.
		static void ProbabilitiesDfs(
			Topic* topic,
			Document* document,
			double* log_sum,
			vector<double>* path_pr,
			int start_level);
	private:
		// Log gamma ratio computation used to 
		// compute the path probabilities.
		static double LogGammaRatio(
			Document* document,
			Topic* topic,
			int level,
			double eta,
			int word_no);
	};


} // namespace hlda

#endif // DOCUMET_H_
