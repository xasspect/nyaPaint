#pragma once
#include <QString>
#include <memory>
#include "../canvas/CanvasModel.h"

class NyaFormat {
public:
    static const QString FILE_EXTENSION;
    static const QString FILE_FILTER;


    static const quint32 MAGIC = 0x4E594121;


    static bool save(const QString &filePath, CanvasModel *model);

    static std::unique_ptr<CanvasModel> load(const QString &filePath);
};
