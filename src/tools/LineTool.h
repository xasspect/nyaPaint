#pragma once
#include "Tool.h"
#include <QApplication>

class LineTool : public Tool {
    Q_OBJECT

public:
    explicit LineTool(QObject* parent = nullptr);

    QString name() const override;
    QCursor cursor() const override;

    void mousePressEvent(QMouseEvent* event, CanvasModel* model) override;
    void mouseMoveEvent(QMouseEvent* event, CanvasModel* model) override;
    void mouseReleaseEvent(QMouseEvent* event, CanvasModel* model) override;

    bool isDrawing() const override;
    void cancelLine() override;

private:
    QPoint snapToAngle(const QPoint& start, const QPoint& end) const;

    QPoint m_startPoint;
    QPoint m_currentPoint;
    bool m_lineStarted;
    QImage m_backupImage;   // Для предпросмотра
    QImage m_undoImage;     // Для Undo
};