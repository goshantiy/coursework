#ifndef VERIFY_H
#define VERIFY_H
#include <QApplication>
#include <QFile>
#include <QFileDialog>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QString>
#include <iostream>

class Verify
{
public:
    enum class statemens
    {
        FOR,
        IF,
        WHILE

    };
    enum class init_type
    {
       INT,
       DOUBLE,
       INT_PTR,
       DOUBLE_PTR,
       CALL_FUNCTION
    };
    enum class condition_type
    {
        EQUAL,
        NOT_EQUAL,
        AND,
        OR,
        GREATER,
        LESS,
        GREATER_EQUAL,
        LESS_EQUAL
    };

    Verify();
    void jsonFromFile(QString way);
    void jsonAnalyze();
    void controlAnalyze();
private:
    QMap<QString,statemens> STATEMENTS;
    QMap<QString,init_type> INIT_TYPE;
    QMap<QString,condition_type> CONDITION_TYPE;

    QJsonArray array;
    QString functionName;
    QJsonValue control;
    QJsonDocument doc;
    QJsonParseError err;

    int ix=0;
    QMap<QString,int> int_map;
    QMap<QString, double> double_map;
    QMap<QString,int*> int_ptr_map;
};

#endif // VERIFY_H
