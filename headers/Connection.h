#ifndef CONNECTION_H
#define CONNECTION_H

#include <QLine>
#include <QDebug>

class Connection
{
public:
    Connection();
    ~Connection();

    //line
    void setLine(QLine line){ this->line = line; }
    QLine getLine(){ return this->line; }

    //firstId
    void setFirstId(int firstId){ this->firstId = firstId; }
    int getFirstId(){ return this->firstId; }

    //secondId
    void setSecondId(int secondId){ this->secondId = secondId; }
    int getSecondId(){ return this->secondId; }

private:
    QLine line;
    int firstId;
    int secondId;
};

#endif // CONNECTION_H
