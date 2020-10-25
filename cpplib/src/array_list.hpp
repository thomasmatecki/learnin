static const int INITIAL_SIZE = 8;

template <class T>
class ArrayList
{
public:
    ArrayList(){};
    ~ArrayList(){};
    void append(T *elem);
    T * get(int index);
private:
    int size;
    int count;
    T *elements[];
};