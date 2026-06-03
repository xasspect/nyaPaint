#pragma once
#include <QObject>
#include <QMouseEvent>
#include <QCursor>
#include <QColor>

class CanvasModel;

class Tool : public QObject {
    Q_OBJECT

public:
    explicit Tool(QObject *parent = nullptr);

    virtual ~Tool() = default;

    virtual QString name() const = 0;

    virtual QCursor cursor() const = 0;

    virtual void mousePressEvent(QMouseEvent *event, CanvasModel *model);

    virtual void mouseMoveEvent(QMouseEvent *event, CanvasModel *model);

    virtual void mouseReleaseEvent(QMouseEvent *event, CanvasModel *model);

    virtual bool isDrawing() const { return false;}

    void setColor(const QColor &color);

    QColor color() const;

    void setSize(int size);

    int size() const;

    virtual void cancelLine() {}

protected:
    QColor m_color;
    int m_size;
    QPoint m_lastPoint;
    bool m_drawing;

    // void saveUndoState(CanvasModel *model);
};
