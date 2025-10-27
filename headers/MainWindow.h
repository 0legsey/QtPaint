#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QToolBar>
#include <QMessageBox>
#include <QApplication>
#include "DrawingArea.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void setupToolBar();
    void setDrawingAreaCursor(Qt::CursorShape shape);
    void loadFromFile();
    QList<Shape*> loadShapes(QJsonArray shapesArray);
    QList<Connection*> loadConnections(QJsonArray connectionsArray);
    void saveToFile();

    DrawingArea *drawingArea;
    QToolBar *toolBar;
    QPixmap pixmap = QPixmap("./icons/delete.png");

private slots:
    void onRectangleClicked();
    void onTriangleClicked();
    void onEllipseClicked();
    void onConnectionClicked();
    void onMoveClicked();
    void onDeleteClicked();
    void onSaveClicked();
    void onLoadClicked();

};
#endif // MAINWINDOW_H
