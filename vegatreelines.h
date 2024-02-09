#ifndef VEGATREELINES_H
#define VEGATREELINES_H

#include <QQuickPaintedItem>

class VegaTreeLines : public QQuickPaintedItem
{
    Q_OBJECT
public:
    struct Line {
        Line() { }
        int x; int y;
        int height;
    };

    VegaTreeLines();

signals:

    // QQuickPaintedItem interface
public:
    void paint(QPainter *painter) override;
    void setLines(const QList<Line> &lines);

private:
    QList<Line> m_lines;
};

#endif // VEGATREELINES_H
