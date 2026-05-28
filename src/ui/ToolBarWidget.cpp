#include "ToolBarWidget.h"
#include <QLabel>


ToolBarWidget::ToolBarWidget(QWidget *parent)
    : QWidget(parent)
      , m_layout(nullptr)
      , m_buttonGroup(nullptr)
      , m_toolManager(nullptr)
      , m_nextId(0) {
    setupUI();
}

void ToolBarWidget::setupUI() {
    m_layout = new QVBoxLayout(this);
    m_layout->setContentsMargins(4, 4, 4, 4);
    m_layout->setSpacing(4);


    QLabel *title = new QLabel("Tools", this);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-weight: bold; font-size: 12px;");
    m_layout->addWidget(title);

    m_buttonGroup = new QButtonGroup(this);
    m_buttonGroup->setExclusive(true);
    connect(m_buttonGroup, &QButtonGroup::idClicked, this, &ToolBarWidget::onToolButtonClicked);

    m_layout->addStretch();

    setFixedWidth(60);
}

QPushButton *ToolBarWidget::createToolButton(Tool *tool, int id) {
    QPushButton *button = new QPushButton(this);
    button->setCheckable(true);
    button->setToolTip(tool->name());
    button->setFixedSize(48, 48);
    button->setCursor(Qt::PointingHandCursor);

    button->setStyleSheet(
        "QPushButton {"
        "  background-color: #f0f0f0;"
        "  border: 2px solid #ccc;"
        "  border-radius: 4px;"
        "  font-size: 10px;"
        "  font-weight: bold;"
        "}"
        "QPushButton:checked {"
        "  background-color: #cce5ff;"
        "  border-color: #0078d4;"
        "}"
        "QPushButton:hover {"
        "  background-color: #e0e0e0;"
        "}"
    );

    button->setText(tool->name().left(5));

    m_buttonGroup->addButton(button, id);
    m_layout->insertWidget(m_layout->count() - 1, button);

    return button;
}

void ToolBarWidget::setToolManager(ToolManager *manager) {
    m_toolManager = manager;

    for (QAbstractButton *button: m_buttonGroup->buttons()) {
        m_buttonGroup->removeButton(button);
        m_layout->removeWidget(button);
        delete button;
    }
    m_nextId = 0;

    for (int i = 0; i < m_toolManager->toolCount(); i++) {
        Tool *tool = m_toolManager->toolAt(i);
        createToolButton(tool, m_nextId++);
    }

    if (m_toolManager->currentTool()) {
        for (int i = 0; i < m_toolManager->toolCount(); i++) {
            if (m_toolManager->toolAt(i) == m_toolManager->currentTool()) {
                QAbstractButton *button = m_buttonGroup->button(i);
                if (button) button->setChecked(true);
                break;
            }
        }
    }

    connect(m_toolManager,
            &ToolManager::toolChanged,
            this,
            &ToolBarWidget::onToolChanged
    );
}

void ToolBarWidget::onToolButtonClicked(int id) {
    if (m_toolManager) {
        m_toolManager->selectTool(id);
    }
}


void ToolBarWidget::onToolChanged(Tool *tool) {
    for (int i = 0; i < m_toolManager->toolCount(); i++) {
        if (m_toolManager->toolAt(i) == tool) {
            QAbstractButton *button = m_buttonGroup->button(i);
            if (button) button->setChecked(true);
            break;
        }
    }
}
