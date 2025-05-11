#pragma once

#include <QWidget>

namespace Ui {
class IncomeWidget;
}

class IncomeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit IncomeWidget(QWidget *parent = nullptr);
    ~IncomeWidget();

    void loadIncomeRecords();
public slots:
    void onUpdate();

private slots:
    void onAddIncomeClicked();

private:
    Ui::IncomeWidget *ui;
    void setupTable();
    void loadUsers();
};

