#include "mainwindow.h"

#include <QApplication>
#include "db/DatabaseManager.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    DatabaseManager::instance().open("family_budget.db");
    DatabaseManager::instance().init();

    MainWindow w;
    w.show();
    return a.exec();
}
