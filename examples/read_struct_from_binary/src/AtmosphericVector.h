#ifndef FEMTA_ATMOSPHERIC_VECTOR
#define FEMTA_ATMOSPHERIC_VECTOR

#include <vector>
#include <limits>

/*
    A wrapper which allows a user-defined maximum size 
    Link: https://stackoverflow.com/questions/9248728/vector-how-to-set-the-maximum-number-of-elements-and-find-the-current-count
*/
template<typename T>
class AtmosphericVector : public std::vector<T>
{
    int _maxSize;
public:
    void setMaxSize(int maxSize) {_maxSize = maxSize;}
    void push_back(const T& element)
    {
        if (std::vector<T>::size() < _maxSize)
           std::vector<T>::push_back(element);
        else
        {
           
        }
    }
    bool isFilled() {return std::vector<T>::size() == _maxSize;}
};

#endif