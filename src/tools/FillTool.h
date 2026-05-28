#pragma once
#include "Tool.h"
#include <QStack>

class FillTool : public Tool {
    Q_OBJECT

public:
    explicit FillTool(QObject *parent = nullptr);

    QString name() const override;

    QCursor cursor() const override;

    void mousePressEvent(QMouseEvent *event, CanvasModel *model) override;

    void mouseMoveEvent(QMouseEvent *event, CanvasModel *model) override;

    void mouseReleaseEvent(QMouseEvent *event, CanvasModel *model) override;

private:
    void floodFill(QImage &image, const QPoint &start, const QColor &fillColor);
};
