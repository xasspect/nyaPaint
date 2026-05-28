#pragma once
#include <QObject>
#include <QList>
#include "Tool.h"


class ToolManager : public QObject {
    Q_OBJECT

public:
    explicit ToolManager(QObject *parent = nullptr);


    void addTool(Tool *tool);


    void selectTool(int index);


    void selectTool(const QString &name);


    Tool *currentTool() const;


    QList<Tool *> tools() const;


    Tool *toolAt(int index) const;

    int toolCount() const;

signals:
    void toolChanged(Tool *newTool);

private:
    QList<Tool *> m_tools;
    int m_currentIndex;
};
