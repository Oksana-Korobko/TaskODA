#ifndef POINT_H
#define POINT_H

struct Point {
    
    Point() : x(0), y(0) {}

    Point(double x_, double y_) : x(x_), y(y_) {}

    double x;
    double y;
};

#endif // POINT_H


