#ifndef FILESYSTEMMODEL_H
#define FILESYSTEMMODEL_H

#include <QDebug>
#include <QFileSystemModel>
#include <QJSValue>

class FileSystemModel : public QFileSystemModel {
    Q_OBJECT
public:
    using QFileSystemModel::fetchMore;
    FileSystemModel(QObject *parent = nullptr) : QFileSystemModel(parent) {
        setRootPath(QDir::rootPath());
    }
    Q_INVOKABLE QString filePath(const QModelIndex &index) {
        return QFileSystemModel::filePath(index);
    }
    Q_INVOKABLE void fetchMore(const QModelIndex &parent, QJSValue jsCallback) {
        QFileSystemModel::fetchMore(parent);
        QObject *context = new QObject(this);
        QObject::connect(this, &QFileSystemModel::directoryLoaded, context, [jsCallback, context]() mutable {
            jsCallback.call(QJSValueList());
            delete context;
        });
    }
    Q_INVOKABLE QModelIndex rootModelIndex() {
        return index(-1, -1);
    }
    Q_INVOKABLE QString getSizeInMB(const QString &path) {
        QFileInfo fileInfo(path);
        if (!fileInfo.exists()) {
            return "Файл или директория не существует.";
        }

        double sizeInMB = 0.0;

        if (fileInfo.isFile()) {
            sizeInMB = fileInfo.size() / 1048576.0;
        } else if (fileInfo.isDir()) {
            qint64 totalSize = 0;
            QDir dir(path);
            QFileInfoList fileList = dir.entryInfoList(QDir::Files | QDir::NoSymLinks);
            for (const QFileInfo &file : fileList) {
                totalSize += file.size();
            }

            QFileInfoList dirList = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
            for (const QFileInfo &subDir : dirList) {
                totalSize += getSizeInMB(subDir.absoluteFilePath()).toDouble() * 1048576;  // Рекурсивный подсчет
            }
            sizeInMB = totalSize / 1048576.0;
        }

        return QString::number(sizeInMB, 'f', 2);  // Два знака после запятой
    }
};

#endif // FILESYSTEMMODEL_H
