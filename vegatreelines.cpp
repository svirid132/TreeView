#include "vegatreelines.h"

#include <QPainter>

VegaTreeLines::VegaTreeLines()
{

}

void VegaTreeLines::paint(QPainter *painter)
{
    QPen pen = QPen(Qt::gray);
    pen.setWidth(2);
    pen.setStyle(Qt::DotLine);
    pen.setDashPattern({0.5, 3});
    painter->setPen(pen);
    for (int i = 0; i < m_lines.length(); ++i) {
        QPoint p1( m_lines[i].x, m_lines[i].y );
        QPoint p2( m_lines[i].x, m_lines[i].y + m_lines[i].height );
        painter->drawLine(p1, p2);
    }
}

void VegaTreeLines::setLines(const QList<Line> &lines)
{
    m_lines = lines;
    update();
}
