#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <cstdarg>
#include "Point.h"
#include "BaseObject.h"
#include "Triangle.h"




class Rect : public BaseObject {
public:
    Rect() : p1(), p2(), p3(), p4() {}
    int k = 0;
    Point points[4];
    template <typename T>
    void set(const T& element) {
        points[k] = element;
        k++;
        if (k == 2) {
            p1 = points[0];
            p3 = points[1];
            p2 = Point(p1.x, p3.y);
            p4 = Point(p3.x, p1.y);

        }
        if (k == 4) {
            p1 = points[0];
            p2 = points[1];
            p3 = points[2];
            p4 = points[3];
        }
        center = std::make_unique<Point>((p1.x + p3.x) / 2, (p1.y + p3.y) / 2);
    }
    template <typename T, typename ... Args>
    void set(const T& element, const Args& ... arguments) {
        points[k] = element;
        k++;
        set(arguments ...);
    }


    Point* getCenter() override {
        return center.get();
    }

    void boundingBox(Point& topLeft, Point& bottomRight) override {

        topLeft.x = std::min({ p1.x, p2.x, p3.x, p4.x });
        topLeft.y = std::max({ p1.y, p2.y, p3.y, p4.y });
        bottomRight.x = std::max({ p1.x, p2.x, p3.x, p4.x });
        bottomRight.y = std::min({ p1.y, p2.y, p3.y, p4.y });
    };

    void saveToFile(const std::string& filename) override {
        std::ofstream file(filename, std::ios::app);
        if (file.is_open()) {
            file << "Rect" << std::endl;
            file << p1.x << " " << p1.y << std::endl;
            file << p2.x << " " << p2.y << std::endl;
            file << p3.x << " " << p3.y << std::endl;
            file << p4.x << " " << p4.y << std::endl;
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
            if (shapeType == "Rect") {
                file >> p1.x >> p1.y;
                file >> p2.x >> p2.y;
                file >> p3.x >> p3.y;
                file >> p4.x >> p4.y;
                center = std::make_unique<Point>((p1.x + p3.x) / 2, (p1.y + p3.y) / 2);
            }
            file.close();
        }
    }

private:
    Point p1, p2, p3, p4;
};

class Circle : public BaseObject {
public:
    Circle() : radius(0) {}

    void set(const Point& center_, double radius_) {
        center = std::make_unique<Point>(center_);
        radius = radius_;
    }

    Point* getCenter() override {
        return center.get();
    }

    void boundingBox(Point& topLeft, Point& bottomRight) override {
        topLeft.x = center->x - radius;
        topLeft.y = center->y + radius;
        bottomRight.x = center->x + radius;
        bottomRight.y = center->y - radius;
    }

    void saveToFile(const std::string& filename) override {
        std::ofstream file(filename, std::ios::app);
        if (file.is_open()) {
            file << "Circle" << std::endl;
            file << center->x << " " << center->y << std::endl;
            file << radius << std::endl;
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
            if (shapeType == "Circle") {
                double x, y;
                file >> x >> y;
                center = std::make_unique<Point>(x, y);
                file >> radius;
            }
            file.close();
        }
    }

private:
    double radius;
};



class Polyline : public BaseObject {
public:
    Polyline() : p1(), p2(), p3(), p4(), p5(), p6(), p7(), p8(), p9(), p10() {}
    int k = 0;
    Point points[10];
    template <typename T>
    void set(const T& element) {
        points[k] = element;
        k++;

    }
    template <typename T, typename ... Args>
    void set(const T& element, const Args& ... arguments) {
        points[k] = element;
        k++;
        set(arguments ...);
    }


    Point* getCenter() override {
        return center.get();
    }

    void boundingBox(Point& topLeft, Point& bottomRight) override {
        double minx = points[0].x;
        double miny = points[0].y;
        double maxx = points[0].x;
        double maxy = points[0].y;

        for (int i = 1; i < k; i++) {
            if (minx > points[i].x) {
                minx = points[i].x;
            }
            if (miny > points[i].y) {
                miny = points[i].y;
            }
            if (maxx < points[i].x) {
                maxx = points[i].x;
            }
            if (maxy < points[i].y) {
                maxy = points[i].y;
            }
        }

        topLeft.x = minx;
        topLeft.y = miny;
        bottomRight.x = maxx;
        bottomRight.y = maxy;
        center = std::make_unique<Point>((minx + maxx) / 2, (miny + maxy) / 2);
    };

    void orientedBoundingBox() {
        Point center;

        for (int i = 0; i < k; i++) {
            center.x += points[i].x;
            center.y += points[i].y;
        }
        center.x /= k;
        center.y /= k;

        //���������� �������
        double covXX, covXY , covYY;
        covXX =  covXY = covYY = 0.0;
        for (int i = 0; i < k; i++) {
            double dx = points[i].x - center.x;
            double dy = points[i].y - center.y;
            covXX += dx * dx;
            covXY += dx * dy;
            covYY += dy * dy;
        }
        covXX /= k;
        covXY /= k;
        covYY /= k;

        //���������� ������� �������
        double sumCov = covXX + covYY;
        double determinant = covXX * covYY - covXY * covXY;
        double l1 = sumCov / 2 + sqrt(sumCov * sumCov / 4 - determinant);
        double l2 = sumCov / 2 - sqrt(sumCov * sumCov / 4 - determinant);

        // ���������� �������� ������� �������
        double eigVecX = covXY != 0 ? l1 - covYY : 1;
        double eigVecY = eigVecX != 0 ? covXY / eigVecX : 1;
        double angle = atan2(eigVecY, eigVecX);

        //������������ ����� �� ��� ��
        double minX = 1e9, minY = 1e9, maxX = -1e9, maxY = -1e9;
        
        for (int i = 0; i < k; i++) {
            double rotateX = (points[i].x - center.x) * cos(angle) + (points[i].y - center.y) * sin(angle);
            double rotateY = -(points[i].x - center.x) * sin(angle) + (points[i].y - center.y) * cos(angle);

            if (rotateX < minX) minX = rotateX;
            if (rotateX > maxX) maxX = rotateX;
            if (rotateY < minY) minY = rotateY;
            if (rotateY > maxY) maxY = rotateY;
        }

        // ���������� ������, �������� ������ OBB
        double width = maxX - minX;
        double height = maxY - minY;
        Point obbCenter = { center.x, center.y };

    }

    void saveToFile(const std::string& filename) override {
        std::ofstream file(filename, std::ios::app);
        if (file.is_open()) {
            file << "Polyline" << std::endl;
            for (int i = 0; i < k; i++) {
                file << points[i].x << " " << points[i].y << std::endl;
            }

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
            if (shapeType == "Polyline") {
                for (int i = 0; i < 10; i++) {

                    if (file.eof()) break;
                    file >> points[i].x >> points[i].y;
                }
            }
            file.close();
        }
    }

private:
    Point p1, p2, p3, p4, p5, p6, p7, p8, p9, p10;
};

BaseObject* factory(int n) {
    switch (n) {
    case 1:
        return new Rect();
    case 2:
        return new Circle();
    case 3:
        return new Triangle();
    default:
        return nullptr;
    }
}

int main() {
    {
        BaseObject* pRect_1 = factory(1);
        Rect* pRect_2 = new Rect();


        BaseObject* pCircle_1 = factory(2);
        Circle* pCircle_2 = new Circle();

        BaseObject* pTriangle_1 = factory(3);
        Triangle* pTriangle_2 = new Triangle();

        { // set rect
            Point p1(1.0, 1.0);
            Point p2(1.0, 5.0);
            Point p3(5.0, 5.0);
            Point p4(5.0, 1.0);
            ((Rect*)pRect_1)->set(p1, p2, p3, p4);
            pRect_2->set(p1, p3);
        };





        { // set triangle
            Point p1(1.0, 1.0);
            Point p2(3.0, 3.0);
            Point p3(4.0, 2.0);
            ((Triangle*)pTriangle_1)->set(p1, p2, p3);
            pTriangle_2->set(p1, p2, p3);
          
        }
        


        { // set Circle
            Point p1(1.0, 1.0);
            ((Circle*)pCircle_1)->set(p1, 5.0);
            pCircle_2->set(p1, 5.0);
        }

        pCircle_1->saveToFile("Ready.txt");
        pTriangle_2->saveToFile("Ready.txt");


        if (pRect_1->getCenter()->x != 3.0 || pRect_1->getCenter()->y != 3.0)
            printf("FAIL 1\n");

        if (pRect_2->getCenter()->x != 3.0 || pRect_2->getCenter()->y != 3.0)
            printf("FAIL 2\n");

        if (pCircle_1->getCenter()->x != 1.0 || pCircle_1->getCenter()->y != 1.0)
            printf("FAIL 3\n");

        if (pCircle_2->getCenter()->x != 1.0 || pCircle_2->getCenter()->y != 1.0)
            printf("FAIL 4\n");

        if (std::abs(pTriangle_1->getCenter()->x - 2.6) > 0.1 || pTriangle_1->getCenter()->y != 2.0)
            printf("FAIL 5\n");

        if (std::abs(pTriangle_2->getCenter()->x - 2.6) > 0.1 || pTriangle_2->getCenter()->y != 2.0)
            printf("FAIL 6\n");

        delete pRect_1;
        delete pRect_2;
        delete pCircle_1;
        delete pCircle_2;
        delete pTriangle_1;
        delete pTriangle_2;
    }
    return 0;
}
