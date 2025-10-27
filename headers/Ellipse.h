#ifndef ELLIPSE_H
#define ELLIPSE_H

#include "Shape.h"

class Ellipse : public Shape
{
public:
    Ellipse();
    ~Ellipse();
    void draw(QPainter* painter, QRect rect) override;
    bool isInsideShape(QPoint point) const override;
    QPoint getCentrePoint() const override;
    void moveShape(QPoint offset) override;
    void setShapeFigure(QRect rect) override;

private:
    QRect rect;
};

#endif // ELLIPSE_H
