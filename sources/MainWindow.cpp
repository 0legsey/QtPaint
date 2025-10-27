#include "headers/MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupToolBar();
    drawingArea = new DrawingArea(this);
    setCentralWidget(drawingArea);
}

MainWindow::~MainWindow()
{

}

//функция для установки верхней панели с кнопками
void MainWindow::setupToolBar()
{
    toolBar = addToolBar("Основные инструменты");

    QIcon rectangleIcon = QIcon("./icons/rectangle.png");
    QIcon triangleIcon = QIcon("./icons/triangle.png");
    QIcon circleIcon = QIcon("./icons/circle.png");
    QIcon connectionIcon = QIcon("./icons/line.png");
    QIcon moveIcon = QIcon("./icons/hand.png");
    QIcon deleteIcon = QIcon("./icons/bin.png");
    QIcon saveIcon = QIcon("./icons/save.png");
    QIcon loadIcon = QIcon("./icons/folder.png");

    QAction* rectangleAction = new QAction("Прямоугольник", this);
    rectangleAction->setIcon(rectangleIcon);
    QAction* triangleAction = new QAction("Треугольник", this);
    triangleAction->setIcon(triangleIcon);
    QAction* ellipseAction = new QAction("Эллипс", this);
    ellipseAction->setIcon(circleIcon);
    QAction* connectAction = new QAction("Связь", this);
    connectAction->setIcon(connectionIcon);
    QAction* moveAction = new QAction("Переместить", this);
    moveAction->setIcon(moveIcon);
    QAction* deleteAction = new QAction("Удалить", this);
    deleteAction->setIcon(deleteIcon);
    QAction* saveAction = new QAction("Сохранить", this);
    saveAction->setIcon(saveIcon);
    QAction* loadAction = new QAction("Загрузить", this);
    loadAction->setIcon(loadIcon);

    toolBar->addAction(rectangleAction);
    toolBar->addAction(triangleAction);
    toolBar->addAction(ellipseAction);
    toolBar->addSeparator();
    toolBar->addAction(connectAction);
    toolBar->addSeparator();
    toolBar->addAction(moveAction);
    toolBar->addAction(deleteAction);
    toolBar->addSeparator();
    toolBar->addAction(saveAction);
    toolBar->addAction(loadAction);

    toolBar->setMovable(false);

    connect(rectangleAction, SIGNAL(triggered()), this, SLOT(onRectangleClicked()));
    connect(triangleAction, SIGNAL(triggered()), this, SLOT(onTriangleClicked()));
    connect(ellipseAction, SIGNAL(triggered()), this, SLOT(onEllipseClicked()));
    connect(connectAction, SIGNAL(triggered()), this, SLOT(onConnectionClicked()));
    connect(moveAction, SIGNAL(triggered()), this, SLOT(onMoveClicked()));
    connect(deleteAction, SIGNAL(triggered()), this, SLOT(onDeleteClicked()));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(onSaveClicked()));
    connect(loadAction, SIGNAL(triggered()), this, SLOT(onLoadClicked()));
}

//обработка нажатий на кнопки
void MainWindow::onRectangleClicked()
{
    setDrawingAreaCursor(Qt::ArrowCursor);
    drawingArea->setCurrentTool(DrawingArea::Tool::rectMode);
}

void MainWindow::onTriangleClicked()
{
    setDrawingAreaCursor(Qt::ArrowCursor);
    drawingArea->setCurrentTool(DrawingArea::Tool::triangleMode);
}

void MainWindow::onEllipseClicked()
{
    setDrawingAreaCursor(Qt::ArrowCursor);
    drawingArea->setCurrentTool(DrawingArea::Tool::ellipseMode);
}

void MainWindow::onConnectionClicked()
{
    setDrawingAreaCursor(Qt::ArrowCursor);
    drawingArea->setCurrentTool(DrawingArea::Tool::connectionMode);
}

void MainWindow::onMoveClicked()
{
    setDrawingAreaCursor(Qt::OpenHandCursor);
    drawingArea->setCurrentTool(DrawingArea::Tool::moveMode);
}

void MainWindow::onDeleteClicked()
{
    QPixmap resizedPixmap = pixmap.scaled(15, 15, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    drawingArea->cursor = QCursor(resizedPixmap);
    drawingArea->setCursor(drawingArea->cursor);
    drawingArea->setCurrentTool(DrawingArea::Tool::deleteMode);
}

void MainWindow::onSaveClicked()
{
    setDrawingAreaCursor(Qt::ArrowCursor);
    saveToFile();
    drawingArea->setCurrentTool(DrawingArea::Tool::None);
}

void MainWindow::onLoadClicked()
{
    setDrawingAreaCursor(Qt::ArrowCursor);
    loadFromFile();
    drawingArea->setCurrentTool(DrawingArea::Tool::None);
}

void MainWindow::setDrawingAreaCursor(Qt::CursorShape shape)
{
    drawingArea->cursor.setShape(shape);
    drawingArea->setCursor(drawingArea->cursor);
}

//загружаем информацию из json-файла
void MainWindow::loadFromFile()
{
    qDebug() << "loading...";
    QString jsonFileName = QFileDialog::getOpenFileName(this, "Load Drawing", "", "JSON files (*.json)");
    QFile file(jsonFileName);
    QMessageBox mb;

    if (file.open(QIODevice::ReadOnly)){
        QByteArray bytes = file.readAll();
        file.close();

        QJsonParseError jsonError;
        QJsonDocument jsonDocument = QJsonDocument::fromJson(bytes, &jsonError);
        QString errorText;
        if (jsonError.error != QJsonParseError::NoError){
            errorText = "Ошибка при загрузке json-файла: " + jsonError.errorString();
            mb.setText(errorText);
        }
        else {
            mb.setText("Файл успешно загружен");
        }
        mb.exec();

        QJsonObject obj = jsonDocument.object();
        QJsonValue nextId = obj["nextId"];
        drawingArea->setShapeId(nextId.toInt());
        QJsonArray shapesArray = obj["shapes"].toArray();
        QJsonArray connectionsArray = obj["connections"].toArray();

        QList<Shape*> shapes;
        QList<Connection*> connections;

        foreach(Shape* shape, drawingArea->getShapes()){
            delete shape;
        }
        drawingArea->setShapes(shapes);

        foreach(Connection* connections, drawingArea->getConnections()){
            delete connections;
        }
        drawingArea->setConnections(connections);

        shapes = loadShapes(shapesArray);
        drawingArea->setShapes(shapes);

        connections = loadConnections(connectionsArray);
        drawingArea->setConnections(connections);

    }
    else{
        mb.setText("Ошибка при открытии файла");
    }
}

//функция отвечающая за загрузку фигур
QList<Shape *> MainWindow::loadShapes(QJsonArray shapesArray)
{
    QList<Shape*> shapes;
    foreach(const auto el, shapesArray){
        QJsonObject shapeJson = el.toObject();
        Shape* shape;
        if (shapeJson["type"].toString() == "rectangle"){
            shape = new Rectangle();
        }
        else if (shapeJson["type"].toString() == "triangle"){
            shape = new Triangle();
        }
        else{
            shape = new Ellipse();
        }

        QPoint topLeft = QPoint(shapeJson["topLeftX"].toInt(), shapeJson["topLeftY"].toInt());
        QPoint bottomRight = QPoint(shapeJson["bottomRightX"].toInt(), shapeJson["bottomRightY"].toInt());

        shape->setId(shapeJson["id"].toInt());
        shape->setTopLeft(topLeft);
        shape->setBottomRight(bottomRight);
        shape->setShapeFigure(QRect(topLeft, bottomRight));
        shapes.push_front(shape);
    }
    return shapes;
}

//функция отвечающая за загрузку связей
QList<Connection *> MainWindow::loadConnections(QJsonArray connectionsArray)
{
    QList<Connection*> connections;
    foreach(const auto el, connectionsArray){
        QJsonObject connectionJson = el.toObject();
        Connection* connection = new Connection();

        QPoint firstPoint = QPoint(connectionJson["firstPointX"].toInt(), connectionJson["firstPointY"].toInt());
        QPoint secondPoint = QPoint(connectionJson["secondPointX"].toInt(), connectionJson["secondPointY"].toInt());

        connection->setLine(QLine(firstPoint, secondPoint));
        connection->setFirstId(connectionJson["firstId"].toInt());
        connection->setSecondId(connectionJson["secondId"].toInt());

        connections.push_front(connection);
    }
    return connections;
}

//функция сохранения информации в json-файл
void MainWindow::saveToFile()
{
    qDebug() << "saving...";
    QMessageBox mb;
    QString jsonFileName = QFileDialog::getSaveFileName(this, tr("Save Drawing"), "", "JSON files (*.json)");
    if (!jsonFileName.isEmpty()){
        QJsonObject jsonForm;
        QJsonArray shapesJson;
        QJsonArray connectionsJson;
        QJsonObject connectionJson;
        QJsonObject shapeJson;
        foreach (Shape* shape, drawingArea->getShapes()){
            shapeJson.insert("id", shape->getId());
            shapeJson.insert("type", shape->getType());
            shapeJson.insert("topLeftX", shape->getTopLeft().x());
            shapeJson.insert("topLeftY", shape->getTopLeft().y());
            shapeJson.insert("bottomRightX", shape->getbottomRight().x());
            shapeJson.insert("bottomRightY", shape->getbottomRight().y());
            shapesJson.push_back(shapeJson);
        }
        foreach (Connection* connection, drawingArea->getConnections()){
            connectionJson.insert("firstPointX", connection->getLine().p1().x());
            connectionJson.insert("firstPointY", connection->getLine().p1().y());
            connectionJson.insert("secondPointX", connection->getLine().p2().x());
            connectionJson.insert("secondPointY", connection->getLine().p2().y());
            connectionJson.insert("firstId", connection->getFirstId());
            connectionJson.insert("secondId", connection->getSecondId());
            connectionsJson.push_back(connectionJson);
        }
        jsonForm.insert("nextId", drawingArea->getShapeId());
        jsonForm.insert("shapes", shapesJson);
        jsonForm.insert("connections", connectionsJson);
        QJsonDocument jsonDoc(jsonForm);
        QByteArray bytes = jsonDoc.toJson();
        QFile file(jsonFileName);
        if(file.open(QIODevice::WriteOnly | QIODevice::Text)){
            file.write(bytes);
            file.close();
            mb.setText("Файл успешно сохранён!");
            mb.exec();
        }
        else{
            mb.setText("Ошибка при записи в файл");
            mb.exec();
        }
    }
}
