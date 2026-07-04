#include "CanvasModel.h"
#include <QPainter>

CanvasModel::CanvasModel(const QSize &size, QObject *parent)
    : QObject(parent)
      , m_canvasSize(size)
      , m_undoManager(std::make_unique<UndoManager>(this)) {
    m_image = QImage(size, QImage::Format_ARGB32);
    m_image.fill(Qt::white);

    m_undoManager->setBaseState(m_image);
    connect(this, &CanvasModel::imageChangeCommitted,
            this, &CanvasModel::onImageChangeCommitted);
}

QImage &CanvasModel::image() {
    return m_image;
}

const QImage &CanvasModel::image() const {
    return m_image;
}

QSize CanvasModel::canvasSize() const {
    return m_canvasSize;
}

void CanvasModel::onImageChangeCommitted() {
    m_undoManager->saveState(m_image);
}

UndoManager *CanvasModel::undoManager() const {
    return m_undoManager.get();
}

void CanvasModel::undo() {
    if (!m_undoManager->canUndo()) return;
    m_image = m_undoManager->undo(m_image);
    emit imageChanged();
}


void CanvasModel::redo() {
    if (!m_undoManager->canRedo()) return;
    m_image = m_undoManager->redo(m_image);
    emit imageChanged();
}

void CanvasModel::clear() {
    m_image.fill(Qt::white);
    m_undoManager->saveState(m_image);
    emit imageChanged();
}

void CanvasModel::newCanvas(const QSize &size) {
    m_canvasSize = size;
    m_image = QImage(size, QImage::Format_ARGB32);
    m_image.fill(Qt::white);
    emit imageChanged();
}
