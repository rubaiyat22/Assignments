#ifndef SEQUENCEMAP_H
#define SEQUENCEMAP_H

#include <vector>

//SequenceMap is a non-templated class that stores a string recognition_sequence_ and a vector of
//strings enzyme_acronyms_. The big-fives are left as defaults. 
//Functions completed:
	//<> SequenceMap(const string &a_rec_seq, const string &an_enz_acro)
	//<> bool operator < (const SequenceMap &rhs) const
	//<> friend std::ostream &operator << (std::ostream &out, const SequenceMap & a_sequece)
	//<> void Merge(const SequenceMap & other_sequence)

//Sample Uses:
	// BinarySearchTree <SequenceMap > a_tree;
	// SequenceMap new_map ("Name of recognition sequence", vector of enzyme acronyms);
	// a_tree.insert(new_map);
	// cout << new_map << endl;
class SequenceMap
{
public:
	SequenceMap();
	//Two Param Constructor
	// Constructs two a SequenceMap from two strings 
	SequenceMap(const string &a_rec_seq, const string &an_enz_acro)
	{
		recognition_sequence_ = a_rec_seq;
		enzyme_acronyms_.push_back(an_enz_acro); 
	}
	//Copy Constructor
	SequenceMap (const SequenceMap & rhs) = default;
	//Copy Assignment Operator
	SequenceMap & operator = (SequenceMap & rhs) = default;
	//Move Constructor
	SequenceMap (SequenceMap && rhs) = default;
	//Move Assignment Operator
	SequenceMap & operator = (SequenceMap && rhs)= default;

	//---------------------------- End of Big Five ------------------------------------//

	//compares the recognition_sequence of this object and rhs.recognition_sequence_
	bool operator <(const SequenceMap &rhs) const
	{
		return (recognition_sequence_ < rhs.recognition_sequence_) ? true : false;
	}

	//outputs the enzyme acronyms for every Recogition sequence
	friend std::ostream &operator << (std::ostream &out, const SequenceMap & a_sequece)
	{
		out << "Enzyme Acronyms: ";	
		for (auto enz_acr: a_sequece.enzyme_acronyms_)
			out << enz_acr <<" ";
		out << std::endl;
		return out;
	}

	//This function assumes that this objects's recognition_sequence_ is equal to 
	//other_sequence.recognition_sequence_
	//merges the enzyme_acronyms of both seqeunces together
	//other_sequence does not change
	void Merge(const SequenceMap & other_sequence)
	{
		for (unsigned int i=0; i<other_sequence.enzyme_acronyms_.size();i++)
			enzyme_acronyms_.push_back(other_sequence.enzyme_acronyms_[i]);
	}

private:
	std::string recognition_sequence_ ; 
	vector<std::string> enzyme_acronyms_;
};

#endif
