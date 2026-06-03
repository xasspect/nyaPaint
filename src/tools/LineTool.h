#pragma once
#include "Tool.h"

class LineTool : public Tool {
    Q_OBJECT

public:
    explicit LineTool(QObject *parent = nullptr);

    QString name() const override;

    QCursor cursor() const override;

    void mousePressEvent(QMouseEvent *event, CanvasModel *model) override;

    void mouseMoveEvent(QMouseEvent *event, CanvasModel *model) override;

    void mouseReleaseEvent(QMouseEvent *event, CanvasModel *model) override;

    void cancelLine();

    bool isDrawing() const;

private:
    QPoint m_startPoint;
    QPoint m_currentPoint;
    bool m_lineStarted;
    QImage m_backupImage;
    QImage m_undoImage;

    QPoint snapToAngle(const QPoint &start, const QPoint &end) const;

};
