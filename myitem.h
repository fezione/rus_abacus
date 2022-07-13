#ifndef MYITEM_H
#define MYITEM_H

#include <QGraphicsItem>
#include <QBrush>
#include <QtGui>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>

class myItem : public QGraphicsItem
{
public:
    myItem(const double &x, const double &y, const int &_nmb,
            const QColor &_color );
    ~myItem() {}

    QRectF boundingRect() const override;
    void paint( QPainter *painter,
                const QStyleOptionGraphicsItem *option,
                QWidget *widget ) override;

    void doCollusion();

    double getCorX() const;

    double getCorY() const;

protected:
    void mousePressEvent( QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent( QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent( QGraphicsSceneMouseEvent *event);

private:
    enum {
        Type = 99
    };

    int type() const {
        return Type;
    }

    bool sgn_move = false;

    int height{30}, width{15}, nmb{0};

    double corX{0.0}, corY{0.0};

    QColor color;

    QMap<int, int> granica;

    int minX{-270};
    QVector<int> vecgranica;
};

#endif // MYITEM_H
