#include "mainwindow.h"
#include <verify.h>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();
Verify doc;
//doc.jsonFromFile("C:/Users/PC/Documents/coursework/tree.json");
//doc.jsonFromFile("C:/Users/PC/Documents/coursework/test.json");
doc.jsonFromFile("C:/Users/PC/Documents/coursework/test2.json");
doc.jsonAnalyze();

 return 0;
}
