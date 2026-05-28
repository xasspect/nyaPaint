#pragma once
#include <QObject>
#include <QStack>
#include <QImage>

class UndoManager : public QObject {
    Q_OBJECT

public:
    explicit UndoManager(QObject *parent = nullptr);


    void saveState(const QImage &image);

    bool canUndo() const;

    bool canRedo() const;

    QImage undo();

    QImage redo();

    void clear();

signals:
    void historyChanged();

private:
    QStack<QImage> m_undoStack;
    QStack<QImage> m_redoStack;
    static const int MAX_HISTORY = 50;
};
