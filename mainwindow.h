#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "widgets/IncomeWidget.h"
#include "widgets/ExpenseWidget.h"
#include "widgets/ProductWidget.h"
#include "widgets/ForecastWidget.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    IncomeWidget* m_incomeWidget;
    ExpenseWidget* m_expenseWidget;
    ProductWidget* m_productWidget;
    ForecastWidget* m_forecastWidget;
signals:
    void sigUpdate();
};
#endif // MAINWINDOW_H
