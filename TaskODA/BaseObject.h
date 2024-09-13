#ifndef BASEOBJECT_H
#define BASEOBJECT_H

#include "Point.h"
#include <memory>
#include <string>
#include <cstdint>



class BaseObject {
public:
    BaseObject() : center(nullptr), color(0) {};
    virtual Point* getCenter() = 0;
    virtual void setColor(uint8_t r, uint8_t g, uint8_t b) = 0 ;
    virtual void boundingBox(Point& topLeft, Point& bottomRight) = 0;
    virtual void saveToFile(const std::string& filename) = 0;
    virtual void loadFromFile(const std::string& filename) = 0;

protected:
    std::unique_ptr<Point> center;
    uint32_t color;
};

#endif // BASEOBJECT_H
