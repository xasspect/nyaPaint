#pragma once
#include <QObject>
#include <QImage>
#include <QRect>
#include <QStack>


struct CanvasPatch {
    QRect rect;         // Область, где произошли изменения
    QImage oldPixels;   // Как эти пиксели выглядели ДО мазка кистью
    QImage newPixels;   // Как они выглядят ПОСЛЕ
};


class UndoManager : public QObject {
    Q_OBJECT

public:
    explicit UndoManager(QObject* parent = nullptr);
    void setBaseState(const QImage& image);
    void saveState(const QImage& image);
    bool canUndo() const;
    bool canRedo() const;
    QImage undo(QImage currentCanvas);
    QImage redo(QImage currentCanvas);
    void clear();

    signals:
        void historyChanged();
private:
    // Математический алгоритм поиска измененной области
    QRect calculateDiffBounds(const QImage& img1, const QImage& img2) const;

    QStack<CanvasPatch> m_undoStack;
    QStack<CanvasPatch> m_redoStack;

    // Храним последнее известное состояние для сравнения с новым
    QImage m_lastState;
};