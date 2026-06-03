#pragma once
#include <QObject>
#include <QImage>
#include "UndoStack.h"
#include "UndoList.h"


#define UNDO_LIST


class UndoManager : public QObject {
    Q_OBJECT

public:
    explicit UndoManager(QObject* parent = nullptr);

    void saveState(const QImage& image);
    bool canUndo() const;
    bool canRedo() const;
    QImage undo();
    QImage redo();
    void clear();

    signals:
        void historyChanged();

private:
#ifdef UNDO_LIST
    UndoList m_storage;   //FIFO
#else
    UndoStack m_storage;  //LIFO (по умолчанию)
#endif
};