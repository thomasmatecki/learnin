static const int INITIAL_SIZE = 8;

template <class T>
class ArrayList
{
private:
    int size;
    int count;
    T **elements;

public:
    ArrayList(){};
    ~ArrayList(){};

    void append(T *elem);
    void insert(T *elem);
    T *operator[](int idx);
};