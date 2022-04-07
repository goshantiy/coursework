#include "mainwindow.h"
#include <verify.h>
#include <thread>

int main(int argc, char *argv[])
{
QApplication a(argc, argv);
//    MainWindow w;
//    w.show();
Verify doc;
//doc.jsonFromFile("C:/Users/PC/Documents/coursework/test.json");
//doc.jsonFromFile("C:/Users/PC/Documents/coursework/test2.json");
//doc.jsonFromFile("C:/Users/PC/Documents/coursework/test3.json");
//doc.jsonFromFile("C:/Users/PC/Documents/coursework/test4.json");
//doc.jsonFromFile("C:/Users/PC/Documents/coursework/test5.json");
doc.jsonFromFile("C:/Users/PC/Documents/coursework/test6.json");
//doc.jsonFromFile("C:/Users/PC/Documents/coursework/test7.json");
//doc.jsonFromFile("C:/Users/PC/Documents/coursework/test8.json");
doc.jsonAnalyze();
 return 0;
}
