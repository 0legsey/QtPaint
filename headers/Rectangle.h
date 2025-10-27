#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "Shape.h"

class Rectangle : public Shape
{
public:
    Rectangle();
    ~Rectangle();
    void draw(QPainter* painter, QRect rect) override;
    bool isInsideShape(QPoint point) const override;
    QPoint getCentrePoint() const override;
    void moveShape(QPoint offset) override;
    void setShapeFigure(QRect rect) override;

private:
    QRect rect;
};

#endif // RECTANGLE_H
