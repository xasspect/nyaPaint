#include "BrushSizeWidget.h"
#include <QVBoxLayout>
#include <QPainter>

BrushSizeWidget::BrushSizeWidget(QWidget *parent)
    : QWidget(parent)
      , m_slider(nullptr)
      , m_spinBox(nullptr)
      , m_previewLabel(nullptr)
      , m_updating(false) {
    setupUI();
}

void BrushSizeWidget::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(4, 4, 4, 4);
    mainLayout->setSpacing(4);

    QLabel *title = new QLabel("Size", this);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-weight: bold; font-size: 12px;");
    mainLayout->addWidget(title);

    m_previewLabel = new QLabel(this);
    m_previewLabel->setAlignment(Qt::AlignCenter);
    m_previewLabel->setFixedSize(40, 40);
    mainLayout->addWidget(m_previewLabel, 0, Qt::AlignCenter);

    m_slider = new QSlider(Qt::Horizontal, this);
    m_slider->setRange(1, 40);
    m_slider->setValue(2);
    m_slider->setTickPosition(QSlider::TicksBelow);
    m_slider->setTickInterval(5);
    mainLayout->addWidget(m_slider);

    m_spinBox = new QSpinBox(this);
    m_spinBox->setRange(1, 40);
    m_spinBox->setValue(2);
    m_spinBox->setSuffix(" px");
    m_spinBox->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(m_spinBox, 0, Qt::AlignCenter);

    connect(m_slider, &QSlider::valueChanged, this, &BrushSizeWidget::onSliderChanged);
    connect(m_spinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &BrushSizeWidget::onSpinBoxChanged);


    mainLayout->addStretch();
    updatePreview(2);

    setFixedWidth(100);
}

void BrushSizeWidget::setSize(int size) {
    m_updating = true;
    m_slider->setValue(size);
    m_spinBox->setValue(size);
    m_updating = false;
    updatePreview(size);
}

int BrushSizeWidget::size() const {
    return m_slider->value();
}

void BrushSizeWidget::onSliderChanged(int value) {
    if (m_updating) return;
    m_updating = true;
    m_spinBox->setValue(value);
    m_updating = false;
    updatePreview(value);
    emit sizeChanged(value);
}

void BrushSizeWidget::onSpinBoxChanged(int value) {
    if (m_updating) return;
    m_updating = true;
    m_slider->setValue(value);
    m_updating = false;
    updatePreview(value);
    emit sizeChanged(value);
}

void BrushSizeWidget::updatePreview(int size) {
    int maxPreview = 36;
    int previewSize = qMin(size, maxPreview);

    QPixmap pixmap(40, 40);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(Qt::black);
    painter.setPen(Qt::NoPen);

    int centerX = 20;
    int centerY = 20;
    painter.drawEllipse(QPoint(centerX, centerY), previewSize / 2, previewSize / 2);
    painter.end();

    m_previewLabel->setPixmap(pixmap);
}
