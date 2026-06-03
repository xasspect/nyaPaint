#include "MainWindow.h"

#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QStatusBar>
#include <QLabel>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QFileInfo>

#include "tools/EraserTool.h"
#include "tools/LineTool.h"
#include "format/NyaFormat.h"
#include "tools/FillTool.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
      , m_canvas(nullptr)
      , m_toolManager(nullptr)
      , m_toolBar(nullptr)
      , m_colorPicker(nullptr)
      , m_brushSize(nullptr)
      , m_modified(false) {
    setWindowTitle("Nya Paint - untitled.nya");
    setWindowIcon(QIcon(":/icon.ico"));
    resize(1280, 720);

    setupUI();
    setupMenuBar();
    setupStatusBar();

    connect(m_model.get(), &CanvasModel::imageChanged, this, [this]() {
        if (!m_modified) {
            m_modified = true;
            updateTitle();
        }
    });
}

void MainWindow::setupUI() {
    QWidget *centralWidget = new QWidget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    m_toolBar = new ToolBarWidget(centralWidget);
    mainLayout->addWidget(m_toolBar);

    m_colorPicker = new ColorPickerWidget(centralWidget);
    mainLayout->addWidget(m_colorPicker);

    m_brushSize = new BrushSizeWidget(centralWidget);
    mainLayout->addWidget(m_brushSize);

    m_model = std::make_unique<CanvasModel>(QSize(1280, 720));
    m_canvas = new Canvas(centralWidget);
    m_canvas->setModel(m_model.get());
    mainLayout->addWidget(m_canvas, 1);

    setCentralWidget(centralWidget);

    m_toolManager = new ToolManager(this);
    m_toolManager->addTool(new PenTool(this));
    m_toolManager->addTool(new EraserTool(this));
    m_toolManager->addTool(new LineTool(this));
    m_toolManager->addTool(new FillTool(this));


    m_toolBar->setToolManager(m_toolManager);
    m_canvas->setTool(m_toolManager->currentTool());

    connect(m_canvas, &Canvas::mouseMoved, this, &MainWindow::onCanvasMouseMoved);
    connect(m_toolManager, &ToolManager::toolChanged, this, &MainWindow::onToolChanged);
    connect(m_colorPicker, &ColorPickerWidget::primaryColorChanged, this, &MainWindow::onPrimaryColorChanged);
    connect(m_brushSize, &BrushSizeWidget::sizeChanged, this, &MainWindow::onSizeChanged);
}

void MainWindow::setupMenuBar() {
    QMenuBar *menuBar = this->menuBar();

    QMenu *fileMenu = menuBar->addMenu("&File");

    QAction *newAction = fileMenu->addAction("&New");
    newAction->setShortcut(QKeySequence::New);
    connect(newAction, &QAction::triggered, this, &MainWindow::onNew);

    QAction *openAction = fileMenu->addAction("&Open...");
    openAction->setShortcut(QKeySequence::Open);
    connect(openAction, &QAction::triggered, this, &MainWindow::onOpen);

    QAction *saveAction = fileMenu->addAction("&Save");
    saveAction->setShortcut(QKeySequence::Save);
    connect(saveAction, &QAction::triggered, this, &MainWindow::onSave);

    QAction *saveAsAction = fileMenu->addAction("Save &As...");
    saveAsAction->setShortcut(QKeySequence("Ctrl+Shift+S"));
    connect(saveAsAction, &QAction::triggered, this, &MainWindow::onSaveAs);

    fileMenu->addSeparator();

    QAction *exitAction = fileMenu->addAction("&Exit");
    exitAction->setShortcut(QKeySequence::Quit);
    connect(exitAction, &QAction::triggered, this, &QMainWindow::close);

    QMenu *editMenu = menuBar->addMenu("&Edit");

    QAction *undoAction = editMenu->addAction("&Undo");
    undoAction->setShortcut(QKeySequence::Undo);
    connect(undoAction, &QAction::triggered, this, [this]() {

        Tool * current = m_toolManager->currentTool();
        if (current && current->isDrawing()) return;
        if (m_model) m_model->undo();

    });

    QAction *redoAction = editMenu->addAction("&Redo");
    redoAction->setShortcut(QKeySequence::Redo);
    connect(redoAction, &QAction::triggered, this, [this]() {
        Tool * current = m_toolManager->currentTool();
        if (current && current->isDrawing()) return;

        if (m_model) m_model->redo();
    });
}

void MainWindow::setupStatusBar() {
    m_mousePosLabel = new QLabel("0, 0 px", this);
    m_mousePosLabel->setMinimumWidth(120);
    statusBar()->addWidget(m_mousePosLabel);


    m_canvasSizeLabel = new QLabel("1280 x 720 px", this);
    m_canvasSizeLabel->setMinimumWidth(120);
    statusBar()->addPermanentWidget(m_canvasSizeLabel);
}


void MainWindow::onNew() {
    if (!maybeSave()) return;
    m_model->newCanvas(QSize(1280, 720));
    m_model->undoManager()->clear();
    m_currentFilePath.clear();
    m_modified = false;
    m_canvasSizeLabel->setText("1280 x 720 px");
    updateTitle();
}

void MainWindow::onOpen() {
    if (!maybeSave()) return;

    QString filePath = QFileDialog::getOpenFileName(
        this, "Open Nya File", QString(), NyaFormat::FILE_FILTER);

    if (filePath.isEmpty()) return;

    auto model = NyaFormat::load(filePath);
    if (model) {
        m_model = std::move(model);
        m_canvas->setModel(m_model.get());
        m_model->undoManager()->clear();
        m_model->undoManager()->saveState(m_model->image());
        m_currentFilePath = filePath;
        m_modified = false;
        m_canvasSizeLabel->setText(
            QString("%1 x %2 px")
            .arg(QString::number(m_model->canvasSize().width()))
            .arg(QString::number(m_model->canvasSize().height()))
        );
        updateTitle();
    } else {
        QMessageBox::warning(this, "Error", "Failed to open file!");
    }
}

void MainWindow::onSave() {
    if (m_currentFilePath.isEmpty()) {
        onSaveAs();
        return;
    }

    if (NyaFormat::save(m_currentFilePath, m_model.get())) {
        m_modified = false;
        updateTitle();
    } else {
        QMessageBox::warning(this, "Error", "Failed to save file!");
    }
}

void MainWindow::onSaveAs() {
    QString filePath = QFileDialog::getSaveFileName(
        this, "Save Nya File", QString(), NyaFormat::FILE_FILTER);

    if (filePath.isEmpty()) return;

    if (!filePath.endsWith("." + NyaFormat::FILE_EXTENSION)) {
        filePath += "." + NyaFormat::FILE_EXTENSION;
    }

    if (NyaFormat::save(filePath, m_model.get())) {
        m_currentFilePath = filePath;
        m_modified = false;
        updateTitle();
    } else {
        QMessageBox::warning(this, "Error", "Failed to save file!");
    }
}

void MainWindow::onToolChanged(Tool *tool) {
    Tool* previous = m_toolManager->currentTool();
    if (previous && previous != tool) {
        previous->cancelLine();
    }

    if (m_canvas) m_canvas->setTool(tool);

    if (tool && m_colorPicker) {
        m_colorPicker->setPrimaryColor(tool->color());
    }
}

void MainWindow::onPrimaryColorChanged(const QColor &color) {
    for (int i = 0; i < m_toolManager->toolCount(); i++) {
        m_toolManager->toolAt(i)->setColor(color);
    }
}

void MainWindow::onSizeChanged(int size) {
    for (int i = 0; i < m_toolManager->toolCount(); i++) {
        m_toolManager->toolAt(i)->setSize(size);
    }
}

void MainWindow::updateTitle() {
    QString title = "Nya Paint - ";
    if (m_currentFilePath.isEmpty()) {
        title += "untitled.nya";
    } else {
        title += QFileInfo(m_currentFilePath).fileName();
    }
    if (m_modified) title += " *";
    setWindowTitle(title);
}

bool MainWindow::maybeSave() {
    if (!m_modified) return true;

    QMessageBox::StandardButton result = QMessageBox::question(
        this, "Save Changes?", "Do you want to save changes?",
        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

    if (result == QMessageBox::Save) {
        onSave();
        return !m_modified;
    }
    return result == QMessageBox::Discard;
}

void MainWindow::onCanvasMouseMoved(const QPoint &pos) {
    m_mousePosLabel->setText(
        QString::number(pos.x()) + ", " + QString::number(pos.y()) + " px"
    );
}
