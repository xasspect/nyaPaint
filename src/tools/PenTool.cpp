#include "PenTool.h"
#include "../canvas/CanvasModel.h"
#include <QPainter>

PenTool::PenTool(QObject *parent)
    : Tool(parent) {
    m_color = Qt::black;
    m_size = 2;
}

QString PenTool::name() const {
    return "Pen";
}

QCursor PenTool::cursor() const {
    QPixmap pixmap(16, 16);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setPen(Qt::black);
    painter.drawLine(8, 0, 8, 16);
    painter.drawLine(0, 8, 16, 8);
    painter.drawPoint(8, 8);
    painter.end();

    return QCursor(pixmap, 8, 8);
}

void PenTool::mousePressEvent(QMouseEvent *event, CanvasModel *model) {
    if (event->button() != Qt::LeftButton) return;
    if (!model) return;

    // saveUndoState(model);

    m_drawing = true;
    m_lastPoint = event->pos();

    QImage &image = model->image();
    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing, m_size > 1);

    painter.setPen(QPen(m_color, m_size, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawPoint(m_lastPoint);
    painter.end();

    emit model->imageChanged();
    // emit model->imageChangeCommitted();
}

void PenTool::mouseMoveEvent(QMouseEvent *event, CanvasModel *model) {
    if (!m_drawing) return;
    if (!(event->buttons() & Qt::LeftButton)) return;
    if (!model) return;

    QPoint currentPoint = event->pos();

    QImage &image = model->image();
    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing, m_size > 1);

    painter.setPen(QPen(m_color, m_size, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawLine(m_lastPoint, currentPoint);
    painter.end();

    m_lastPoint = currentPoint;

    emit model->imageChanged();
}

void PenTool::mouseReleaseEvent(QMouseEvent *event, CanvasModel *model) {
    if (event->button() != Qt::LeftButton) return;
    m_drawing = false;
    if (model) emit model->imageChangeCommitted();
}
