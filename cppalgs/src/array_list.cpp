/*
 *
 */
#include <cstring>
#include <iostream>
#include "array_list.hpp"

template <class T>
void ArrayList<T>::append(T *elem)
{
  if (count == size)
  {
    T **elements = new T *[size + INITIAL_SIZE];
  }
  elements[count] = elem;
  count++;
}

template <class T>
T * ArrayList<T>::get(int index)
{
  return this->elements[index];
}


template class ArrayList<char>;
