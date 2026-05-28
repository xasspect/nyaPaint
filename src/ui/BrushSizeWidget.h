#pragma once
#include <QWidget>
#include <QSlider>
#include <QSpinBox>
#include <QLabel>

class BrushSizeWidget : public QWidget {
    Q_OBJECT

public:
    explicit BrushSizeWidget(QWidget *parent = nullptr);

    int size() const;

    void setSize(int size);

signals:
    void sizeChanged(int size);

private slots:
    void onSliderChanged(int value);

    void onSpinBoxChanged(int value);

private:
    void setupUI();

    void updatePreview(int size);

    QSlider *m_slider;
    QSpinBox *m_spinBox;
    QLabel *m_previewLabel;
    bool m_updating;
};
