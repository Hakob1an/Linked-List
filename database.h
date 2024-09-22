#ifndef DATABASE_H
#define DATABASE_H

#include <QtSql>
#include <QDebug>
#include "linkedlist.h"

class Database {
public:
    Database();
    ~Database();

    bool connect();
    void saveList(const LinkedList<int>& list);
    LinkedList<int>* loadList();

private:
    QSqlDatabase db_;
};

#endif // DATABASE_H
