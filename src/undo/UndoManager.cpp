#include "UndoManager.h"

#include <QPainter>

UndoManager::UndoManager(QObject* parent)
    : QObject(parent)
{
}


void UndoManager::setBaseState(const QImage& image) {
    m_lastState = image.copy();
    clear();
}
void UndoManager::saveState(const QImage& currentImage) {
    // Вычисляем прямоугольник, в котором пиксели отличаются
    QRect diffRect = calculateDiffBounds(m_lastState, currentImage);

    // Если изменений нет (кликнули и не сдвинули кисть), ничего не делаем
    if (diffRect.isNull()) return;

    // Создаем "заплатку" только для измененной области
    CanvasPatch patch;
    patch.rect = diffRect;
    patch.oldPixels = m_lastState.copy(diffRect);
    patch.newPixels = currentImage.copy(diffRect);

    m_undoStack.push(patch);
    m_redoStack.clear();


    int totalMemory = 0;
    for (const auto& p : m_undoStack) {
        totalMemory += p.oldPixels.sizeInBytes() + p.newPixels.sizeInBytes();
    }
    while (totalMemory > MAX_MEMORY_MB * 1024 * 1024 && m_undoStack.size() > 1) {
        CanvasPatch oldest = m_undoStack.first();
        totalMemory -= (oldest.oldPixels.sizeInBytes() + oldest.newPixels.sizeInBytes());
        m_undoStack.removeFirst();
    }

    // Обновляем последнее известное состояние
    m_lastState = currentImage.copy();

    emit historyChanged();
}

QRect UndoManager::calculateDiffBounds(const QImage& img1, const QImage& img2) const {
    if (img1.size() != img2.size() || img1.format() != img2.format()) {
        return img1.rect(); // Разные форматы - возвращаем весь холст
    }

    int width = img1.width();
    int height = img1.height();
    int minX = width, minY = height, maxX = -1, maxY = -1;

    // Линейный алгоритм поиска границ изменений (O(N) по пикселям)
    for (int y = 0; y < height; ++y) {
        const QRgb* line1 = reinterpret_cast<const QRgb*>(img1.constScanLine(y));
        const QRgb* line2 = reinterpret_cast<const QRgb*>(img2.constScanLine(y));

        bool rowHasDiff = false;
        for (int x = 0; x < width; ++x) {
            if (line1[x] != line2[x]) {
                rowHasDiff = true;
                if (x < minX) minX = x;
                if (x > maxX) maxX = x;
            }
        }
        if (rowHasDiff) {
            if (y < minY) minY = y;
            if (y > maxY) maxY = y;
        }
    }

    if (maxX < 0) return QRect(); // Различий нет

    // Возвращаем точный прямоугольник
    return QRect(QPoint(minX, minY), QPoint(maxX, maxY));
}

bool UndoManager::canUndo() const {
    return !m_undoStack.isEmpty();
}

bool UndoManager::canRedo() const {
    return !m_redoStack.isEmpty();
}

QImage UndoManager::undo(QImage currentCanvas) {
    if (m_undoStack.isEmpty()) return currentCanvas;

    CanvasPatch patch = m_undoStack.pop();
    m_redoStack.push(patch);

    // Вклеиваем СТАРЫЕ пиксели поверх текущего холста
    QPainter painter(&currentCanvas);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.drawImage(patch.rect.topLeft(), patch.oldPixels);
    painter.end();

    m_lastState = currentCanvas.copy();
    emit historyChanged();
    return currentCanvas;
}

QImage UndoManager::redo(QImage currentCanvas) {
    if (m_redoStack.isEmpty()) return currentCanvas;

    CanvasPatch patch = m_redoStack.pop();
    m_undoStack.push(patch);

    // Вклеиваем НОВЫЕ пиксели поверх текущего холста
    QPainter painter(&currentCanvas);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.drawImage(patch.rect.topLeft(), patch.newPixels);
    painter.end();

    m_lastState = currentCanvas.copy();
    emit historyChanged();
    return currentCanvas;
}

void UndoManager::clear() {
    m_undoStack.clear();
    m_redoStack.clear();
    emit historyChanged();
}