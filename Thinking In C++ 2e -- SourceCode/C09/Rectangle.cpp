//: C09:Rectangle.cpp
// From Thinking in C++, 2nd Edition
// Available at http://www.BruceEckel.com
// (c) Bruce Eckel 1999
// Copyright notice in Copyright.txt
// Accessors & mutators

class Rectangle {
  int _width, _height;
public:
  Rectangle(int w = 0, int h = 0)
    : _width(w), _height(h) {}
  int width() const { return _width; } // Read
  void width(int w) { _width = w; } // Set
  int height() const { return _height; } // Read
  void height(int h) { _height = h; } // Set
};

int main() {
  Rectangle r(19, 47);
  // Change width & height:
  r.height(2 * r.width());
  r.width(2 * r.height());
} ///:~
