#pragma once
#include <QObject>
#include <QImage>
#include <QSize>
#include <QColor>
#include <memory>
#include "undo/UndoManager.h"

class CanvasModel : public QObject {
    Q_OBJECT

public:
    explicit CanvasModel(const QSize &size, QObject *parent = nullptr);

    QImage &image();

    const QImage &image() const;

    QSize canvasSize() const;

    void onImageChangeCommitted();

    void clear();

    void newCanvas(const QSize &size);

    UndoManager *undoManager() const;

    void undo();

    void redo();

signals:
    void imageChanged();
    void imageChangeCommitted();
    // void onImageChangeCommitted();

private:
    QImage m_image;
    QSize m_canvasSize;
    std::unique_ptr<UndoManager> m_undoManager;
};
