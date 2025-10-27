#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Shape.h"

class Triangle : public Shape
{
public:
    Triangle();
    ~Triangle();
    void draw(QPainter* painter, QRect rect) override;
    bool isInsideShape(QPoint point) const override;
    QPoint getCentrePoint() const override;
    void moveShape(QPoint offset) override;
    void setShapeFigure(QRect rect) override;

private:
    int* getKoefs(QPoint a, QPoint b) const;
    bool isHigherThen(QPoint point, int* koefs) const;
    double getSideLength(QLine side) const;
    QPolygon poly;
};

#endif // TRIANGLE_H
