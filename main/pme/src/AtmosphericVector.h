#ifndef FEMTA_ATMOSPHERIC_VECTOR
#define FEMTA_ATMOSPHERIC_VECTOR

#include <vector>
#include <limits>

/**
 * @brief A wrapper of vector which allows a user-defined maximum size 
 * 
 * @cite https://stackoverflow.com/questions/9248728/vector-how-to-set-the-maximum-number-of-elements-and-find-the-current-count
 * 
 * @tparam T 
 */
template<typename T>
class AtmosphericVector : public std::vector<T>
{
    /**
     * @brief Capacity of vector 
     * 
     */
    int _maxSize;
public:
    /**
     * @brief Set the capacity of the vector
     * 
     * @param maxSize 
     */
    void setMaxSize(int maxSize) {_maxSize = maxSize;}

    /**
     * @brief Append an element to the vector as long as the vector size is less than
     * the capacity
     * 
     * @param element 
     */
    void push_back(const T& element)
    {
        if (std::vector<T>::size() < _maxSize)
           std::vector<T>::push_back(element);
        else
        {
           
        }
    }

    /**
     * @brief Check if the vector is filled to its capacity
     * 
     * @return true 
     * @return false 
     */
    bool isFilled() {return std::vector<T>::size() == _maxSize;}
};

#endif