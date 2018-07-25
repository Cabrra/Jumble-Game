#pragma once

#include "SLList.h"

template<typename Type>
class HTable
{
private:
	unsigned int buckets;
	unsigned int(*f) (const Type&);
	SLList<Type>* myPointer;

public:

	HTable(unsigned int numOfBuckets, unsigned int(*hFunction) (const Type &v));
	~HTable();
	HTable<Type>& operator=(const HTable<Type>& that);
	HTable(const HTable<Type>& that);
	void insert(const Type& v);
	bool findAndRemove(const Type& v);
	void clear();
	int find(const Type& v) const;

	void printSomeStuff(const char* filePath = "hashdata.txt");
	Type GiveListValue(unsigned int numBuckets, int random, int randomIndex);
};

template<typename Type>
HTable<Type>::HTable(unsigned int numOfBuckets, unsigned int(*hFunction) (const Type &v)){
	buckets = numOfBuckets;
	f = hFunction;
	myPointer = new SLList<Type>[buckets];
}

template<typename Type>
HTable<Type>::~HTable(){
	delete[] myPointer;
}

template<typename Type>
HTable<Type>& HTable<Type>::operator=(const HTable<Type>& that){
	if (this != &that){
		delete[] myPointer;
		buckets = that.buckets;
		myPointer = new SLList<Type>[buckets];
		f = that.f;

		for (unsigned int i = 0; i < buckets; i++)
			myPointer[i] = that.myPointer[i];
	}
	return *this;
}

template<typename Type>
HTable<Type>::HTable(const HTable<Type>& that){
	*this = that;
}

template<typename Type>
void HTable<Type>::insert(const Type& v){
	unsigned int  match = (*f)(v);
	if (match >= 0){
		myPointer[match].addHead(v);
	}
}

template<typename Type>
bool HTable<Type>::findAndRemove(const Type& v){
	int match = find(v);
	if (match >= 0){
		SLLIter<Type> *myIter = new SLLIter<Type>(myPointer[match]);
		myIter->begin();
		while (myIter->current() != v)
			(*myIter)++;

		myPointer[match].remove(*myIter);
		delete myIter;
		return true;
	}
	return false;
}

template<typename Type>
void HTable<Type>::clear(){
	for (unsigned int i = 0; i < buckets; i++){
		myPointer[i].clear();
	}

}

template<typename Type>
int HTable<Type>::find(const Type& v) const{
	unsigned int  match = (*f)(v);
	if (match >= 0){
		SLLIter<Type> *myIter = new SLLIter<Type>(myPointer[match]);
		myIter->begin();
		for (unsigned int i = 0; i < myPointer[match].size(); i++){
			if (!myIter->end()){
				if (myIter->current() == v){
					delete myIter;
					return match;
				}
				else
					(*myIter)++;
			}
		}
		delete myIter;
		return -1;
	}
	else
		return -1;
}


template<typename Type>
Type HTable<Type>::GiveListValue(unsigned int NumBuckets, int random, int randomIndex){

	SLLIter<Type> myIter = SLLIter<Type>(myPointer[random]);
	randomIndex %= myPointer[random].size();

	myIter.begin();

	for (int i = 0; i != randomIndex; i++)
		myIter++;

	if (myPointer[random].size() != 0)
		return  myIter.current();
	else{
		Type temp;
		temp = GiveListValue(NumBuckets, random % 50, randomIndex);
		return temp;
	}


}

template <typename Type>
void HTable<Type>::printSomeStuff(const char* filePath = "hashdata.txt")
{
	// we're gonna need to open a file for all this data
	ofstream outFile(filePath);

	// let's make sure the file got opened
	if (outFile.is_open())
	{
		// number of empty buckets, total count of elements, index of emptiest bucket, index of fullest bucket
		unsigned int empty = 0;
		unsigned int totalCount = 0;
		unsigned int loIndex = 0;
		unsigned int hiIndex = 0;

		// loop through all the buckets
		for (unsigned int i = 0; i < buckets; ++i)
		{
			// add the number of elements in each bucket to the total count
			totalCount += myPointer[i].size();

			// print the index of this bucket and it's size to the file
			//outFile << i << " : " << myPointer[i].size() << '\n';

			// if this list is empty, increment the empty count
			if (myPointer[i].size() == 0)
				++empty;

			// if this list has less elements than the lowest one recorded so far, store this as the new lowest
			if (myPointer[i].size() < myPointer[loIndex].size())
				loIndex = i;
			// else, if this list has more elements than the highest one recorded so far, store this as the new highest
			else if (myPointer[i].size() > myPointer[hiIndex].size())
				hiIndex = i;
		}

		// print the total count of items and number of buckets to the file
		outFile << '\n' << totalCount << " Total items stored in " << buckets << " buckets\n";
		// print the number of empty buckets
		outFile << '\n' << empty << " Buckets are empty\n\n";

		// get the number of elements in the emptiest bucket
		unsigned int Low = myPointer[loIndex].size();
		// get the range fullest-emptiest+1
		unsigned int range = myPointer[hiIndex].size() - Low + 1;
		// print this info to the file
		outFile << "each bucket contains between " << Low << " and " << Low + range - 1 << " items.\n\n";

		// make a new array to count how many buckets contain each number of items between the emptiest and fullest
		// and initialize each cell to 0
		unsigned int* arr = new unsigned int[range];
		for (unsigned int j = 0; j < range; ++j)
			arr[j] = 0;

		// now we go through and count how many buckets contain each number of items...
		// 3 buckets have 15 items
		// 5 buckets have 16 items
		// etc,
		for (unsigned int k = 0; k < buckets; ++k)
			++arr[myPointer[k].size() - Low];

		// now print this data to the file
		for (unsigned int p = 0; p < range; ++p)
			outFile << arr[p] << " buckets have " << p + Low << " items\n";

		// delete the array we made a minute ago, we are done with it
		delete[] arr;
	}
}