/*
 *
 */
#include <cstring>
#include <iostream>
#include "array_list.h"

template <class T>
void ArrayList<T>::append(T *elem)
{
  if (count == size)
  {
    T **elements = new T *[size + INITIAL_SIZE];
  }
  elements[count] = elem;
  count++;
};

template <class T>
T *ArrayList<T>::operator[](int idx)
{
  return elements[idx];
};

int main()
{
  ArrayList<int> a;

  for (int i = 3; i < 20; i++)
  {
    int *v = new int;
    *v = i;
    a.append(v);
  }

  for (int i = 0; i < 17; i++)
  {
    std::cout << *a[i];
    std::cout << '\n';
  }

  return 0;
}
