#include "outputsysteminterface.h"
#include "outputsysteminterface_p.h"

#include <audioplugin/internal/outputsystem.h>
#include <audioplugin/iaudio.h>

namespace Audio {

    void OutputSystemInterfacePrivate::init(AbstractOutputSystem *abstractOutputSystem_, bool isVST_) {
        abstractOutputSystem = abstractOutputSystem_;
        isVST = isVST_;
    }

    OutputSystemInterface::OutputSystemInterface(QObject *parent) : QObject(parent), d_ptr(new OutputSystemInterfacePrivate) {
        Q_D(OutputSystemInterface);
        d->q_ptr = this;
    }
    OutputSystemInterface::~OutputSystemInterface() {
    }
    bool OutputSystemInterface::isVST() const {
        Q_D(const OutputSystemInterface);
        return d->isVST;
    }
    talcs::AudioDriverManager *OutputSystemInterface::audioDriverManager() const {
        Q_D(const OutputSystemInterface);
        if (!d->isVST)
            return static_cast<OutputSystem *>(d->abstractOutputSystem)->driverManager();
        return nullptr;
    }
    talcs::AudioDriver *OutputSystemInterface::audioDriver() const {
        Q_D(const OutputSystemInterface);
        if (!d->isVST)
            return static_cast<OutputSystem *>(d->abstractOutputSystem)->driver();
        return nullptr;
    }
    talcs::AudioDevice *OutputSystemInterface::audioDevice() const {
        Q_D(const OutputSystemInterface);
        return d->abstractOutputSystem->device();
    }
    talcs::MixerAudioSource *OutputSystemInterface::preMixer() const {
        Q_D(const OutputSystemInterface);
        return d->abstractOutputSystem->preMixer();
    }
    bool OutputSystemInterface::makeReady() {
        Q_D(OutputSystemInterface);
        return d->abstractOutputSystem->makeReady();
    }
}