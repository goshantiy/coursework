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
#include <QVector>
#include <random>
#include <QPair>
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
    void initAnalyze();
    void controlAnalyze();
    bool statementAnalyze(QJsonValue, QList<QPair<QString,bool>>&);
    bool conditionAnalyze(QJsonObject);
    auto findInMap(QString name);
private:


    QMap<QString,statemens> STATEMENTS;
    QMap<QString,init_type> INIT_TYPE;
    QMap<QString,condition_type> CONDITION_TYPE;

    QJsonObject control;
    QJsonArray array;
    QString functionName;
    QJsonDocument doc;
    QJsonParseError err;
    QJsonObject function;



    QMap<QString, init_type> init_map;
    QMap<QString,int> int_map;
    QMap<QString, double> double_map;
    QMap<QString,QVector<int> > int_ptr_map;
    QMap<QString,QVector<double> > double_ptr_map;

};

#endif // VERIFY_H
