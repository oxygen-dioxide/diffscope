#include "mandarinanalyzer.h"

#include <qrandom.h>

namespace LyricTool::Internal {
    MandarinAnalyzer::MandarinAnalyzer(const QString &id, QObject *parent)
        : SingleCharAnalyzer(id, parent) {
        setDisplayName(tr("Mandarin"));
        setDescription(tr("Capture Mandarin characters."));
    }

    bool isHanzi(const QChar &c) {
        return c >= QChar(0x4e00) && c <= QChar(0x9fa5);
    }

    bool MandarinAnalyzer::contains(QChar c) const {
        return isHanzi(c);
    }

    QString MandarinAnalyzer::randString() const {
        const int unicode = QRandomGenerator::global()->bounded(0x4e00, 0x9fa5 + 1);
        return {QChar(unicode)};
    }

}