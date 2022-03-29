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

void Verify::initAnalyze()
{
QJsonArray init_list=function.value("init_list").toArray();
for(int i=0;i<init_list.count();i++)
{
    QString init_type=init_list.at(i).toObject().value("init_type").toString();
    switch(INIT_TYPE.value(init_type))
        {
        case (init_type::INT):
        {
            QString name=init_list.at(i).toObject().value("init_name").toString();//достаём имя переменной
            int ix=NULL;
            if(!init_list.at(i).toObject().value("init").isUndefined())
            ix=init_list.at(i).toObject().value("init").toInt();//достаём значение
            int_map.insert(name,ix);//вставляем в мап
            init_map.insert(name,init_type::INT);
            break;
        };
        case (init_type::INT_PTR):
        {
        QJsonArray arr;
        arr=init_list.at(i).toObject().value("init").toArray();
        if(arr.count())
        {
        QString name=init_list.at(i).toObject().value("init_name").toString();//достаём имя переменной
        QVector<int> mass;//создаём вектор
        for(int i=0;i<arr.count();i++)
        mass.append(arr.at(i).toInt());//заполняем значениями из json
        int_ptr_map.insert(name,mass);//вставляем в мап
        init_map.insert(name,init_type::INT_PTR);
        }

        break;
        }
        case (init_type::DOUBLE):
        {
        QString name=init_list.at(i).toObject().value("init_name").toString();//достаём имя переменной
        double dx=NULL;
        if(!init_list.at(i).toObject().value("init").isUndefined())
        dx=init_list.at(i).toObject().value("init").toDouble();//достаём значение
        double_map.insert(name,dx);//вставляем в мап
        init_map.insert(name,init_type::DOUBLE);
        break;
        }
        case (init_type::DOUBLE_PTR):
        {
        QJsonArray arr;
        arr=init_list.at(i).toObject().value("init").toArray();
        if(arr.count())
        {
        QString name=init_list.at(i).toObject().value("init_name").toString();//достаём имя переменной
        QVector<double> mass;//создаём вектор
        for(int i=0;i<arr.count();i++)
        mass.append(arr.at(i).toDouble());//заполняем значениями из json
        double_ptr_map.insert(name,mass);//вставляем в мап
        init_map.insert(name,init_type::DOUBLE_PTR);
        }
        break;
        }
        default:
        {
        break;
        }
        }
}
}

void Verify::controlAnalyze(int strategy)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    QMapIterator<QString, int> iterator_int(int_map);
    QMapIterator<QString, double> iterator_double(double_map);
    switch (strategy)
    {
    case 0://все 0
    {
        qDebug()<<"All zeros:";
        while(iterator_int.hasNext())
        {
                 iterator_int.next();
                 int_map[iterator_int.key()]=0;
        }
        while(iterator_double.hasNext())
        {
                 iterator_double.next();
                 double_map[iterator_double.key()]=0;
        }
        break;
    }
    case 1://все одинаковые положительные
    {
        qDebug()<<"All equal positive:";
        while(iterator_int.hasNext())
        {
                 iterator_int.next();
                 int_map[iterator_int.key()]=10;
        }
        while(iterator_double.hasNext())
        {
                 iterator_double.next();
                 double_map[iterator_double.key()]=10;
        }
        break;
    }
    case 2://все одинаковые отрицательные
    {
        qDebug()<<"All equal negative:";
        while(iterator_int.hasNext())
        {
                 iterator_int.next();
                 int_map[iterator_int.key()]=-10;
        }
        while(iterator_double.hasNext())
        {
                 iterator_double.next();
                 double_map[iterator_double.key()]=-10;
        }
        break;
    }
    case 3://по возрастанию положительные
    {
        qDebug()<<"In ascending order positive:";
        int a = 1;
        double b = 1;
        while(iterator_int.hasNext())
        {
                 iterator_int.next();
                 int_map[iterator_int.key()]=a;
                 a++;
        }
        while(iterator_double.hasNext())
        {
                 iterator_double.next();
                 double_map[iterator_double.key()]=b;
                 b++;
        }
        break;
    }
    case 4://по возрастанию отрицательные
    {
         qDebug()<<"In ascending order negative:";
        int a = INT_FAST16_MIN;
        double b = INT_FAST16_MIN;
        while(iterator_int.hasNext())
        {
                 iterator_int.next();
                 int_map[iterator_int.key()]=a;
                 a++;
        }
        while(iterator_double.hasNext())
        {
                 iterator_double.next();
                 double_map[iterator_double.key()]=b;
                 b++;
        }
        break;
    }
    case 5://по убыванию положительные
    {
        qDebug()<<"In descending order positive:";
        int a = INT_FAST16_MAX;
        double b = INT_FAST16_MAX;
        while(iterator_int.hasNext())
        {
                 iterator_int.next();
                 int_map[iterator_int.key()]=a;
                 a--;
        }
        while(iterator_double.hasNext())
        {
                 iterator_double.next();
                 double_map[iterator_double.key()]=b;
                 b--;
        }
        break;
    }
    case 6://по убыванию отрицательные
    {
        qDebug()<<"In descending order negative:";
        int a = -1;
        double b=-1;
        while(iterator_int.hasNext())
        {
                 iterator_int.next();
                 int_map[iterator_int.key()]=a;
                 a--;
        }
        while(iterator_double.hasNext())
                {
                         iterator_double.next();
                         double_map[iterator_double.key()]=b;
                         b--;
                }
        break;
    }
    case 7://рандомные от -100 до 100
    {
        qDebug()<<"Random from -100 to 100:";
        std::uniform_int_distribution<> int_random(-100,100);
        std::uniform_real_distribution<> double_random(-100,100);
        while(iterator_int.hasNext())
        {
                 iterator_int.next();
                 int_map[iterator_int.key()]=int_random(gen);
        }
        while(iterator_double.hasNext())
                {
                         iterator_double.next();
                         double_map[iterator_double.key()]=double_random(gen);
                }
        break;
    }
    case 8://рандомные от 0 до 100
    {
        qDebug()<<"Random from 0 to 100:";
        std::uniform_int_distribution<> int_random(0,100);
        std::uniform_real_distribution<> double_random(0,100);
        while(iterator_int.hasNext())
        {
                 iterator_int.next();
                 int_map[iterator_int.key()]=int_random(gen);
        }
        while(iterator_double.hasNext())
                {
                         iterator_double.next();
                         double_map[iterator_double.key()]=double_random(gen);
                }
        break;
    }
    case 9://рандомные от -100 до 0
    {
        qDebug()<<"Random from -100 to 0:";
        std::uniform_int_distribution<> int_random(-100,0);
        std::uniform_real_distribution<> double_random(-100,0);
        while(iterator_int.hasNext())
        {
                 iterator_int.next();
                 int_map[iterator_int.key()]=int_random(gen);
        }
        while(iterator_double.hasNext())
                {
                         iterator_double.next();
                         double_map[iterator_double.key()]=double_random(gen);
                }
        break;
    }
    default:
        qDebug()<<"Random from -100 to 100:";
        std::uniform_int_distribution<> int_random(-100,100);
        std::uniform_real_distribution<> double_random(-100,100);
        while(iterator_int.hasNext())
        {
                 iterator_int.next();
                 int_map[iterator_int.key()]=int_random(gen);
        }
        while(iterator_double.hasNext())
                {
                         iterator_double.next();
                         double_map[iterator_double.key()]=double_random(gen);
                }
        break;
    }
    QList<QPair<QString,bool>> res;
    while(!control.isEmpty())
    {
    if(statementAnalyze(control.value("statement"),res))
    control = control.value("right_child").toObject();
    else control = control.value("left_child").toObject();
    }
    for (int i=0;i<res.count();i++)
    {
        QPair<QString, bool> pair = res.at(i);
        qDebug() << pair.first << " " << pair.second;
    }

}

bool Verify::conditionAnalyze(QJsonObject condition)//возвращает результат анализа условия
{
    QString cond = condition.value("condition_type").toString();
    double left=0;
    double right=0;
    if(condition.value("condition_left").isObject())
    {
        left = conditionAnalyze(condition.value("condition_left").toObject());
    }
    if(condition.value("condition_right").isObject())
    {
        right = conditionAnalyze(condition.value("condition_right").toObject());
    }
    if(condition.value("condition_left").isString())
    {
        switch(init_map.value(condition.value("condition_left").toString()))
        {
        case init_type::INT:
        {
            left=int_map.value(condition.value("condition_left").toString());
            break;
        }
        case init_type::DOUBLE:
        {
            left=double_map.value(condition.value("condition_left").toString());
            break;
        }
        }
    }
    if(condition.value("condition_left").isDouble())
        left=condition.value("condition_left").toDouble();
    if(condition.value("condition_right").isString())
    {
        switch(init_map.value(condition.value("condition_right").toString()))
        {
        case init_type::INT:
        {
            right=int_map.value(condition.value("condition_right").toString());
            break;
        }
        case init_type::DOUBLE:
        {
            right=double_map.value(condition.value("condition_right").toString());
            break;
        }
        }
    }
    if(condition.value("condition_right").isDouble())
        right=condition.value("condition_right").toDouble();
    switch(CONDITION_TYPE.value(cond))
    {
    case  condition_type::EQUAL:
    {
        return left==right;
        break;
    }
    case  condition_type::NOT_EQUAL:
    {
        return left!=right;
        break;
    }
    case  condition_type::AND:
    {
        return left&&right;
        break;
    }
    case  condition_type::OR:
     {
        return left||right;
        break;
    }
    case  condition_type::GREATER:
    {
        return left>right;
        break;
    }
    case  condition_type::LESS:
    {
       return left<right;
        break;
    }
    case  condition_type::GREATER_EQUAL:
    {
        return left>=right;
        break;
    }
    case  condition_type::LESS_EQUAL:
    {
        return left<=right;
        break;
    }
    }
}

bool Verify::statementAnalyze(QJsonValue statement, QList<QPair<QString,bool>>& result)
{
    switch(STATEMENTS.value(statement.toString()))
    {
        case  (statemens::FOR):
        {
            break;
        }
        case  (statemens::WHILE):
        {
            break;
        }
        case  (statemens::IF):
        {
        if(conditionAnalyze(control))
        {
            condition_map[control.value("condition").toString()]++;//логическое выражение true - фиксируем в map
            result.append(QPair<QString,bool>(control.value("condition").toString(),1));
            return true;
        }
        else
        {
            result.append(QPair<QString,bool>(control.value("condition").toString(),0));
            return false;
        }
            break;
        }
    }
}

void Verify::collectConditions(QJsonObject control)//собираем в map логические выражения
{

    QJsonObject getCond = control;
    if(getCond.isEmpty()) return;
        condition_map.insert(getCond.value("condition").toString(),0);
        collectConditions(getCond.value("left_child").toObject());
        condition_map.insert(getCond.value("condition").toString(),0);
        collectConditions(getCond.value("right_child").toObject());
}

void Verify::jsonAnalyze()
{
 if(err.errorString().toInt()==QJsonParseError::NoError)//если нормально прочитали документ
 {
    array=QJsonValue(doc.object().value("functions")).toArray();//достаем массив функций

    if(!array.isEmpty())//проверяем на пустоту
    {
        function=array.at(array.count()-1).toObject();
        control = function.value("control_structure").toObject();
        collectConditions(control);
        initAnalyze();
        for(int i=0;i<10;i++)
        {
        control = function.value("control_structure").toObject();
        controlAnalyze(i);

            QMapIterator<QString, int> iterator_int(int_map);
            while(iterator_int.hasNext())
            {
                     iterator_int.next();
                     qDebug() << iterator_int.key()<<" = "<<iterator_int.value();
            }
            QMapIterator<QString, double> iterator_double(double_map);
            while(iterator_double.hasNext())
            {
                     iterator_double.next();
                     qDebug() << iterator_double.key()<<" = "<<iterator_double.value();
            }
            qDebug()<<"----";
        }
        qDebug()<<"Count all true conditions:";
        QHashIterator<QString, int> iterator(condition_map);
        while(iterator.hasNext())
        {
                 iterator.next();
                 qDebug() << iterator.key()<<" = "<< iterator.value();
        }

     }
  }
 else
     qDebug()<<"file parse error";
}
