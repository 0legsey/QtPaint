#include "headers/Rectangle.h"

Rectangle::Rectangle() {
    type = "rectangle";
    qDebug() << "Прямоугольник создан";
}

Rectangle::~Rectangle(){
    qDebug() << "Прямоугольник был удалён";
}

void Rectangle::draw(QPainter* painter, QRect rect)
{
    this->rect = rect;
    painter->drawRect(this->rect);
}

//для прямоугольника просто проверяем, что точка нажатия лежит внутри него
bool Rectangle::isInsideShape(QPoint point) const
{
    if((point.x() >= this->rect.left() && point.x() <= this->rect.right() + 1) &&
        point.y() >= this->rect.top() && point.y() <= this->rect.bottom() + 1){
        return true;
    }
    return false;
}

QPoint Rectangle::getCentrePoint() const
{
    return this->rect.center();
}

void Rectangle::moveShape(QPoint offset)
{
    this->rect.translate(offset);
    this->topLeft = this->rect.topLeft();
    this->bottomRight = this->rect.bottomRight();
}

void Rectangle::setShapeFigure(QRect rect)
{
    this->rect = rect;
    this->topLeft = rect.topLeft();
    this->bottomRight = rect.bottomRight();
}

