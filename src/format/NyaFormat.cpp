#include "NyaFormat.h"
#include <QFile>
#include <QDataStream>
#include <QBuffer>
#include <QDebug>

const QString NyaFormat::FILE_EXTENSION = "nya";
const QString NyaFormat::FILE_FILTER = "Nya Paint Files (*.nya)";

bool NyaFormat::save(const QString &filePath, CanvasModel *model) {
    if (!model) return false;

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) return false;

    QDataStream stream(&file);

    stream.setVersion(QDataStream::Qt_6_11);

    stream << MAGIC;
    stream << model->canvasSize().width();
    stream << model->canvasSize().height();

    QByteArray imageData;
    QBuffer buffer(&imageData);
    buffer.open(QIODevice::WriteOnly);
    model->image().save(&buffer, "PNG");

    stream << imageData;

    file.close();

    return true;
}

std::unique_ptr<CanvasModel> NyaFormat::load(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) return nullptr;

    QDataStream stream(&file);


    quint32 magic;
    qint32 width, height;

    stream >> magic;
    stream >> width;
    stream >> height;

    if (magic != MAGIC) return nullptr;

    if (width <= 0 || height <= 0 || width > 10000 || height > 10000) {
        qDebug() << "NyaFormat::load: invalid dimensions";
        return nullptr;
    }

    QByteArray imageData;
    stream >> imageData;

    QImage image = QImage::fromData(imageData, "PNG");
    if (image.isNull()) return nullptr;

    auto model = std::make_unique<CanvasModel>(QSize(width, height));
    model->image() = image;

    file.close();
    return model;
}
