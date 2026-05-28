#include "Canvas.h"
#include <QPainter>
#include <QPaintEvent>

Canvas::Canvas(QWidget *parent)
    : QWidget(parent)
      , m_model(nullptr)
      , m_currentTool(nullptr) {
    setAutoFillBackground(true);
    QPalette pal = this->palette();
    pal.setColor(QPalette::Window, Qt::white);
    setPalette(pal);

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setMinimumSize(QSize(100, 100));

    setMouseTracking(true);
}

void Canvas::setModel(CanvasModel *model) {
    if (m_model) {
        disconnect(m_model,
                   &CanvasModel::imageChanged,
                   this,
                   &Canvas::onImageChanged);
    }

    m_model = model;

    if (m_model) {
        connect(m_model,
                &CanvasModel::imageChanged,
                this,
                &Canvas::onImageChanged);
        onImageChanged();
    }
}

CanvasModel *Canvas::model() const {
    return m_model;
}

void Canvas::paintEvent(QPaintEvent *event) {
    QPainter painter(this);

    if (!m_displayImage.isNull()) {
        int x = (width() - m_displayImage.width()) / 2;
        int y = (height() - m_displayImage.height()) / 2;
        painter.drawImage(x, y, m_displayImage);
    }
}


void Canvas::onImageChanged() {
    if (!m_model) return;

    m_displayImage = m_model->image();

    update();
}

void Canvas::setTool(Tool *tool) {
    m_currentTool = tool;
    if (m_currentTool) {
        setCursor(m_currentTool->cursor());
    } else {
        setCursor(Qt::ArrowCursor);
    }
}

Tool *Canvas::tool() const {
    return m_currentTool;
}

void Canvas::mousePressEvent(QMouseEvent *event) {
    if (m_currentTool && m_model) {
        QPointF imagePos = widgetToImage(event->pos());

        QMouseEvent imageEvent(
            event->type(),
            imagePos,
            event->globalPosition(),
            event->button(),
            event->buttons(),
            event->modifiers()
        );

        m_currentTool->mousePressEvent(&imageEvent, m_model);
    }
    QWidget::mousePressEvent(event);
}

void Canvas::mouseMoveEvent(QMouseEvent *event) {
    QPointF imagePos = widgetToImage(event->pos());
    emit mouseMoved(imagePos.toPoint());

    if (m_currentTool && m_model) {
        QMouseEvent imageEvent(
            event->type(),
            imagePos,
            event->globalPosition(),
            event->button(),
            event->buttons(),
            event->modifiers()
        );
        m_currentTool->mouseMoveEvent(&imageEvent, m_model);
    }
    QWidget::mouseMoveEvent(event);
}

void Canvas::mouseReleaseEvent(QMouseEvent *event) {
    if (m_currentTool && m_model) {
        QPointF imagePos = widgetToImage(event->pos());

        QMouseEvent imageEvent(
            event->type(),
            imagePos,
            event->globalPosition(),
            event->button(),
            event->buttons(),
            event->modifiers()
        );

        m_currentTool->mouseReleaseEvent(&imageEvent, m_model);
    }
    QWidget::mouseReleaseEvent(event);
}

QPointF Canvas::widgetToImage(const QPointF &widgetPos) const {
    int offsetX = (width() - m_displayImage.width()) / 2;
    int offsetY = (height() - m_displayImage.height()) / 2;

    return QPointF(widgetPos.x() - offsetX, widgetPos.y() - offsetY);
}
