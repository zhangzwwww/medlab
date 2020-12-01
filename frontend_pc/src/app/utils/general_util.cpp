#include "general_util.h"

#include <QDir>
#include <QStringList>

QString GetFileName(QString &file_path) {
    QStringList paths = file_path.split(QDir::separator());
    return paths.back();
}
