#include "myitem.h"

myItem::myItem(const double &x, const double &y,
               const int &_nmb, const QColor &_color)
{
    corX = x;
    corY = y;
    nmb = _nmb;
    color = _color;

    setToolTip( QString("Chips_" + QString::number(nmb)) );
    setCursor( Qt::OpenHandCursor );
    setAcceptedMouseButtons( Qt::LeftButton );
    setFlags( ItemIsSelectable | ItemIsMovable );
    setAcceptHoverEvents( true );
}

QRectF myItem::boundingRect() const
{
    return QRectF( 1, 1, width, height );
}

void myItem::paint(QPainter *painter,
                   const QStyleOptionGraphicsItem *option,
                   QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    if ( sgn_move ) {
        painter->setBrush( Qt::green );
    }else{
        painter->setBrush(color);
    }

    if ( !scene()->collidingItems(this).empty() ) {
        QList<QGraphicsItem*> lst_items = scene()->collidingItems(this);

        for ( int q = 0; q < lst_items.size(); ++q ) {
            if ( lst_items.at(q)->type() == 99 ) {
                painter->setBrush( Qt::magenta );
                ///< doCollusion();
                q = lst_items.size();
            }
        }
    }

    painter->drawEllipse( 0, 0, width, height );
    update();
}

void myItem::doCollusion()
{
    setPos( corX, corY );
}

void myItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    setCursor( Qt::DragMoveCursor );

    QGraphicsItem::mousePressEvent( event );

    update();
}

void myItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    sgn_move = true;
    int newX = event->scenePos().x();

    if ( newX > minX && newX < corX ) {
        setPos( newX, corY );
    }

    if ( granica.find(newX) == granica.end() ) {
        if ( granica.isEmpty() ) {
            granica[0] = newX;
        }else{
            int key = granica.lastKey();
            granica.insert( ++key, newX );
        }
    }

//    if ( !(nmb%10) ) {
//        minX = -270;
//    }else{
//        if ( newX > -270 ) {
//            vecgranica.append( newX );
//            qSort( vecgranica.begin(), vecgranica.end() );
//            minX = vecgranica.first();
//        }
//    }
}

void myItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);

    sgn_move = false;

    setCursor( Qt::OpenHandCursor );
}

double myItem::getCorY() const
{
    return corY;
}

double myItem::getCorX() const
{
    return corX;
}
