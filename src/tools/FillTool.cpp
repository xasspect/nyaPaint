#include "FillTool.h"
#include "../canvas/CanvasModel.h"
#include <QPainter>
#include <QDebug>

FillTool::FillTool(QObject *parent)
    : Tool(parent) {
    m_color = Qt::black;
}

QString FillTool::name() const {
    return "Fill";
}

QCursor FillTool::cursor() const {
    QPixmap pixmap(18, 18);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setPen(Qt::black);
    painter.setBrush(m_color);

    painter.drawRect(5, 10, 10, 6);
    painter.drawLine(7, 10, 4, 2);
    painter.drawLine(13, 10, 16, 2);
    painter.drawLine(4, 2, 16, 2);
    painter.setPen(Qt::NoPen);
    painter.setBrush(m_color);
    painter.drawRect(8, 15, 4, 2);

    painter.end();

    return QCursor(pixmap, 10, 10);
}

void FillTool::mousePressEvent(QMouseEvent *event, CanvasModel *model) {
    if (event->button() != Qt::LeftButton) return;

    if (!model) return;

    QPoint pos = event->pos();
    QImage &image = model->image();

    if (!image.rect().contains(pos)) return;

    // saveUndoState(model);

    floodFill(image, pos, m_color);

    emit model->imageChanged();
    emit model->imageChangeCommitted();
}


void FillTool::mouseMoveEvent(QMouseEvent *event, CanvasModel *model) {
}

void FillTool::mouseReleaseEvent(QMouseEvent *event, CanvasModel *model) {
}

void FillTool::floodFill(QImage &image, const QPoint &start, const QColor &fillColor) {
    int width = image.width();
    int height = image.height();

    if (start.x() < 0 || start.x() >= width || start.y() < 0 || start.y() >= height) {
        return;
    }

    QColor targetColor = image.pixelColor(start);

    if (targetColor.rgb() == fillColor.rgb()) return;

    QVector<bool> visited(width * height, false);

    QStack<QPoint> stack;
    stack.push(start);

    auto isValid = [width, height](const QPoint &p) -> bool {
        return p.x() >= 0 && p.x() < width && p.y() >= 0 && p.y() < height;
    };

    while (!stack.isEmpty()) {
        QPoint p = stack.pop();

        if (!isValid(p)) continue;

        int idx = p.y() * width + p.x();
        if (idx < 0 || idx >= visited.size()) continue;
        if (visited[idx]) continue;
        visited[idx] = true;

        if (image.pixelColor(p).rgb() != targetColor.rgb()) continue;

        image.setPixelColor(p, fillColor);

        QPoint neighbors[4] = {
            QPoint(p.x() + 1, p.y()),
            QPoint(p.x() - 1, p.y()),
            QPoint(p.x(), p.y() + 1),
            QPoint(p.x(), p.y() - 1)
        };

        for (const QPoint &n: neighbors) {
            if (isValid(n) && !visited[n.y() * width + n.x()]) {
                stack.push(n);
            }
        }
    }
}
