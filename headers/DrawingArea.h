#ifndef DRAWINGAREA_H
#define DRAWINGAREA_H

#include <QWidget>
#include <QPainter>
#include <QPoint>
#include <QMouseEvent>
#include <QMessageBox>
#include <QRect>
#include <QList>
#include "Rectangle.h"
#include "Triangle.h"
#include "Ellipse.h"
#include "Connection.h"
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFileDialog>
#include <QFile>
#include <QCursor>

class DrawingArea : public QWidget
{
    Q_OBJECT

public:
    enum Tool{
        None,
        rectMode,
        triangleMode,
        ellipseMode,
        connectionMode,
        moveMode,
        deleteMode
    };

    QCursor cursor;

    DrawingArea(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent* ) override;
    void drawShapes(QPainter* painter);

    void setShapeId(int newShapeId);
    int getShapeId() const;

    void setCurrentTool(Tool tool);
    Tool getCurrentTool() const;

    void setShapes(QList<Shape*> shapes);
    QList<Shape*> getShapes() const;

    void setConnections(QList<Connection*> connections);
    QList<Connection*> getConnections() const;

private:
    bool isDrawing;
    bool isConnecting;
    bool isMoving;

    int shapeId;

    QList<Shape*> shapes;
    QList<Connection*> connections;

    QPoint startPoint;
    QPoint currentPoint;

    Shape* currentShape;
    Shape* firstConnectedShape;
    Connection* currentConnection;

    QRect oldPos;
    Tool currentTool;

    void startDrawing(QMouseEvent* event);
    void cancelEvent();

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
};

#endif // DRAWINGAREA_H
