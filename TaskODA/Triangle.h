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

    void orientedBoundingBox(Point& p1, Point& p2, Point& p3) {
        Point center;
        center.x = (p1.x + p2.x + p3.x)/3;
        center.y = (p1.y + p2.y + p3.y) / 3;

        //Обчислення матриці
        double covXX = ((center.x-p1.x)* (center.x - p1.x) + (center.x - p2.x) * (center.x - p2.x) + (center.x - p3.x)* (center.x - p3.x))/3;
        double covXY = ((center.x - p1.x) * (center.y - p1.y) + (center.x - p2.x) * (center.y - p2.y) + (center.x - p3.x) * (center.y - p3.y)) / 3;
        double covYY = ((center.y - p1.y) * (center.y - p1.y) + (center.y - p2.y) * (center.x - p2.y) + (center.x - p3.y) * (center.y - p3.y)) / 3;
        
        //Обчислення власних значень
        double sumCov = covXX + covYY;
        double determinant = covXX * covYY - covXY * covXY;
        double l1 = sumCov / 2 + sqrt(sumCov * sumCov / 4 - determinant);  
        double l2 = sumCov / 2 - sqrt(sumCov * sumCov / 4 - determinant);

        // Обчислення напрямків власних векторів
        double eigVecX = covXY != 0 ? l1 - covYY : 1;
        double eigVecY = eigVecX != 0 ? covXY / eigVecX : 1;
        double angle = atan2(eigVecY, eigVecX);

        //Проектування точок на нові осі
        double minX = 1e9, minY = 1e9, maxX = -1e9, maxY = -1e9;
        Point points[3] = { p1, p2, p3 };
        for (int i = 0; i < 3; i++ ) {
            double rotateX = (points[i].x - center.x) * cos(angle) + (points[i].y - center.y) * sin(angle);
            double rotateY = -(points[i].x - center.x) * sin(angle) + (points[i].y - center.y) * cos(angle);

            if (rotateX < minX) minX = rotateX;
            if (rotateX > maxX) maxX = rotateX;
            if (rotateY < minY) minY = rotateY;
            if (rotateY > maxY) maxY = rotateY;
        }

        // Обчислення ширини, висотита центру OBB
        double width = maxX - minX;
        double height = maxY - minY;
        Point obbCenter = { center.x, center.y };
       
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
