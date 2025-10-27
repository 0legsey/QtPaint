#include "headers/Ellipse.h"

Ellipse::Ellipse() {
    type = "ellipse";
    qDebug() << "Эллипс создан";
}

Ellipse::~Ellipse(){
    qDebug() << "Эллипс был удалён";
}

void Ellipse::draw(QPainter *painter, QRect rect)
{
    this->rect = rect;
    QRect rectF(rect);
    painter->drawEllipse(rectF.center(), rectF.width() / 2, rectF.height() / 2);
}

/*в qt отрисовка эллипса происходит немного специфично, отрисовка левой части
  происхдит на единицу левее реального местоположения левого края прямоугольника,
  в который эллипс вписан, аналогичная ситуация с верхним краем, только он отрисован выше
  поэтому при поиске центра смещаем его на 1 влево и на 1 вверх*/
bool Ellipse::isInsideShape(QPoint point) const
{
    QRectF rectF(this->rect);
    QPointF centre = QPoint((rectF.left() + rectF.right() - 1) / 2, (rectF.top() + rectF.bottom() - 1) / 2);

    double a = rectF.width() / 2;
    double b = rectF.height() / 2;

    double E = powf((point.x() - centre.x()), 2) / powf(a, 2) + powf((point.y() - centre.y()), 2) / powf(b, 2);

    if (E <= 1){
        return true;
    }
    return false;
}

QPoint Ellipse::getCentrePoint() const
{
    return this->rect.center();
}

void Ellipse::moveShape(QPoint offset)
{
    this->rect.translate(offset);
    this->topLeft = this->rect.topLeft();
    this->bottomRight = this->rect.bottomRight();
}

void Ellipse::setShapeFigure(QRect rect)
{
    this->rect = rect;
    this->topLeft = rect.topLeft();
    this->bottomRight = rect.bottomRight();
}

