#include "ColorPickerWidget.h"
#include <QFrame>
#include <QHBoxLayout>

ColorPickerWidget::ColorPickerWidget(QWidget *parent)
    : QWidget(parent)
      , m_primaryColor(Qt::black)
      , m_secondaryColor(Qt::white)
      , m_primaryColorBtn(nullptr)
      , m_secondaryColorBtn(nullptr) {
    setupUI();
}

void ColorPickerWidget::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(4, 4, 4, 4);
    mainLayout->setSpacing(4);

    QLabel *title = new QLabel("Colors", this);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-weight: bold; font-size: 12px;");
    mainLayout->addWidget(title);

    QHBoxLayout *currentColorLayout = new QHBoxLayout();
    currentColorLayout->setSpacing(2);
    currentColorLayout->setAlignment(Qt::AlignCenter);

    m_primaryColorBtn = new QPushButton(this);
    m_primaryColorBtn->setFixedSize(28, 28);
    m_primaryColorBtn->setCursor(Qt::PointingHandCursor);
    connect(m_primaryColorBtn,
            &QPushButton::clicked,
            this,
            &ColorPickerWidget::onPrimaryColorClicked);

    m_secondaryColorBtn = new QPushButton(this);
    m_secondaryColorBtn->setFixedSize(28, 28);
    m_secondaryColorBtn->setCursor(Qt::PointingHandCursor);
    connect(m_secondaryColorBtn, &QPushButton::clicked, this, &ColorPickerWidget::onSecondaryColorClicked);

    currentColorLayout->addWidget(m_secondaryColorBtn);
    currentColorLayout->addWidget(m_primaryColorBtn);
    mainLayout->addLayout(currentColorLayout);

    updateColorDisplay();

    m_gridLayout = new QGridLayout();
    m_gridLayout->setSpacing(1);
    m_gridLayout->setAlignment(Qt::AlignCenter);
    mainLayout->addLayout(m_gridLayout);

    int columns = 4;
    for (int i = 0; i < m_defaultColors.size(); i++) { // цикл создания кнопко выбора цвета на виджете
        QPushButton *button = createColorButton(m_defaultColors[i]);
        int row = i / columns;
        int col = i % columns;
        m_gridLayout->addWidget(button, row, col);
    }

    mainLayout->addStretch();

    setFixedWidth(130);
}


QPushButton *ColorPickerWidget::createColorButton(const QColor &color) {
    QPushButton *button = new QPushButton(this);
    button->setFixedSize(24, 24);
    button->setCursor(Qt::PointingHandCursor);

    button->setStyleSheet(
        QString(
            "QPushButton {"
            "  background-color: %1;"
            "  border: 1px solid #999;"
            "  border-radius: 2px;"
            "}"
            "QPushButton:hover {"
            "  border: 2px solid #555;"
            "}"
        ).arg(color.name())
    );

    button->setProperty("color", color);

    connect(button, &QPushButton::clicked, this,
            &ColorPickerWidget::onColorButtonClicked);

    return button;
}

void ColorPickerWidget::onColorButtonClicked() {
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if (!button) return;

    QColor color = button->property("color").value<QColor>();
    setPrimaryColor(color);
    emit primaryColorChanged(color);
}


void ColorPickerWidget::onPrimaryColorClicked() {
    QColor temp = m_primaryColor;
    m_primaryColor = m_secondaryColor;
    m_secondaryColor = temp;

    updateColorDisplay();

    emit primaryColorChanged(m_primaryColor);
}

void ColorPickerWidget::onSecondaryColorClicked() {
    setPrimaryColor(m_secondaryColor);
    updateColorDisplay();
    emit primaryColorChanged(m_primaryColor);
}

void ColorPickerWidget::setPrimaryColor(const QColor &color) {
    if (m_primaryColor == color) return;
    m_primaryColor = color;
    updateColorDisplay();
}

QColor ColorPickerWidget::primaryColor() const {
    return m_primaryColor;
}

void ColorPickerWidget::updateColorDisplay() {
    m_primaryColorBtn->setStyleSheet(
        QString(
            "QPushButton {"
            "  background-color: %1;"
            "  border: 2px solid #333;"
            "  border-radius: 1px;"
            "}"
        ).arg(m_primaryColor.name()));


    m_secondaryColorBtn->setStyleSheet(
        QString(
            "QPushButton {"
            "  background-color: %1;"
            "  border: 5px solid #f0f0f0;"
            "  border-radius: 1px;"
            "}"
        ).arg(m_secondaryColor.name())
    );
}
