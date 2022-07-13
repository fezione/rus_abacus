#include "myview.h"
#include <QMouseEvent>
#include <QTimeLine>
#include <QGraphicsItemAnimation>

myView::myView(QWidget *parent):
    QGraphicsView(parent), scene()
{
    setScene( &scene );

    scene.setSceneRect( -50, -50, 200, 200 );

    setSceneRect( -50,-50, 100, 100 );

    createAbacus();
}

myView::~myView()
{
    for ( auto it = 0; it < itemLst.size(); ++it ) {
        for ( auto elem = 0; elem < itemLst.at(it).size(); ++elem ) {
            delete itemLst.at(it).at(elem);
        }
    }

    itemLst.clear();
}

void myView::setValue(const QVector<int> &val)
{
    setBasePositionChips();

    int lineStep = 1;

    for ( int q = 0; q < val.size(); ++q ) {
        if ( val.at(q) ) {
            slt_setInputNumber( lineStep, val.at(q) );
        }
        ++lineStep;
    }
}

void myView::mousePressEvent(QMouseEvent *event)
{
    //setBasePositionChips();
}

void myView::onAnimationFinished()
{
    sender()->deleteLater();
}

void myView::createAbacus()
{
    int xmin{xminScene}, ymin{yminScene}, xmax{120}, ymax{180};

    QPolygonF poly;
        poly << QPointF(xmin,ymin)
             << QPointF(xmax,ymin)
             << QPointF(xmax,ymax)
             << QPointF(xmin,ymax);
    scene.addPolygon( poly );

    int nmbItem = 1;

    for ( int q = 1; q <= 8; ++q ) {

        double newy = ((ymax*2)/9.)*q;
        scene.addLine( xmin, ymin+newy, xmax, ymin+newy, QPen(Qt::magenta) );

        newy = ymin + (((ymax*2)/9.)*q)-18;
        QList<myItem*> tmpLst;
        if ( q != 6 ) {
            for ( int t = 1; t <= 10; ++t ) {
                double corX = xmax - deltaCorX*t;
                QColor color(222,184,135);

                if (t == 5) {
                    color = Qt::black;
                }

                if (t == 6) {
                    color = Qt::black;
                }

                myItem *item = new myItem( corX, newy, nmbItem, color );
                item->setPos( corX, newy );
                scene.addItem( item );
                tmpLst << item;
                ++nmbItem;
            }
        }else{
            for ( int t = 1; t <= 4; ++t ) {
                double corX = xmax - deltaCorX*t;

                QColor color(222,184,135);

                if (t == 2) {
                    color = Qt::black;
                }

                if (t == 3) {
                    color = Qt::black;
                }

                myItem *item = new myItem( corX, newy, nmbItem, color );
                item->setPos( corX, newy );
                scene.addItem( item );
                tmpLst << item;
                ++nmbItem;
            }
        }
        itemLst << tmpLst;
    }
}

void myView::setBasePositionChips()
{
    for ( auto q = 0; q < itemLst.size(); ++q ) {
        for ( auto t = 0; t < itemLst.at(q).size(); ++t ) {
            QPointF corBase(itemLst.at(q).at(t)->getCorX(), itemLst.at(q).at(t)->getCorY() );
            itemLst.at(q).at(t)->setPos( corBase );
        }
    }
}

void myView::slt_moveToPos()
{
    QPointF eventPos;
    static int q = 0;

    if ( q ) {
        eventPos.setX(itemLst.last().first()->getCorX());
        eventPos.setY(itemLst.last().first()->getCorY());
    }else{
        eventPos.setX(xminScene);
        eventPos.setY(yminScene);
        ++q;
    }

    QTimeLine *timer = new QTimeLine( speedAnimation, this );
    QGraphicsItemAnimation *animation = new QGraphicsItemAnimation(timer);
    connect( timer, SIGNAL(finished()), SLOT(onAnimationFinished()));
    animation->setItem( itemLst.last().first() );
    animation->setTimeLine( timer );
    animation->setPosAt( 1.0, eventPos );
    timer->start();
}

void myView::slt_setInputNumber(const int &nmbLine, const int &nmbChip)
{
    int count = nmbChip;
    int curChip = itemLst.at(nmbLine-1).size()-1;

    int xCor{xminScene}, yCor{itemLst.at(nmbLine-1).first()->y()};

    while ( count ) {

        QPointF eventPos;

        eventPos.setX(xCor);
        eventPos.setY(yCor);

        QTimeLine *timer = new QTimeLine( speedAnimation, this );
        QGraphicsItemAnimation *animation = new QGraphicsItemAnimation(timer);
        connect( timer, SIGNAL(finished()), SLOT(onAnimationFinished()));
        animation->setItem( itemLst.at(nmbLine-1).at(curChip) );
        animation->setTimeLine( timer );
        animation->setPosAt( 1.0, eventPos );
        timer->start();

        xCor += deltaCorX;
        --count;
        --curChip;
    }

}
