#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include "myview.h"
#include <stdio.h>
#include <stdlib.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QString getValueByArifmOperation( const double &val1, const double &val2);

    void setValueToView( const QString &value );

    Ui::MainWindow *ui;

    QLineEdit *le_1ch;
    QLineEdit *le_2ch;
    QLineEdit *le_res;
    QComboBox *cb_oper;

    myView *view;

private slots:
    void slt_calcStart();

    void slt_show1ch();

    void slt_show2ch();
};

#endif // MAINWINDOW_H
