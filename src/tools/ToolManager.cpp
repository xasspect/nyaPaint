#include "ToolManager.h"
#include <QDebug>

ToolManager::ToolManager(QObject *parent)
    : QObject(parent)
      , m_currentIndex(-1) {
}


void ToolManager::addTool(Tool *tool) {
    if (!tool) {
        return;
    }

    m_tools.append(tool);

    if (m_tools.size() == 1) {
        selectTool(0);
    }
}

void ToolManager::selectTool(int index) {
    if (index < 0 || index >= m_tools.size()) { return; }
    if (index == m_currentIndex) { return; }

    m_currentIndex = index;
    emit toolChanged(m_tools[index]);

    // qDebug() << "выбрано : " << m_tools[index]->name();
}

Tool *ToolManager::currentTool() const {
    if (m_currentIndex >= 0 && m_currentIndex < m_tools.size()) {
        return m_tools[m_currentIndex];
    }
    return nullptr;
}

QList<Tool *> ToolManager::tools() const {
    return m_tools;
}

Tool *ToolManager::toolAt(int index) const {
    if (index >= 0 && index < m_tools.size()) {
        return m_tools[index];
    }
    return nullptr;
}

int ToolManager::toolCount() const {
    return m_tools.size();
}
