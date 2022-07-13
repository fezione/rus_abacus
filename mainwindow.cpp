#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setGeometry( 10, 30, 600, 600 );
    this->setFont( QFont("Helvetica") );
    this->setWindowTitle( "R_Abacus" );

/*--Elem--*/
    QLabel* lab_1ch = new QLabel("Введите первое число", this );
    QLabel* lab_2ch = new QLabel("Введите второе число", this );
    QLabel* lab_oper = new QLabel("Операция", this );
    QLabel* lab_res = new QLabel("Результат", this );

    le_1ch = new QLineEdit(this);
    le_2ch = new QLineEdit(this);
    le_res = new QLineEdit(this);
    le_1ch->setInputMask("99999.99");
    le_2ch->setInputMask("99999.99");
    le_res->setReadOnly( true );

    QPushButton *but_show1ch = new QPushButton("Отобразить",this);
    connect( but_show1ch, SIGNAL(clicked(bool)), SLOT(slt_show1ch()));
    QPushButton *but_show2ch = new QPushButton("Отобразить",this);
    connect( but_show2ch, SIGNAL(clicked(bool)), SLOT(slt_show2ch()));

    QPushButton *but_calc = new QPushButton("Вычислить",this);
    connect( but_calc, SIGNAL(clicked(bool)), SLOT(slt_calcStart()));

   cb_oper = new QComboBox(this);
        cb_oper->addItem("+");
        cb_oper->addItem("-");
        cb_oper->addItem("*");

    QGridLayout *lay_elem = new QGridLayout(this);
        lay_elem->addWidget( lab_1ch, 0, 0 );
        lay_elem->addWidget( le_1ch, 0, 1 );
        lay_elem->addWidget( but_show1ch, 0, 2 );
        lay_elem->addWidget( lab_2ch, 1, 0 );
        lay_elem->addWidget( le_2ch, 1, 1 );
        lay_elem->addWidget( but_show2ch, 1, 2 );
        lay_elem->addWidget( lab_oper, 2, 0 );
        lay_elem->addWidget( cb_oper, 2, 1 );
        lay_elem->addWidget( but_calc, 3, 0, 1, 2);
        lay_elem->addWidget( lab_res, 4, 0 );
        lay_elem->addWidget( le_res, 4, 1 );
/*---*/

/*--View--*/
    view = new myView(this);
    view->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Preferred );

    QHBoxLayout *lay_view = new QHBoxLayout(this);
        lay_view->addWidget( view );
/*---*/

    QVBoxLayout *lay_unity = new QVBoxLayout( this );
        lay_unity->addLayout( lay_view );
        lay_unity->addLayout( lay_elem );

    ui->centralWidget->setLayout( lay_unity );
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::getValueByArifmOperation(const double &val1, const double &val2)
{
    QString result;
    double val{0.0};
    switch ( cb_oper->currentIndex() ) {
    case 0:
        val = val1+val2;
        break;
    case 1:
        val = val1-val2;
        break;
    case 2:
        val = val1*val2;
        break;
    default:
        break;
    }

    if ( val >= 100000 || val < 0.01 ) {
        throw 1;
    }

    result = QString::number( val,'g',8);

    return result;
}

void MainWindow::setValueToView(const QString &value)
{
    QVector<int> prVec;
    QVector<int> afterVec;
    QVector<int> resVec;

    bool f = true;
    int countRankPr = 0, countRankAf = 0;

    for ( int q = 0; q < value.size(); ++q ) {
        if ( value.at(q) == '.' ) {
            f = false;
        }else{
            if (f) {
                prVec << value.at(q).unicode() - 48;
                ++countRankPr;
            }else{
                if ( !countRankAf ) {
                    bool flCh = false;
                    if ( q + 1 < value.size() ) {
                        QString temp;
                        temp.append( value.at(q));
                        temp.append(value.at(q+1));
                        int nmb = temp.toInt();
                        if ( !(nmb%25) ) {
                            afterVec << nmb/25;
                            ++countRankAf;
                            flCh = true;
                            q = value.size();
                        }
                    }else{
                        if ( (value.at(q).unicode() - 48) != 0 ) {
                             if ( !((value.at(q).unicode() - 48)%5) ) {
                                  afterVec << 2;
                                  ++countRankAf;
                                  flCh = true;
                                  q = value.size();
                             }
                        }
                    }

                    if (!flCh) {
                        afterVec << 0;
                        ++countRankAf;
                        afterVec << value.at(q).unicode() - 48;
                        ++countRankAf;
                    }

                }else{
                    afterVec << value.at(q).unicode() - 48;
                    ++countRankAf;
                }
            }
        }
    }

    switch (countRankPr) {
    case 1:
        resVec << 0 << 0 << 0 << 0 << prVec;
        break;
    case 2:
        resVec << 0 << 0 << 0 << prVec;
        break;
    case 3:
        resVec << 0 << 0 << prVec;
        break;
    case 4:
        resVec << 0 << prVec;
        break;
    case 5:
        resVec << prVec;
        break;
    default:
        break;
    }

    switch (countRankAf) {
    case 1:
        resVec << afterVec << 0 << 0 << 0;
        break;
    case 2:
        resVec << afterVec << 0 << 0;
        break;
    case 3:
        resVec << afterVec << 0;
        break;
    case 4:
        resVec << afterVec;
        break;
    default:
        break;
    }

    view->setValue( resVec );
}

void MainWindow::slt_calcStart()
{
    le_res->setText(QString('0'));

    if ( le_1ch->text().toDouble() != 0.0 && le_2ch->text().toDouble() != 0.0 ) {

        double val1 = le_1ch->text().toDouble();
        double val2 = le_2ch->text().toDouble();

        QString s1;

        try {

            s1 = getValueByArifmOperation(val1,val2);

            setValueToView( s1 );

            le_res->setText( s1 );

        } catch (...) {
            QMessageBox msg;
            msg.setText("Can't do this operation by current values.");
            msg.exec();
        }

    }else{
        QMessageBox msg;
        msg.setText("Incorrect input value(s).");
        msg.exec();
    }
}

void MainWindow::slt_show1ch()
{
    if ( le_1ch->text().toDouble() != 0.0 ) {
        setValueToView( le_1ch->text() );
    }
}

void MainWindow::slt_show2ch()
{
    if ( le_2ch->text().toDouble() != 0.0 ) {
        setValueToView( le_2ch->text() );
    }
}
