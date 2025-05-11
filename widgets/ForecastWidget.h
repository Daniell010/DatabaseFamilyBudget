#pragma once

#include <QWidget>

namespace Ui {
class ForecastWidget;
}

class ForecastWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ForecastWidget(QWidget *parent = nullptr);
    ~ForecastWidget();

private slots:
    void onAddForecastClicked();

private:
    Ui::ForecastWidget *ui;
    void setupTable();
    void loadForecastRecords();
};

