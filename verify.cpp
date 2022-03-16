#include "verify.h"

Verify::Verify()
{
    STATEMENTS.insert("FOR",statemens::FOR);
    STATEMENTS.insert("IF",statemens::IF);
    STATEMENTS.insert("WHILE",statemens::WHILE);

    INIT_TYPE.insert("CALL_FUNCTION",init_type::CALL_FUNCTION);
    INIT_TYPE.insert("INT",init_type::INT);
    INIT_TYPE.insert("INT_PTR",init_type::INT_PTR);
    INIT_TYPE.insert("DOUBLE",init_type::DOUBLE);
    INIT_TYPE.insert("DOUBLE_PTR",init_type::DOUBLE_PTR);

    CONDITION_TYPE.insert("EQUAL",condition_type::EQUAL);
    CONDITION_TYPE.insert("NOT_EQUAL",condition_type::NOT_EQUAL);
    CONDITION_TYPE.insert("AND",condition_type::AND);
    CONDITION_TYPE.insert("OR",condition_type::OR);
    CONDITION_TYPE.insert("GREATER",condition_type::GREATER);
    CONDITION_TYPE.insert("LESS",condition_type::LESS);
    CONDITION_TYPE.insert("GREATER_EQUAL",condition_type::GREATER_EQUAL);
    CONDITION_TYPE.insert("LESS_EQUAL",condition_type::LESS_EQUAL);
}

void Verify::jsonFromFile(QString way)
{
        QFile jsonFile;
        jsonFile.setFileName(way);
        QString test=jsonFile.fileName();
        qDebug()<<test;
        if(!jsonFile.exists())
        {
            qDebug()<<"file not exists";
        }
       if(jsonFile.open(QFile::ReadOnly))
           {
           QByteArray file=jsonFile.readAll();
            doc = QJsonDocument::fromJson(file,&err);//считываем json
            jsonFile.close();
           }
}
void Verify::controlAnalyze()
{
    while(!control.isUndefined())//если обьект определён
    {
    switch(INIT_TYPE.value(control.toObject().value("init_type").toString()))
        {
        case (init_type::INT):
        {
            QString name=control.toObject().value("init_name").toString();//достаём имя переменной
            ix=control.toObject().value("init").toInt();//достаём значение
            int_map.insert(name,ix);//вставляем в мап
            control=control.toObject().value("right_child");//идём на следующую строку
            break;
        }
        case (init_type::INT_PTR):
        {
        QJsonArray arr;
        arr=control.toObject().value("init").toArray();
        if(arr.count())
        {
        QString name=control.toObject().value("init_name").toString();//достаём имя переменной
        QVector<int> mass;//создаём вектор
        for(int i=0;i<arr.count();i++)
        mass.append(arr.at(i).toInt());//заполняем значениями из json
        int_ptr_map.insert(name,mass);//вставляем в мап
        control=control.toObject().value("right_child");//переключаем на следующую строку
        }
            break;
        }
        case (init_type::DOUBLE)://TO DO
        {
            break;
        }
        case (init_type::DOUBLE_PTR)://TO DO
        {
            break;
        }
        case (init_type::CALL_FUNCTION):
        {
            functionName=control.toObject().value("init_name").toString();//записываем имя вызываемой функции
            for(int i=0;i<array.count();i++)
            {
                if(array.at(i).toObject().value("func_name")==functionName)
                control=array.at(i).toObject().value("control_structure");
            }//ищем её среди массива функций
            controlAnalyze();
            break;
        }
        default:
        {
            qDebug()<<"Wrong init_type";
            break;
        }
        }
    }
}
void Verify::jsonAnalyze()
{
 if(err.errorString().toInt()==QJsonParseError::NoError)//если нормально прочитали документ
 {
    array=QJsonValue(doc.object().value("functions")).toArray();//достаем массив функций
    if(!array.isEmpty())//проверяем на пустоту
    {
        QJsonValue main=array.at(array.count()-1).toObject().value("name");//достаём последний элемент = функцию main
        control=array.at(array.count()-1).toObject().value("control_structure");//достаём обьект "контрольную структуру"
        if(main=="main")//проверяем main ли это
        {
        controlAnalyze();
        }
    }
 }
 else
     qDebug()<<"file parse error";
}
