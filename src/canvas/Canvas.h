#pragma once
#include <QWidget>

#include "CanvasModel.h"
#include "../tools/Tool.h"

class Canvas : public QWidget {
    Q_OBJECT

public:
    explicit Canvas(QWidget *parent = nullptr);

    void setModel(CanvasModel *model);

    CanvasModel *model() const;

    void setTool(Tool *tool);

    Tool *tool() const;

signals:
    void mouseMoved(const QPoint &imagePos);

protected:
    void paintEvent(QPaintEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

private slots:
    void onImageChanged();

private:
    QPointF widgetToImage(const QPointF &widgetPos) const;


    CanvasModel *m_model;
    QImage m_displayImage;
    Tool *m_currentTool;
};
