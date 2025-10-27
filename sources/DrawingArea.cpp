#include "headers/DrawingArea.h"

DrawingArea::DrawingArea(QWidget *parent) {
    setMinimumSize(800, 600);
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);
    isDrawing = false;
    isConnecting = false;
    isMoving = false;
    shapeId = 0;
    currentTool = Tool::None;
    setCursor(cursor);
}

//обработчик рисования
void DrawingArea::paintEvent(QPaintEvent *)
{
    QPainter painter;
    painter.begin(this);
    drawShapes(&painter);
    if (isDrawing){
        currentShape->draw(&painter, QRect(startPoint, currentPoint));
        update();
    }
    if (isConnecting){
        currentConnection->setLine(QLine(startPoint, currentPoint));
        painter.drawLine(currentConnection->getLine());
        update();
    }
    painter.end();
}

//отрисовка уже имеющихся фигур и связей
void DrawingArea::drawShapes(QPainter *painter)
{
    foreach (Shape* shape, shapes){
        shape->draw(painter, QRect(shape->getTopLeft(), shape->getbottomRight()));
    }
    foreach(Connection* connection, connections){
        painter->drawLine(connection->getLine());
    }
    update();
}

void DrawingArea::setCurrentTool(Tool tool)
{
    currentTool = tool;
}

DrawingArea::Tool DrawingArea::getCurrentTool() const
{
    return this->currentTool;
}

void DrawingArea::setShapes(QList<Shape *> shapes)
{
    this->shapes = shapes;
}

QList<Shape *> DrawingArea::getShapes() const
{
    return this->shapes;
}

void DrawingArea::setConnections(QList<Connection *> connections)
{
    this->connections = connections;
}

QList<Connection *> DrawingArea::getConnections() const
{
    return this->connections;
}

void DrawingArea::setShapeId(int newShapeId)
{
    shapeId = newShapeId;
}

int DrawingArea::getShapeId() const
{
    return shapeId;
}

//обработка нажатий на левую кнопку мыши
void DrawingArea::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        switch(currentTool){
        case rectMode:{
            currentShape = new Rectangle();
            startDrawing(event);
            break;
        }
        case triangleMode:{
            currentShape = new Triangle();
            startDrawing(event);
            break;
        }
        case ellipseMode:{
            currentShape = new Ellipse();
            startDrawing(event);
            break;
        }
        case connectionMode:{
            //если ещё не соединяем, то создаём новую связь
            if (!isConnecting){
                foreach (Shape* shape, shapes){
                    if (shape->isInsideShape(event->pos())){
                        isConnecting = true;
                        currentConnection = new Connection();
                        currentConnection->setFirstId(shape->getId());
                        startPoint = shape->getCentrePoint();
                        currentPoint = event->pos();
                        currentConnection->setLine(QLine(startPoint, currentPoint));
                        firstConnectedShape = shape;
                        update();
                        break;
                    }
                }
            }
            //иначе проверяем на попадание мышью по фигуре
            else{
                bool isFound = false;
                foreach (Shape* shape, shapes){
                    if (shape->isInsideShape(event->pos()) && shape != firstConnectedShape){
                        currentPoint = shape->getCentrePoint();
                        currentConnection->setSecondId(shape->getId());
                        currentConnection->setLine(QLine(startPoint, currentPoint));
                        connections.append(currentConnection);
                        isFound = true;
                        break;
                    }
                }
                if (!isFound){
                    delete currentConnection;
                }
                isConnecting = false;
            }
            break;
        }
        case moveMode:{
            foreach(Shape* shape, shapes){
                if (shape->isInsideShape(event->pos())){
                    isMoving = true;
                    cursor.setShape(Qt::ClosedHandCursor);
                    setCursor(cursor);
                    currentShape = shape;
                    oldPos = QRect(currentShape->getTopLeft(), currentShape->getbottomRight());
                    startPoint = event->pos();
                    currentPoint = event->pos();
                    update();
                    break;
                }
            }
            break;
        }
        case deleteMode:{
            int elPos = 0;
            //удаляем фигуру и все её связи
            foreach (Shape* shape, shapes){
                if (shape->isInsideShape(event->pos())){
                    int conPos = 0;
                    foreach(Connection* connection, connections){
                        if (connection->getFirstId() == shape->getId() || connection->getSecondId() == shape->getId()){
                            connections.remove(conPos);
                            delete connection;
                        }
                        else{
                            conPos++;
                        }
                    }
                    shapes.remove(elPos);
                    delete shape;
                    break;
                }
                elPos++;
            }
            update();
            break;
        }
        }
    }
    else if(event->button() == Qt::RightButton){
        cancelEvent();
    }
}

//обработка движения мыши
void DrawingArea::mouseMoveEvent(QMouseEvent *event)
{
    if(isDrawing){
        if (event->pos().x() >= startPoint.x() && event->pos().y() >= startPoint.y()){
            currentPoint = event->pos();
            update();
        }
    }
    if(isConnecting){
        currentPoint = event->pos();
        update();
    }
    if(isMoving){
        //перемещаем фигуру и связи вслед за ней
        currentPoint = event->pos();
        currentShape->moveShape(currentPoint - startPoint);
        startPoint = currentPoint;
        foreach (Connection* connection, connections){
            if (connection->getFirstId() == currentShape->getId()){
                connection->setLine(QLine(currentShape->getCentrePoint(), connection->getLine().p2()));
            }
            else if (connection->getSecondId() == currentShape->getId()){
                connection->setLine(QLine(connection->getLine().p1(), currentShape->getCentrePoint()));
            }
        }
    }
}

//обработка отпускания кнопки мыши
void DrawingArea::mouseReleaseEvent(QMouseEvent *event)
{
    //отменяем рисование или движение
    if (isDrawing){
        if (currentPoint != startPoint){
            currentShape->setId(shapeId);
            currentShape->setTopLeft(startPoint);
            currentShape->setBottomRight(currentPoint);
            shapeId++;
            shapes.push_front(currentShape);
        }
        isDrawing = false;
    }
    else if (isMoving){
        currentPoint = event->pos();
        isMoving = false;
        cursor.setShape(Qt::OpenHandCursor);
        setCursor(cursor);
    }
}

void DrawingArea::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape){
        cancelEvent();
    }
}

void DrawingArea::startDrawing(QMouseEvent* event)
{
    isDrawing = true;
    startPoint = event->pos();
    currentPoint = event->pos();
    update();
}

void DrawingArea::cancelEvent()
{
    if (isDrawing){
        isDrawing = false;
        delete currentShape;
    }
    else if (isConnecting){
        isConnecting = false;
        delete currentConnection;
    }
    else if (isMoving){
        isMoving = false;
        currentShape->setShapeFigure(QRect(oldPos.topLeft(), oldPos.bottomRight()));
        foreach (Connection* connection, connections){
            if (connection->getFirstId() == currentShape->getId()){
                connection->setLine(QLine(currentShape->getCentrePoint(), connection->getLine().p2()));
            }
            else if (connection->getSecondId() == currentShape->getId()){
                connection->setLine(QLine(connection->getLine().p1(), currentShape->getCentrePoint()));
            }
        }
        cursor.setShape(Qt::OpenHandCursor);
        setCursor(cursor);
    }
    update();
}
