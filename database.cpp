#include "database.h"
#include <QStandardPaths>
#include <QCoreApplication>


Database::Database() {
    QString dbPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/linkedlist.db";

    //Checking if the dir exist >_<
    QDir dir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    db_ = QSqlDatabase::addDatabase("QSQLITE");
    db_.setDatabaseName(dbPath);

    qDebug() << "Database path:" << db_.databaseName();
    qDebug() << "Available drivers:" << QSqlDatabase::drivers();
}

Database::~Database() {
    if (db_.isOpen()) {
        db_.close();
    }
}

bool Database::connect() {
    if (!db_.open()) {
        qDebug() << "Error: connection with database failed:" << db_.lastError().text();
        return false;
    }

    qDebug() << "Database: connection ok";

    QSqlQuery query(db_);
    if (!query.exec("CREATE TABLE IF NOT EXISTS list (id INTEGER PRIMARY KEY, data INTEGER)")) {
        qDebug() << "Failed to create table 'list':" << query.lastError().text();
        return false;
    } else {
        qDebug() << "Table 'list' created or already exists";
    }
    return true;
}

void Database::saveList(const LinkedList<int>& list) {
    if (!db_.isOpen()) {
        qDebug() << "Database is not open!";
        return;
    }

    QSqlQuery query(db_);
    if (!query.exec("DELETE FROM list")) {
        qDebug() << "Error clearing the list table:" << query.lastError().text();
    }

    Node<int>* current = list.head();
    int id = 0;
    while (current) {
        query.prepare("INSERT INTO list (id, data) VALUES (:id, :data)");
        query.bindValue(":id", id++);
        query.bindValue(":data", current->data);
        if (!query.exec()) {
            qDebug() << "Error inserting data into 'list':" << query.lastError().text();
        }
        current = current->next;
    }
}

LinkedList<int>* Database::loadList() {
    LinkedList<int>* list = new LinkedList<int>();
    if (!db_.isOpen()) {
        qDebug() << "Database is not open!";
        return list;
    }

    QSqlQuery query(db_);
    if (!query.exec("SELECT data FROM list ORDER BY id")) {
        qDebug() << "Error reading from the list table:" << query.lastError().text();
        return list;
    }

    Node<int>* lastNode = nullptr;
    while (query.next()) {
        int data = query.value(0).toInt();
        if (lastNode == nullptr) {
            list->addAfter(nullptr, data);
            lastNode = list->head();
        } else {
            list->addAfter(lastNode, data);
            lastNode = lastNode->next;
        }
    }
    return list;
}

