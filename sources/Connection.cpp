#include "headers/Connection.h"

Connection::Connection() {
    qDebug() << "Связь создана";
}

Connection::~Connection() {
    qDebug() << "Связь была удалена";
}
