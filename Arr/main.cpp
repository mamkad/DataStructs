#include "Arr.hpp"

using std::exception;

Arr<float> gtr(Arr<float> a)
{
    cout << a.len() << ' ' << a.cap() << '\n';
    return a;
}

template<typename type>
void print(Arr<type> const& a)
{
    cout << ": -> ";
    for (size_t i  = 0; i < a.len(); ++i)
        cout << a[i] << ' ';
    cout << '\n';
}

int main()
{
    //Arr<double> df;

    try 
    {
        /*
        Arr<float> a;
        Arr<float> b({1.0f,2.0f,3.0f,4.0f});

        cout << b.len() << ' ' << b.cap() << '\n';
        b.push(45.0f);
        cout << b.len() << ' ' << b.cap() << '\n';

        cout << a.len() << ' ' << a.cap() << '\n';
        a = b;
        cout << a.len() << ' ' << a.cap() << '\n';*/

        auto h = gtr({1.0f,2.0f,3.0f,4.0f});
        print(h);
        /*cout << h.len() << ' ' << h.cap() << '\n'; h.resize(256); h.push(3.14);
        cout << h.len() << ' ' << h.cap() << '\n'; h.shrink();
        cout << h.len() << ' ' << h.cap() << '\n';
        h.push(3.14);
         h.push(3.14);
          h.push(3.14);
           h.push(3.14);
            h.push(3.14);
             h.push(3.14);
              h.push(3.14);
               h.push(3.14);
                h.push(3.14);
                 h.push(3.14);
                    h.push(3.14);*/
        cout << h.len() << ' ' << h.cap() << '\n';
        h.shrink();

        cout << h.len() << ' ' << h.cap() << '\n';
        print(h);

        h.resize(246);
        h.push(56);
        h.push(45);
        h.push(3556);
        h.push(5456);
        cout << h.len() << ' ' << h.cap() << '\n';
        print(h);
        h.pushf(0);
        h.pushf(20);
        h.pushf(30);
         cout << h.len() << ' ' << h.cap() << '\n';
        print(h);
        h.pop();
        h.pop();
        h.popf();
         h.popf();
          h.popf();
           h.popf();
            h.popf();
             h.popf();
             h.pop();
         cout << h.len() << ' ' << h.cap() << '\n';
        print(h);
        h.popf();
        h.pop();
         cout << h.len() << ' ' << h.cap() << '\n';
        print(h);
        h.push(23);
        h.shrink();
         cout << h.len() << ' ' << h.cap() << '\n';
        print(h);
         h.pushf(56);
        h.push(45);
        h.push(3556);
          cout << h.len() << ' ' << h.cap() << '\n';
        h.pop();
        h.pop();
        h.pop();
        print(h);
         h.pushf(0);
        h.pushf(20);
        h.pushf(30);
         h.pushf(0);
        h.pushf(20);
        h.pushf(30);
         h.pushf(0);
        h.pushf(20);
        h.pushf(30);
        cout << h.len() << ' ' << h.cap() << '\n';
        h.shrink();
        cout << h.len() << ' ' << h.cap() << '\n';
    }
    catch(exception const& e)
    {
        cout << e.what() << '\n';
        //cout << df.len() << ' '  << df.cap() << '\n';
    }

}