#include "Tool.h"
#include "canvas/CanvasModel.h"
#include "undo/UndoManager.h"

Tool::Tool(QObject *parent)
    : QObject(parent)
      , m_color(Qt::black)
      , m_size(2)
      , m_drawing(false) {
}


void Tool::mousePressEvent(QMouseEvent* event, CanvasModel* model) {

}

void Tool::mouseMoveEvent(QMouseEvent* event, CanvasModel* model) {

}

void Tool::mouseReleaseEvent(QMouseEvent* event, CanvasModel* model) {

}

void Tool::setColor(const QColor &color) {
    m_color = color;
}

QColor Tool::color() const {
    return m_color;
}

void Tool::setSize(int size) {
    m_size = qMax(1, size);
}

int Tool::size() const {
    return m_size;
}

// void Tool::saveUndoState(CanvasModel *model) {
//     if (!model) return;
//     model->undoManager()->saveState(model->image());
// }
