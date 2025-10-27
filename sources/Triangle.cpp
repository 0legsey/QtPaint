#include "headers/Triangle.h"

Triangle::Triangle() {
    type = "triangle";
    qDebug() << "Треугольник создан";
}

Triangle::~Triangle(){
    qDebug() << "Треугольник был удалён";
}

void Triangle::draw(QPainter *painter, QRect rect)
{
    poly = QPolygon();
    QPoint p1(rect.left(), rect.bottom());
    QPoint p2(rect.left() + (rect.right() - rect.left()) / 2 , rect.top());
    QPoint p3(rect.bottomRight());
    poly << p1 << p2 << p3;
    painter->drawPolygon(poly);
}

//для треугольника нужно проверить, что точка лежит ниже его рёбер и выше основания
//однако, так как в qt координата Y увеличивается сверху вниз, то проверяем, что точка выше рёбер
//и ниже основания
bool Triangle::isInsideShape(QPoint point) const
{
    int* leftLineKoefs = new int[3];
    leftLineKoefs = getKoefs(poly.point(0), poly.point(1));
    int* rightLineKoefs = new int[3];
    rightLineKoefs = getKoefs(poly.point(1), poly.point(2));

    if (isHigherThen(point, leftLineKoefs) && isHigherThen(point, rightLineKoefs)
        && point.y() <= poly.point(0).y()){
        delete[] leftLineKoefs;
        delete[] rightLineKoefs;
        return true;
    }

    delete[] leftLineKoefs;
    delete[] rightLineKoefs;
    return false;
}

//определяем центр треугольника по координатам его точкам
QPoint Triangle::getCentrePoint() const
{
    double a = getSideLength(QLine(poly.point(1), poly.point(2)));
    double b = getSideLength(QLine(poly.point(0), poly.point(2)));
    double c = getSideLength(QLine(poly.point(0), poly.point(1)));
    double x = (a * poly.point(0).x() + b * poly.point(1).x() + c * poly.point(2).x()) / (a + b + c);
    double y = (a * poly.point(0).y() + b * poly.point(1).y() + c * poly.point(2).y()) / (a + b + c);
    return QPoint(x, y);
}

void Triangle::moveShape(QPoint offset)
{
    this->poly.translate(offset);
    this->topLeft = QPoint(this->poly.point(0).x(), this->poly.point(1).y());
    this->bottomRight = this->poly.point(2);
}

void Triangle::setShapeFigure(QRect rect)
{
    poly = QPolygon();
    QPoint p1(rect.left(), rect.bottom());
    QPoint p2(rect.left() + (rect.right() - rect.left()) / 2 , rect.top());
    QPoint p3(rect.bottomRight());
    poly << p1 << p2 << p3;
    this->topLeft = rect.topLeft();
    this->bottomRight = rect.bottomRight();
}

//получаем коэффициенты при x, y и свободном члене
int* Triangle::getKoefs(QPoint a, QPoint b) const
{
    int* koefs = new int[3];
    koefs[0] = b.y() - a.y();
    koefs[1] = a.x() - b.x();
    koefs[2] = a.x() * (b.y() - a.y()) - a.y() * (b.x() - a.x());
    return koefs;
}

//проверка на местоположение точки относительно линии
bool Triangle::isHigherThen(QPoint point, int *koefs) const
{
    return (koefs[0] * point.x() + koefs[1] * point.y() <= koefs[2]);
}

double Triangle::getSideLength(QLine side) const
{
    return sqrt(pow(side.x2() - side.x1(),2) + pow(side.y2() - side.y1(), 2));
}


