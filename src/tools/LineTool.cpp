#include "LineTool.h"
#include "../canvas/CanvasModel.h"
#include <QPainter>

LineTool::LineTool(QObject* parent)
    : Tool(parent)
    , m_lineStarted(false)
{
    m_color = Qt::black;
    m_size = 2;
}

QString LineTool::name() const {
    return "Line";
}

QCursor LineTool::cursor() const {
    QPixmap pixmap(16, 16);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setPen(Qt::black);
    painter.drawLine(2, 14, 14, 2);
    painter.end();

    return QCursor(pixmap, 0, 16);
}

bool LineTool::isDrawing() const {
    return m_lineStarted;
}

void LineTool::cancelLine() {
    m_lineStarted = false;
    m_backupImage = QImage();
    m_undoImage = QImage();
}

QPoint LineTool::snapToAngle(const QPoint& start, const QPoint& end) const {
    if (!(QApplication::keyboardModifiers() & Qt::ControlModifier)) {
        return end;
    }

    QPoint delta = end - start;
    int dx = qAbs(delta.x());
    int dy = qAbs(delta.y());

    if (dx > 2 * dy) {
        return QPoint(end.x(), start.y());
    } else if (dy > 2 * dx) {
        return QPoint(start.x(), end.y());
    } else {
        int length = qMax(dx, dy);
        int signX = (delta.x() >= 0) ? 1 : -1;
        int signY = (delta.y() >= 0) ? 1 : -1;
        return QPoint(start.x() + signX * length, start.y() + signY * length);
    }
}

void LineTool::mousePressEvent(QMouseEvent* event, CanvasModel* model) {
    if (event->button() != Qt::LeftButton) return;
    if (!model) return;

    m_lineStarted = true;
    m_startPoint = event->pos();
    m_currentPoint = event->pos();

    m_backupImage = model->image().copy();
    m_undoImage = model->image().copy();
}

void LineTool::mouseMoveEvent(QMouseEvent* event, CanvasModel* model) {
    if (!m_lineStarted) return;
    if (!model) return;
    if (m_backupImage.isNull()) return;

    m_currentPoint = snapToAngle(m_startPoint, event->pos());

    model->image() = m_backupImage.copy();

    QPainter painter(&model->image());
    painter.setRenderHint(QPainter::Antialiasing, m_size > 1);
    painter.setPen(QPen(m_color, m_size, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawLine(m_startPoint, m_currentPoint);
    painter.end();

    emit model->imageChanged();
}

void LineTool::mouseReleaseEvent(QMouseEvent* event, CanvasModel* model) {
    if (event->button() != Qt::LeftButton) return;
    if (!m_lineStarted) return;
    if (!model) return;

    QPoint endPoint = snapToAngle(m_startPoint, event->pos());

    model->image() = m_undoImage.copy();

    QPainter painter(&model->image());
    painter.setRenderHint(QPainter::Antialiasing, m_size > 1);
    painter.setPen(QPen(m_color, m_size, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawLine(m_startPoint, endPoint);
    painter.end();

    m_lineStarted = false;
    m_backupImage = QImage();
    m_undoImage = QImage();

    emit model->imageChanged();
    emit model->imageChangeCommitted();
}