#ifndef MYVIEW_H
#define MYVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include "myitem.h"

class myView : public QGraphicsView {
    Q_OBJECT
public:
    explicit myView( QWidget *parent = 0);
    ~myView();

    /*
        Выставляет значение на поле
        QVector<int> &val - вектор линий, на которых расположены костяшки
        исходя из значений выставляет нужно количество костяшек
    */
    void setValue(const QVector<int> &val );

protected slots:
    void mousePressEvent( QMouseEvent *event );
    void onAnimationFinished();

private:
    void createAbacus();        ///< создание поля с костяшками

    void setBasePositionChips();    ///< выставка базовых позиций костяшек

    const int speedAnimation = 1000;
    const int sizeItem = 10;

    const int deltaCorX = 15;   ///< расстояние между костяшками

    const int xminScene = -270; ///< граница поля с костяшками
    const int yminScene = -180; ///< -//-

    QGraphicsScene scene;

    QList<QList<myItem*>> itemLst;

private slots:
    void slt_moveToPos();

    void slt_setInputNumber( const int &nmbLine, const int &nmbChip );
};

#endif // MYVIEW_H
