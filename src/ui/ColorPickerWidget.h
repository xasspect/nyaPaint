#pragma once
#include <QWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QColor>
#include <QList>

class ColorPickerWidget : public QWidget {
    Q_OBJECT

public:
    explicit ColorPickerWidget(QWidget *parent = nullptr);

    QColor primaryColor() const;

    void setPrimaryColor(const QColor &color);

signals:
    void primaryColorChanged(const QColor &color);

private slots:
    void onColorButtonClicked();

    void onPrimaryColorClicked();

    void onSecondaryColorClicked();

private:
    void setupUI();

    QPushButton *createColorButton(const QColor &color);

    void updateColorDisplay();

    QPushButton *m_primaryColorBtn;
    QPushButton *m_secondaryColorBtn;

    QColor m_primaryColor;
    QColor m_secondaryColor;

    QGridLayout *m_gridLayout;

    QList<QColor> m_defaultColors = {
        Qt::black,
        Qt::darkGray,
        QColor(128, 128, 128),
        Qt::gray,
        Qt::white,
        QColor(128, 0, 0),
        Qt::red,
        QColor(255, 128, 0),
        QColor(255, 255, 0),
        QColor(0, 128, 0),
        Qt::green,
        QColor(0, 255, 128),
        QColor(0, 128, 128),
        Qt::cyan,
        QColor(0, 0, 128),
        Qt::blue,
        QColor(128, 0, 128),
        Qt::magenta,
        QColor(128, 64, 0),
        QColor(255, 200, 150)
    };
};
