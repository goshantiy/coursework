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
#include <mutex>
#include <QtGlobal>
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
    enum class operations
    {
        PLUS,
        MINUS,
        MULTIPLY,
        DIVISION,
        ASSIGMENT,
        MOD
    };
    Verify();
    void jsonFromFile(QString way);
    void jsonAnalyze();

private:
    void initAnalyze();
    void controlAnalyze(int);
    bool statementAnalyze(QJsonValue, QList<QPair<QString,bool>>&);
    bool conditionAnalyze(QJsonObject);
    void collectConditions(QJsonObject);
    double ifAction(QJsonObject);

    QHash<QString,statemens> STATEMENTS;
    QHash<QString,init_type> INIT_TYPE;
    QHash<QString,condition_type> CONDITION_TYPE;
    QHash<QString,operations> OPERATIONS;

    QJsonObject control;
    QJsonArray array;
    QString functionName;
    QJsonDocument doc;
    QJsonParseError err;
    QJsonObject function;


    QHash<QString, int> condition_map;
    QHash<QString, init_type> init_map;
    QMap<QString,int> int_map;
    QMap<QString, double> double_map;
    QHash<QString,QVector<int> > int_ptr_map;
    QHash<QString,QVector<double> > double_ptr_map;

};

#endif // VERIFY_H
