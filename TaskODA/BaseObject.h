#ifndef BASEOBJECT_H
#define BASEOBJECT_H

#include "Point.h"
#include <memory>
#include <string>

struct Color {
    int r;
    int g;
    int b;

    Color(int red, int green, int blue) : r(red), g(green), b(blue) {}
};

class BaseObject {
public:
    BaseObject() : center(nullptr), color(0, 0, 0) {};
    virtual Point* getCenter() = 0;
    void setColor(int r, int g, int b) { color = Color(r, g, b); };
    virtual void boundingBox(Point& topLeft, Point& bottomRight) = 0;
    virtual void saveToFile(const std::string& filename) = 0;
    virtual void loadFromFile(const std::string& filename) = 0;

protected:
    std::unique_ptr<Point> center;
    Color color;
};

#endif // BASEOBJECT_H
