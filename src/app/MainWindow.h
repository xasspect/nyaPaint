#pragma once
#include <QMainWindow>

#include "../canvas/Canvas.h"
#include "../canvas/CanvasModel.h"
#include "../tools/ToolManager.h"
#include "../tools/PenTool.h"
#include "../ui/ToolBarWidget.h"
#include "ui/ColorPickerWidget.h"
#include "ui/BrushSizeWidget.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() = default;

private slots:
    void onNew();

    void onOpen();

    void onSave();

    void onSaveAs();

    void onToolChanged(Tool *tool);

    void onPrimaryColorChanged(const QColor &color);

    void onSizeChanged(int size);

    void onCanvasMouseMoved(const QPoint &pos);

private:
    void setupUI();

    void setupMenuBar();

    void setupStatusBar();

    void updateTitle();

    bool maybeSave();

    Canvas *m_canvas;
    std::unique_ptr<CanvasModel> m_model;
    ToolManager *m_toolManager;
    ToolBarWidget *m_toolBar;
    ColorPickerWidget *m_colorPicker;
    BrushSizeWidget *m_brushSize;

    QString m_currentFilePath;
    bool m_modified;

    QLabel *m_mousePosLabel;
    QLabel *m_canvasSizeLabel;
};
