#include <QApplication>
#include <QFileSystemModel>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QTreeView>
#include "model/FileSystemModel.h"
#include "model/treemodel.h"
#include <QDebug>
#include <QTreeView>

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QApplication app(argc, argv);

    qmlRegisterType<TreeModel>("Models", 1, 0, "TreeModel");
    qmlRegisterType<FileSystemModel>("Models", 1, 0, "FileSystemModel");

//    new FileSystemModel();

//    FileSystemModel* model = new FileSystemModel();
//    QModelIndex index = model->rootModelIndex();
//    qDebug() << index;
//    qDebug() << model->rowCount(index) << model->filePath(index);
//    QObject::connect(model, &QFileSystemModel::directoryLoaded, [model]() {
//        qDebug() << "this!!";
//        QModelIndex index = model->rootModelIndex();
//        qDebug() << index;
//        qDebug() << model->rowCount(index) << model->filePath(index);
//    });
//    model->fetchMore(index);
//    if (model->canFetchMore(index)) {
//        qDebug() << "fetch more";
//        model->fetchMore(index);
//    }
//    QTreeView *tree = new QTreeView();
//    tree->setModel(model);
//    tree->show();

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
