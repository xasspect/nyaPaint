#include "EraserTool.h"
#include "../canvas/CanvasModel.h"
#include <QPainter>

EraserTool::EraserTool(QObject *parent)
    : Tool(parent) {
    m_size = 20;
}

QString EraserTool::name() const {
    return "Eraser";
}

QCursor EraserTool::cursor() const {
    int cursorSize = qMin(m_size, 24);

    QPixmap pixmap(cursorSize, cursorSize);

    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);

    painter.setPen(QPen(Qt::black, 1));
    painter.drawRect(1, 1, cursorSize - 2, cursorSize - 2);
    painter.end();

    return QCursor(pixmap, cursorSize / 2, cursorSize / 2);
}


void EraserTool::mousePressEvent(QMouseEvent *event, CanvasModel *model) {
    if (event->button() != Qt::LeftButton) { return; }
    if (!model) return;

    // saveUndoState(model);

    m_drawing = true;
    m_lastPoint = event->pos();

    QImage &image = model->image();
    QPainter painter(&image);

    painter.setPen(QPen(Qt::white, m_size, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawPoint(m_lastPoint);
    painter.end();

    emit model->imageChanged();
    // emit model->imageChangeCommitted();
}

void EraserTool::mouseMoveEvent(QMouseEvent *event, CanvasModel *model) {
    if (!m_drawing) return;
    if (!(event->buttons() & Qt::LeftButton)) return;
    if (!model) return;

    QPoint currentPoint = event->pos();
    QImage &image = model->image();
    QPainter painter(&image);

    painter.setPen(QPen(Qt::white,
                        m_size,
                        Qt::SolidLine,
                        Qt::RoundCap,
                        Qt::RoundJoin));
    painter.drawLine(m_lastPoint, currentPoint);
    painter.end();
    m_lastPoint = currentPoint;

    emit model->imageChanged();
}

void EraserTool::mouseReleaseEvent(QMouseEvent *event, CanvasModel *model) {
    Q_UNUSED(model);
    if (event->button() != Qt::LeftButton) return;
    m_drawing = false;
    if(model) emit model->imageChangeCommitted();
}
