#include "UndoManager.h"

UndoManager::UndoManager(QObject* parent)
    : QObject(parent)
{
}

void UndoManager::saveState(const QImage& image) {
    m_storage.push(image);
    emit historyChanged();
}

bool UndoManager::canUndo() const {
    return m_storage.canUndo();
}

bool UndoManager::canRedo() const {
    return m_storage.canRedo();
}

QImage UndoManager::undo() {
    QImage result = m_storage.undo();
    emit historyChanged();
    return result;
}

QImage UndoManager::redo() {
    QImage result = m_storage.redo();
    emit historyChanged();
    return result;
}

void UndoManager::clear() {
    m_storage.clear();
    emit historyChanged();
}