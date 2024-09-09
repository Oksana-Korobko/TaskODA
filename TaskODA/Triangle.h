#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "BaseObject.h"
#include <fstream> 
#include <iostream>


class Triangle : public BaseObject {
public:
    Triangle() : p1(), p2(), p3() {}

    void set(const Point& p1_, const Point& p2_, const Point& p3_) {
        p1 = p1_;
        p2 = p2_;
        p3 = p3_;
        center = std::make_unique<Point>((p1.x + p2.x + p3.x) / 3, (p1.y + p2.y + p3.y) / 3);
    }

    Point* getCenter() override {
        return center.get();
    }

    void boundingBox(Point& topLeft, Point& bottomRight) override {
        topLeft.x = std::min({ p1.x, p2.x, p3.x });
        topLeft.y = std::max({ p1.y, p2.y, p3.y });
        bottomRight.x = std::max({ p1.x, p2.x, p3.x });
        bottomRight.y = std::min({ p1.y, p2.y, p3.y });
    }

    void saveToFile(const std::string& filename) override {
        std::ofstream file(filename);
        if (file.is_open()) {
            file << "Triangle" << std::endl;
            file << p1.x << " " << p1.y << std::endl;
            file << p2.x << " " << p2.y << std::endl;
            file << p3.x << " " << p3.y << std::endl;
            file.close();
        }
    }

    void loadFromFile(const std::string& filename) override {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Unable to open file: " << filename << std::endl;
            return;
        }
        else
        {
            std::string shapeType;
            file >> shapeType;
            if (shapeType == "Triangle") {
                file >> p1.x >> p1.y;
                file >> p2.x >> p2.y;
                file >> p3.x >> p3.y;
                center = std::make_unique<Point>((p1.x + p2.x + p3.x) / 3, (p1.y + p2.y + p3.y) / 3);
            }
            file.close();
        }
    }

   

private:
    Point p1, p2, p3;
};

#endif // TRIANGLE_H
