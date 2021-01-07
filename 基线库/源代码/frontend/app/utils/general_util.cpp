#include "general_util.h"

#include <QDir>
#include <QStringList>

QString GetFileName(QString &file_path) {

#ifdef Q_OS_MACOS   // Define on MACOS system
    QStringList paths = file_path.split(QDir::separator());
#endif

#ifdef Q_OS_WIN32   // Define on windows system
    QStringList paths = file_path.split("/");
#endif

    return paths.back();
}
