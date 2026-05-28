#pragma once
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QButtonGroup>
#include "../tools/ToolManager.h"

class ToolBarWidget : public QWidget {
    Q_OBJECT

public:
    explicit ToolBarWidget(QWidget *parent = nullptr);

    void setToolManager(ToolManager *manager);

private slots:
    void onToolButtonClicked(int id);

    void onToolChanged(Tool *tool);

private:
    void setupUI();

    QPushButton *createToolButton(Tool *tool, int id);

    QVBoxLayout *m_layout;
    QButtonGroup *m_buttonGroup;
    ToolManager *m_toolManager;
    int m_nextId;
};
