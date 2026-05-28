#include "UndoManager.h"

UndoManager::UndoManager(QObject *parent)
    : QObject(parent) {
}

void UndoManager::saveState(const QImage &image) {
    m_undoStack.push(image.copy());

    m_redoStack.clear();

    while (m_undoStack.size() > MAX_HISTORY) {
        m_undoStack.removeFirst();
    }

    emit historyChanged();
}

bool UndoManager::canUndo() const {
    return m_undoStack.size() >= 2;  // Минимум 2 для отмены
}

bool UndoManager::canRedo() const {
    return !m_redoStack.isEmpty();
}

QImage UndoManager::undo() {
    if (m_undoStack.isEmpty()) {
        return QImage();
    }

    QImage current = m_undoStack.pop();
    m_redoStack.push(current);

    emit historyChanged();

    if (m_undoStack.isEmpty()) {
        return current;
    }
    return m_undoStack.top();
}


QImage UndoManager::redo() {
    if (m_redoStack.isEmpty()) {
        return QImage();
    }

    QImage image = m_redoStack.pop();
    m_undoStack.push(image);

    emit historyChanged();

    return image;
}

void UndoManager::clear() {
    m_undoStack.clear();
    m_redoStack.clear();
    emit historyChanged();
}
