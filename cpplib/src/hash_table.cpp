#include <iostream>
using namespace std;

static const int INITIAL_SIZE = 8;

template <class T>
class ArrayList
{
private:
    int count = 0;
    int size = INITIAL_SIZE;
    T *elements[INITIAL_SIZE];

public:
    ArrayList() { T *elements = new T[INITIAL_SIZE]; };
    ~ArrayList(){};
    void append(T elem)
    {
        if (this->count < this->size)
        {
            this->elements[count] = &elem;
            count++;
        }
        else
        {
            // reallocate
            int size = 2 * this->size;
            T *elements = new T[size];
        }
    }
};

template <class K, class V>
class HashTable
{
public:
    void set(K k, V v){

    };

private:
protected:
};

int main()
{
    ArrayList<string *> s;
    string *str = new string("foo");
    s.append(str);
    s.append(new string("foo"));

    return 0;
}