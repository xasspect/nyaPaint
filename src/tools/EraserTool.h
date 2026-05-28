#pragma once
#include "Tool.h"

class EraserTool : public Tool {
    Q_OBJECT

public:
    explicit EraserTool(QObject *parent = nullptr);

    QString name() const override;

    QCursor cursor() const override;


    void mouseMoveEvent(QMouseEvent *event, CanvasModel *model) override;

    void mousePressEvent(QMouseEvent *event, CanvasModel *model) override;

    void mouseReleaseEvent(QMouseEvent *event, CanvasModel *model) override;
};
