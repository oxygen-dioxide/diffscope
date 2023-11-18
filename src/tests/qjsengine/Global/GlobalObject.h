#ifndef DIFFSCOPE_GLOBALOBJECT_H
#define DIFFSCOPE_GLOBALOBJECT_H

#include <QJSValue>
#include <QObject>

class QJSEngine;

class GlobalRegistryObject;
class GlobalStorageObject;
class Console;

class GlobalObject : public QObject {
    Q_OBJECT
    Q_PROPERTY(QJSValue registry READ jsRegistry)
    Q_PROPERTY(QJSValue storage READ jsStorage)
public:
    explicit GlobalObject(QObject *parent = nullptr);
    ~GlobalObject() override;
    static GlobalObject *instance();

    QString stackTrace(int depth = 0);
    QString fileTrace(int depth = 0);

    QJSEngine *engine() const;
    QJSValue load(const QString &scriptFilename);
    GlobalRegistryObject *registry() const;
    GlobalStorageObject *storage() const;
    Console *console() const;

    QJSValue jsRegistry() const;
    QJSValue jsStorage() const;

    struct JSEnumEntry {
        QString s;
        int i = -1;
    };
    void defineEnum(const QString &enumName, const QList<JSEnumEntry> &entries);

    template<class T>
    static T *getOfWrappedObject(const QJSValue &obj) {
        auto *v = qobject_cast<T *>(obj.property("_p").toQObject());
        if (v)
            return v;
        v = qobject_cast<T *>(obj.property("_p").property("_subObject").toQObject());
        return v;
    }

public slots:
    void pause();

private:
    QJSEngine *m_engine;

    GlobalRegistryObject *m_registry;
    GlobalStorageObject *m_storage;
    Console *m_console;

    QJSValue m_registryObject;
    QJSValue m_storageObject;

    void installTextCodec();
    void installConsole();
};

#define jsGlobal GlobalObject::instance()
#define JS_QOBJ jsGlobal->engine()->newQObject
#define JS_THROW jsGlobal->engine()->throwError

#endif // DIFFSCOPE_GLOBALOBJECT_H
