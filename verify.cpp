#include "verify.h"

Verify::Verify()//конструктор по умолчанию. вносит в словари строки для осуществления ветвления в switch case
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


    OPERATIONS.insert("PLUS",operations::PLUS);
    OPERATIONS.insert("MINUS",operations::MINUS);
    OPERATIONS.insert("MULTIPLY",operations::MULTIPLY);
    OPERATIONS.insert("DIVISION",operations::DIVISION);
    OPERATIONS.insert("ASSIGMENT",operations::ASSIGMENT);
}

void Verify::jsonFromFile(QString way)//считыние файла из JSON документа, на вход подается путь.
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

void Verify::initAnalyze()//анализ массива инициализации в JSON документе и занесение их в словарь.
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

void Verify::controlAnalyze(int strategy)//анализ контрольной структуры JSON и генерация тестовых данных
{
    std::random_device rd;
    std::mt19937 gen(rd());
    QMapIterator<QString, int> iterator_int(int_map);
    QMapIterator<QString, double> iterator_double(double_map);
    switch (strategy)//выбор стратегии генерации тестовых данных
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
        int a = 0;
        double b = 0;
        while(iterator_int.hasNext())
        {
            std::uniform_int_distribution<> int_random(a,INT_FAST16_MAX);
                 iterator_int.next();
                 int_map[iterator_int.key()]=int_random(gen);
                 a=int_map[iterator_int.key()];
        }
        while(iterator_double.hasNext())
        {
            std::uniform_real_distribution<> double_random(b,INT_FAST16_MAX);
                 iterator_double.next();
                 double_map[iterator_double.key()]=double_random(gen);
                 b= double_map[iterator_double.key()];
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
            std::uniform_int_distribution<> int_random(a,0);
                 iterator_int.next();
                 int_map[iterator_int.key()]=int_random(gen);
                 a=int_map[iterator_int.key()];
        }
        while(iterator_double.hasNext())
        {
            std::uniform_real_distribution<> double_random(b,0);
                 iterator_double.next();
                 double_map[iterator_double.key()]=double_random(gen);
                 b=double_map[iterator_double.key()];
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
            std::uniform_int_distribution<> int_random(0,a);
                 iterator_int.next();
                 int_map[iterator_int.key()]=int_random(gen);
                 a=int_map[iterator_int.key()];
        }
        while(iterator_double.hasNext())
        {
            std::uniform_real_distribution<> double_random(0,b);
                 iterator_double.next();
                 double_map[iterator_double.key()]=double_random(gen);
                 b=double_map[iterator_double.key()];
        }
        break;
    }
    case 6://по убыванию отрицательные
    {
        qDebug()<<"In descending order negative:";
        int a = 0;
        double b=0;
        while(iterator_int.hasNext())
        {
             std::uniform_int_distribution<> int_random(INT_FAST16_MIN,a);
                 iterator_int.next();
                 int_map[iterator_int.key()]=int_random(gen);
                 a=int_map[iterator_int.key()];
        }
        while(iterator_double.hasNext())
        {
            std::uniform_real_distribution<> double_random(INT_FAST16_MIN,b);
                 iterator_double.next();
                 double_map[iterator_double.key()]=double_random(gen);
                 b=double_map[iterator_double.key()];
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
    if(statementAnalyze(control.value("statement"),res))//вызов анализ условий, в зависимости от результата осуществляется выбор ветвления в дереве
        {
        control = control.value("right_child").toObject();
        ifAction(control);
        }
    else
        {

            control = control.value("left_child").toObject();
                    ifAction(control);
        }
    }
    for (int i=0;i<res.count();i++)
        {
            QPair<QString, bool> pair = res.at(i);
            qDebug() << pair.first << " " << pair.second;
        }

}

bool Verify::conditionAnalyze(QJsonObject condition)//анализ условия, возвращает значение логического выражения
{
    QString cond = condition.value("condition_type").toString();
    double left=0;
    double right=0;
    if(condition.value("condition_left").isObject())//если левое условие сложное
    {
        left = conditionAnalyze(condition.value("condition_left").toObject());
    }
    if(condition.value("condition_right").isObject())//если правое условие сложное
    {
        right = conditionAnalyze(condition.value("condition_right").toObject());
    }
    if(condition.value("condition_left").isString())//если слева переменная из init list
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
    if(condition.value("condition_left").isDouble())//если слева цифра
        left=condition.value("condition_left").toDouble();
    if(condition.value("condition_right").isString())//если справа переменная из init list
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
    if(condition.value("condition_right").isDouble())//если справа цифра
        right=condition.value("condition_right").toDouble();
    switch(CONDITION_TYPE.value(cond))//ветвление в зависимости от типа логического оператора
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

bool Verify::statementAnalyze(QJsonValue statement, QList<QPair<QString,bool>>& result)//анализ условного оператора
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
        if(conditionAnalyze(control))//если логическое выражение истино
        {
            condition_map[control.value("condition").toString()]++;//логическое выражение true - фиксируем в map
            result.append(QPair<QString,bool>(control.value("condition").toString(),1));//фиксируем в список
            return true;
        }
        else//если логическое условие ложно
        {
            result.append(QPair<QString,bool>(control.value("condition").toString(),0));//логическое выражение false - фиксируем в список
            return false;
        }
            break;
        }
    }
}
double Verify::ifAction(QJsonObject action)//выполнение действия в условном операторе(присвоение переменной из map какого-то значения)
{
    int* left_assigment=NULL;//левый операнд присвоения
    int right_assigment=0;//правый операнд присвоения
    double left=0;//левый операнд арифмитических действий
    double right=0;//правый операнд арифмитических действий
if(!action.value("assigment_left").isUndefined())//устанавливаем указателю адрес значения из map для дальнейшего изменения
   left_assigment = &int_map[action.value("assigment_left").toString()];

if(action.value("assigment_right").isString())//если правый операнд просто элемент из map
{
   right_assigment=int_map[action.value("assigment_right").toString()];
}
if(action.value("assigment_right").isDouble())//если правый операнд просто число
{
   right_assigment=action.value("assigment_right").toDouble();
}
if(action.value("assigment_right").isObject())//если правый операнд обьект, вызываем функцию для анализа арифмитических действий
{
    right_assigment=ifAction(action.value("assigment_right").toObject());
}
if(action.value("action_left").isObject())//если левый операнд арифмитических действий обьект
{
    left=ifAction(action.value("action_left").toObject());//выполняем анализ обьекта
}
if(action.value("action_left").isString())//если левый операнд арифмитических действий элемент из init_list, ищем его и достаем значение
{
    switch(init_map.value(action.value("action_left").toString()))
    {
    case init_type::INT:
    {
        left=int_map.value(action.value("action_left").toString());
        break;
    }
    case init_type::DOUBLE:
    {
        left=double_map.value(action.value("action_left").toString());
        break;
    }
    }
}else if(action.value("action_left").isDouble())//если левый операнд арифмитических действий число
    left=action.value("action_left").toDouble();
if(action.value("action_right").isObject())//если правый операнд арифмитических действий обьект
{
    right=ifAction(action.value("action_right").toObject());//выполняем анализ обьекта
}
if(action.value("action_right").isString())//если правый операнд арифмитических действий элемент из init_list, ищем его и достаем значение
{
    switch(init_map.value(action.value("action_right").toString()))
    {
    case init_type::INT:
    {
        right=int_map.value(action.value("action_right").toString());
        break;
    }
    case init_type::DOUBLE:
    {
        right=double_map.value(action.value("action_right").toString());
        break;
    }
    }
}
else if(action.value("action_right").isDouble())//если правый операнд арифмитический действий число
    right=action.value("action_right").toDouble();

    switch(OPERATIONS.value(action.value("operation").toString()))//выполняем действие
    {
        case operations::ASSIGMENT:
        {
       *left_assigment=right_assigment;
           return 0;
        }
        case operations::PLUS:
        {
        return left+right;

        }
        case operations::MINUS:
        {
        return left-right;

        }
        case operations::MULTIPLY:
        {
        return left*right;

        }
        case operations::DIVISION:
        {
        return left/right;

        }
    }

}
void Verify::collectConditions(QJsonObject control)//собираем в map логические выражения для последующего вывода
{

    QJsonObject getCond = control;//рекурсивный обход дерева
    if(getCond.isEmpty()) return;
        if(!getCond.value("condition").isUndefined())
        condition_map.insert(getCond.value("condition").toString(),0);
        collectConditions(getCond.value("left_child").toObject());
        if(!getCond.value("condition").isUndefined())
        condition_map.insert(getCond.value("condition").toString(),0);
        collectConditions(getCond.value("right_child").toObject());
}

void Verify::jsonAnalyze()//анализ JSON документа
{
 if(err.errorString().toInt()==QJsonParseError::NoError)//если нормально прочитали документ
 {
    array=QJsonValue(doc.object().value("functions")).toArray();//достаем массив функций

    if(!array.isEmpty())//проверяем на пустоту
    {
        function=array.at(array.count()-1).toObject();//достаем функцию main
        control = function.value("control_structure").toObject();//приводим к обьекту
        collectConditions(control);//собираем логические условия
        initAnalyze();//инициализируем переменные
        for(int i=0;i<15;i++)//в цикле осуществляем генерацию тестовых данных и анализ условий
        {
        control = function.value("control_structure").toObject();
        controlAnalyze(i);

            QMapIterator<QString, int> iterator_int(int_map);
            while(iterator_int.hasNext())//выводим значения переменных типа INT
            {
                     iterator_int.next();
                     qDebug() << iterator_int.key()<<" = "<<iterator_int.value();
            }
            QMapIterator<QString, double> iterator_double(double_map);
            while(iterator_double.hasNext())//выводим значения переменных типа DOUBLE
            {
                     iterator_double.next();
                     qDebug() << iterator_double.key()<<" = "<<iterator_double.value();
            }
            qDebug()<<"----";
        }
        qDebug()<<"Count all true conditions:";
        QHashIterator<QString, int> iterator(condition_map);//выводим какие условия сколько раз выполнились
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
