
#include <iostream>
#include <string>
using namespace std;

class Screen {
  public:
    Screen(int hi = 8, int wid = 40, char bkground = '#');
    void home() const { _cursor = 0; }
    inline void forward() const;
    inline void backward() const;
    inline void up() const;
    inline void down() const;
    inline void move(int,int) const;
    char get() const { return _screen [_cursor ]; }
    int height() const { return _height; }
    int width() const { return _width; }
    inline char get(int, int) const;
    bool checkRange(int, int) const;
    void set(const string &s);
    void set(char ch);
    void copy(const Screen &sobj);
    bool isEqual(char ch) const;
    void display() const;

  private:
    inline int remainingSpace() const;
    int row() const { return (_cursor + _width) / _width;}
    mutable string::size_type _cursor;
    string _screen;
    short _height;
    short _width;
};

void Screen::move(int r,int c)const
{
    if (checkRange(r,c)) {
        int row = (r-1) * _width;
        _cursor = row + c - 1;
    }
}

char Screen::get(int r, int c) const
{
    move(r,c);
    return get();
}

int Screen::remainingSpace()const
{
    int sz = _width * _height;
    return (sz-_cursor);
}

void Screen::forward()const
{
    ++_cursor;
    if (_cursor == _screen.size())
        home();
}

void Screen::backward()const
{
    if(_cursor == 0)
        _cursor = _width * _height - 1;
    else
        --_cursor;
}

const char BELL = '\007';

void Screen::up() const
{
    if (row() == 1)
        cout << BELL << endl;
    else
        _cursor -=_width;
}

void Screen::down()const
{
    if (row() == _height)
        cout << BELL << endl;
    else
        _cursor += _width;
}

int main()
{
}
