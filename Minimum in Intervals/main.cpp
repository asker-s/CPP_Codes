#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cctype>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <set>
#include <list>
#include <forward_list>
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <functional>
#include <memory>
#include <stdexcept>
using namespace std;

template<typename T_, typename Cmp_ = std::less<T_>>
class CIntervalMin
{
  public:
    // default constructor
    CIntervalMin(){
        this->cmp = Cmp_();

        changeHasHappened = true;
        maxElements = initialMaxElements;
    }

    // constructor with comparator
    CIntervalMin(Cmp_ cmp){
        this->cmp = cmp;

        changeHasHappened = true;
        maxElements = initialMaxElements;
    }

    // constructor - 2 iterators
    template<class Iterator>
    CIntervalMin(Iterator copy_begin, Iterator copy_end){
        this->cmp = Cmp_();
        this->elements.insert(this->elements.begin(), copy_begin, copy_end);

        changeHasHappened = true;
        maxElements = initialMaxElements;
    }

    // constructor - 2 iterators + comparator
    template<class Iterator>
    CIntervalMin(Iterator copy_begin, Iterator copy_end, Cmp_ cmp){
        this->cmp = cmp;
        this->elements.insert(this->elements.begin(), copy_begin, copy_end);

        changeHasHappened = true;
        maxElements = initialMaxElements;
    }

    // const_iterator
    using const_iterator = typename std::vector<T_>::const_iterator;

    // push_back
    void push_back(T_ element){
        elements.push_back(element);

        if(size() > maxElements && !changeHasHappened){
            changeHasHappened = true;
        }
        else if(!changeHasHappened){
            elementAdded();
        }
    }

    // pop_back
    void pop_back(){
        elements.pop_back();
        changeHasHappened = true;
    }

    // min
    T_ min(const_iterator search_begin, const_iterator search_end) {
        int startIndex = search_begin - begin();
        int endIndex = search_end - begin();

        if(startIndex < 0 || endIndex > size() || startIndex >= endIndex){
            throw std::invalid_argument("Invalid Range!!");
        }

        if(changeHasHappened){
            constructSegmentTree();
        }

        return getMinElement(startIndex, endIndex - 1);
    }

    // begin
    const_iterator begin(){
        return elements.cbegin();
    }

    // end
    const_iterator end(){
        return elements.cend();
    }

    // size
    int size(){
        return elements.size();
    }

    void segTree(){
        for(auto e: segmentTree){
            std::cout<<e<<" ";
        }
        std::cout<<std::endl;
    }
  private:
    std::vector<T_> elements;
    std::function<bool(T_, T_)> cmp;

    std::vector<int> segmentTree;
    int maxElements;
    bool changeHasHappened;

    static const int initialMaxElements = 1<<10; // 2^10
    static const int maxElementsScalingFactor = 2; // should only be powers of 2!!

    void constructSegmentTree(){
        while(maxElements < size()) maxElements *= maxElementsScalingFactor;

        changeHasHappened = false;
        segmentTree.clear();

        int segmentTreeSize = 2 * maxElements + 10;
        segmentTree.resize(segmentTreeSize);

        recurrConstructSegmentTree(1, 0, maxElements - 1);
    }

    void recurrConstructSegmentTree(int parent, int leftIndex, int rightIndex){
        if(leftIndex == rightIndex){
            segmentTree[parent] = leftIndex;
            return;
        }

        int leftChild = 2 * parent;
        int rightChild = 2 * parent + 1;

        int midIndex = (leftIndex + rightIndex) / 2;

        recurrConstructSegmentTree(leftChild, leftIndex, midIndex);
        recurrConstructSegmentTree(rightChild, midIndex + 1, rightIndex);

        segmentTree[parent] = indexCmp(segmentTree[leftChild], segmentTree[rightChild]);
    }

    int indexCmp(int ind1, int ind2){
        if(ind2 >= size() || ind1 >= size())
            return std::min(ind1, ind2);

        bool e1Smaller = cmp(elements[ind1], elements[ind2]);
        bool e2Smaller = cmp(elements[ind2], elements[ind1]);

        if(!e1Smaller && !e2Smaller)
            return std::min(ind1, ind2);
        else if(e1Smaller)
            return ind1;
        else
            return ind2;
    }

    T_ getMinElement(int startIndex, int endIndex){
        int minElemIndex = recurrGetMin(1, 0, maxElements - 1, startIndex, endIndex);
        return elements[minElemIndex];
    }

    int recurrGetMin(int parent, int leftIndex, int rightIndex, int searchLeftIndex, int searchRightIndex){
        if(searchLeftIndex <= leftIndex && rightIndex <= searchRightIndex){
            return segmentTree[parent];
        }

        if(searchRightIndex < leftIndex || searchLeftIndex > rightIndex){
            return segmentTree.size();
        }

        int leftChild = 2 * parent;
        int rightChild = 2 * parent + 1;

        int midIndex = (leftIndex + rightIndex) / 2;

        int leftMin = recurrGetMin(leftChild, leftIndex, midIndex, searchLeftIndex, searchRightIndex);
        int rightMin = recurrGetMin(rightChild, midIndex + 1, rightIndex, searchLeftIndex, searchRightIndex);

        return indexCmp(leftMin, rightMin);
    }

    void elementAdded(){
        int addedIndex = size() - 1;
        updateSegmentTree(1, 0, maxElements - 1, addedIndex);
    }

    void updateSegmentTree(int parent, int leftIndex, int rightIndex, int addedIndex){
        if(leftIndex == rightIndex)
            return;

        if(addedIndex < leftIndex || rightIndex < addedIndex)
            return;

        segmentTree[parent] = indexCmp(segmentTree[parent], addedIndex);

        int leftChild = 2 * parent;
        int rightChild = 2 * parent + 1;

        int midIndex = (leftIndex + rightIndex) / 2;
        updateSegmentTree(leftChild, leftIndex, midIndex, addedIndex);
        updateSegmentTree(rightChild, midIndex + 1, rightIndex, addedIndex);
    }

};

class CStrComparator
{
  public:
               CStrComparator ( bool byLength = true )
      : m_ByLength ( byLength )
    {
    }
    bool       operator () ( const string & a, const string & b ) const
    {
      return m_ByLength ? a . length () < b . length () : a < b;
    }
  private:
    bool       m_ByLength;
};
//-------------------------------------------------------------------------------------------------
bool strCaseCmpFn ( const string & a, const string & b )
{
  return strcasecmp ( a . c_str (), b . c_str () ) < 0;
}
//-------------------------------------------------------------------------------------------------
int main ( void )
{
  CIntervalMin <int> a1;
  for ( auto x : initializer_list<int> { 5, 15, 79, 62, -3, 0, 92, 16, 2, -4 } ){
    a1 . push_back ( x );
  }

  assert ( a1 . size () == 10 );

  ostringstream oss;
  for ( auto x : a1 )
    oss << x << ' ';

  assert ( oss . str () == "5 15 79 62 -3 0 92 16 2 -4 " );
  assert ( a1 . min ( a1 . begin (), a1 . end () ) == -4 );
  assert ( a1 . min ( a1 . begin () + 2, a1 . begin () + 3 ) == 79 );
  assert ( a1 . min ( a1 . begin () + 2, a1 . begin () + 9 ) == -3 );

  try
  {
    a1 . min ( a1 . begin (), a1 . begin () );
    assert ( "Missing an exception" == nullptr );
  }
  catch ( const invalid_argument & e )
  {
  }
  catch ( ... )
  {
    assert ( "Invalid exception" == nullptr );
  }

  a1 . pop_back ();
  assert ( a1 . size () == 9 );
  a1 . push_back ( 42 );

  assert ( a1 . min ( a1 . begin (), a1 . end () ) == -3 );

  vector<string> words{ "auto", "if", "void", "NULL" };
  CIntervalMin <string> a2 ( words . begin (), words . end () );
  assert ( a2 . min ( a2 . begin (), a2 . end () ) ==  "NULL" );

  CIntervalMin <string, bool(*)(const string &, const string &)> a3 ( words . begin (), words . end (), strCaseCmpFn );
  assert ( a3 . min ( a3 . begin (), a3 . end () ) == "auto" );

  CIntervalMin <string, CStrComparator> a4 ( words . begin (), words . end () );
  assert ( a4 . min ( a4 . begin (), a4 . end () ) == "if" );

  CIntervalMin <string, CStrComparator> a5 ( words . begin (), words . end (), CStrComparator ( false ) );
  assert ( a5 . min ( a5 . begin (), a5 . end () ) == "NULL" );

  CIntervalMin <string, function<bool(const string &, const string &)> > a6 ( [] ( const string & a, const string & b )
  {
    return a > b;
  } );
  for ( const auto & w : words )
    a6 . push_back ( w );
  assert ( a6 . min ( a6 . begin (), a6 . end () ) == "void" );
  return 0;
}
