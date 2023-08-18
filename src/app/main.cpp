#include <QApplication>
#include <QNetworkProxyFactory>

#include <extensionsystem/pluginmanager.h>

#include <QBreakpadHandler.h>

#include <QMAppExtension.h>
#include <QMConsole.h>
#include <QMDecoratorV2.h>
#include <QMSystem.h>

#include <CoreApi/ILoader.h>

#include <loaderconfig.h>

#include "choruskit_config.h"

class MyLoaderConfiguration : public LoaderConfiguration {
public:
    MyLoaderConfiguration() {
        allowRoot = false;
        pluginIID = QString("org.ChorusKit.%1.Plugin").arg(APP_NAME);
        splashSettingPath = qAppExt->appShareDir() + "/config.json";
        userSettingPath = qAppExt->appDataDir();
        systemSettingPath = qAppExt->appShareDir();
        
        pluginPaths << qAppExt->appPluginsDir();
    }

    bool preprocessArguments(QStringList &arguments, int *code) override {
        if (!qAppExt->createDataAndTempDirs()) {
            *code = -1;
            return false;
        }
        return true;
    }

    void beforeLoadPlugin(QSplashScreen *screen) override {
        // Make sure we honor the system's proxy settings
        QNetworkProxyFactory::setUseSystemConfiguration(true);

        // Don't show plugin manager debug info
        QLoggingCategory::setFilterRules(QLatin1String("qtc.*.debug=false"));

        // Restore language and themes
        {
            auto settings = ExtensionSystem::PluginManager::settings();
            settings->beginGroup("Preferences");
            auto value = settings->value("Translation");
            if (value.type() == QVariant::String) {
                qIDec->setLocale(value.toString());
            }

            value = settings->value("Theme");
            if (value.type() == QVariant::String) {
                qIDec->setTheme(value.toString());
            }
            settings->endGroup();
        }

        // Set ILoader settings path
        Core::ILoader &loader = *Core::ILoader::instance();
        loader.setSettingsPath(QSettings::UserScope,
                               QString("%1/%2.settings.json").arg(userSettingPath, qApp->applicationName()));
        loader.setSettingsPath(QSettings::SystemScope,
                               QString("%1/%2.settings.json").arg(systemSettingPath, qApp->applicationName()));

        // Add initial routine to loader object pool
        auto initRoutine = new Core::InitialRoutine(screen);
        loader.addObject("choruskit_init_routine", initRoutine);
        QObject::connect(initRoutine, &Core::InitialRoutine::done, [&] {
            loader.removeObject(initRoutine); //
            delete initRoutine;
        });
    }

    void afterLoadPlugin() override {
        // Do nothing
    }
};

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QMAppExtension host;

    a.setApplicationName(APP_NAME);
    a.setApplicationVersion(APP_VERSION);
    a.setOrganizationName("ChorusKit");
    a.setOrganizationDomain("ChorusKit");

    Core::ILoader loader;

#ifdef CONFIG_ENABLE_BREAKPAD
    QBreakpadInstance.setDumpPath(host.appDataDir() + "/crashes");
#endif

    return MyLoaderConfiguration().run();
}