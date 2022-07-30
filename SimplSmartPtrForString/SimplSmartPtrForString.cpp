#include <iostream>

using namespace std;

class StringPointer {
public:
    StringPointer(string* pointer) : p(pointer) {};
    ~StringPointer() { if (p == nullptr) { delete p; } };

    string* operator->() 
    {
        initIfNull();
        return p;
    }

    operator string* () 
    { 
        initIfNull(); 
        return p; 
    };

private:
    void initIfNull() { if (p == nullptr) { p = new string(""); } };
    string* p;
};

int main()
{
    string s1 = "Hello, world!";

    StringPointer sp1(&s1);
    StringPointer sp2(NULL);

    cout << sp1->length() << endl;
    cout << *sp1 << endl;
    cout << sp2->length() << endl;
    cout << *sp2 << endl;

    return 1;
}