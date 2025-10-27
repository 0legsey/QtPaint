#ifndef SHAPE_H
#define SHAPE_H

#include <QPainter>
#include <QRect>
#include <QPolygon>
#include <QList>
#include <QPoint>
#include <QLine>
#include <cmath>
#include <QDebug>

class Shape{
public:
    virtual void draw(QPainter* painter, QRect rect) = 0;
    virtual bool isInsideShape(QPoint point) const = 0;
    virtual QPoint getCentrePoint() const = 0;
    virtual void moveShape(QPoint offset) = 0;
    virtual void setShapeFigure(QRect rect) = 0;
    virtual ~Shape() = default;

    //id
    virtual void setId(int id){ this->id = id; }
    virtual int getId() const { return this->id; }

    //type
    virtual void setType(QString type){ this->type = type; }
    virtual QString getType() const { return this->type; }

    //topLeft
    virtual void setTopLeft(QPoint topLeft){ this->topLeft = topLeft; }
    virtual QPoint getTopLeft() const { return this->topLeft; }

    //bottomRight
    virtual void setBottomRight(QPoint bottomRight){ this->bottomRight = bottomRight; }
    virtual QPoint getbottomRight() const { return this->bottomRight; }

protected:
    int id;
    QString type;
    QPoint topLeft;
    QPoint bottomRight;
};

#endif // SHAPE_H
