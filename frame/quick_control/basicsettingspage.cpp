#include "basicsettingspage.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

using namespace dcc::widgets;

namespace dcc {

BasicSettingsModel::BasicSettingsModel(QObject *parent) :
    QObject(parent)
{

}

void BasicSettingsModel::setVolume(double volume)
{
    if (volume != m_volume) {
        m_volume = volume;
        emit volumeChanged(volume);
    }
}

void BasicSettingsModel::setBrightness(double brightness)
{
    if (brightness != m_brightness) {
        m_brightness = brightness;
        emit brightnessChanged(brightness);
    }
}


BasicSettingsWorker::BasicSettingsWorker(BasicSettingsModel *model, QObject *parent) :
    QObject(parent),
    m_model(model),
    m_audioInter(new com::deepin::daemon::Audio("com.deepin.daemon.Audio", "/com/deepin/daemon/Audio", QDBusConnection::sessionBus(), this)),
    m_displayInter(new com::deepin::daemon::Display("com.deepin.daemon.Display", "/com/deepin/daemon/Display", QDBusConnection::sessionBus(), this))
{
    m_audioInter->setSync(false);
    m_displayInter->setSync(false);

    connect(m_audioInter, &__Audio::DefaultSinkChanged, this, &BasicSettingsWorker::onDefaultSinkChanged);
    connect(m_displayInter, &__Display::BrightnessChanged, this, &BasicSettingsWorker::onBrightnessChanged);

    m_audioInter->defaultSink();
    m_displayInter->brightness();
}

void BasicSettingsWorker::setVolume(const double &volume)
{
    if(m_sinkInter)
        m_sinkInter->SetVolume(volume / 100.0, true);
}

void BasicSettingsWorker::setBrightness(const double &brightness)
{
    for (QString monitor : m_monitors) {
        m_displayInter->SetBrightness(monitor, brightness / 100.0);
    }
}

void BasicSettingsWorker::onDefaultSinkChanged(const QDBusObjectPath &value)
{
    if (m_sinkInter) m_sinkInter->deleteLater();

    m_sinkInter = new com::deepin::daemon::audio::Sink("com.deepin.daemon.Audio", value.path(), QDBusConnection::sessionBus(), this);
    m_sinkInter->setSync(false);

    connect(m_sinkInter, &com::deepin::daemon::audio::Sink::VolumeChanged, m_model, &BasicSettingsModel::setVolume);
    m_model->setVolume(m_sinkInter->volume());
}

void BasicSettingsWorker::onBrightnessChanged(const BrightnessMap value)
{
    m_monitors.clear();

    double brightness = 1;
    for (QString monitor : value.keys()) {
        m_monitors << monitor;
        double b = value.value(monitor);
        brightness = qMin(b, brightness);
    }

    m_model->setBrightness(brightness);
}

BasicSettingsPage::BasicSettingsPage(QWidget *parent)
    : QWidget(parent),

      m_volumeLow(new QLabel),
      m_volumeHigh(new QLabel),
      m_soundSlider(new DCCSlider),
      m_brightnessLow(new QLabel),
      m_brightnessHigh(new QLabel),
      m_lightSlider(new DCCSlider),
      m_model(new BasicSettingsModel(this)),
      m_worker(new BasicSettingsWorker(m_model, this))
{
    m_volumeLow->setObjectName("HomeVolumeLowLabel");
    m_volumeLow->setFixedSize(24, 24);
    m_volumeHigh->setObjectName("HomeVolumeHighLabel");
    m_volumeHigh->setFixedSize(24, 24);
    m_brightnessLow->setObjectName("HomeBrightnessLowLabel");
    m_brightnessLow->setFixedSize(24, 24);
    m_brightnessHigh->setObjectName("HomeBrightnessHighLabel");
    m_brightnessHigh->setFixedSize(24, 24);

    m_soundSlider->setOrientation(Qt::Horizontal);
    m_soundSlider->setRange(0, 150);
    m_lightSlider->setOrientation(Qt::Horizontal);
    m_lightSlider->setRange(0, 100);

    QHBoxLayout *volumeLayout = new QHBoxLayout;
    volumeLayout->setMargin(0);
    volumeLayout->setSpacing(0);
    volumeLayout->setContentsMargins(20, 0, 20, 0);
    volumeLayout->addWidget(m_volumeLow);
    volumeLayout->addSpacing(10);
    volumeLayout->addWidget(m_soundSlider);
    volumeLayout->addSpacing(10);
    volumeLayout->addWidget(m_volumeHigh);

    QHBoxLayout *brightnessLayout = new QHBoxLayout;
    brightnessLayout->setMargin(0);
    brightnessLayout->setSpacing(0);
    brightnessLayout->setContentsMargins(20, 0, 20, 0);
    brightnessLayout->addWidget(m_brightnessLow);
    brightnessLayout->addSpacing(10);
    brightnessLayout->addWidget(m_lightSlider);
    brightnessLayout->addSpacing(10);
    brightnessLayout->addWidget(m_brightnessHigh);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(volumeLayout);
    mainLayout->addLayout(brightnessLayout);
    setLayout(mainLayout);

    auto onVolumeChanged = [this] (const double &volume) {
        m_soundSlider->blockSignals(true);
        m_soundSlider->setValue(volume * 100);
        m_soundSlider->blockSignals(false);
    };

    auto onBrightnessChanged = [this] (const double &brightness) {
        m_lightSlider->blockSignals(true);
        m_lightSlider->setValue(brightness * 100);
        m_lightSlider->blockSignals(false);
    };

    connect(m_model, &BasicSettingsModel::volumeChanged, onVolumeChanged);
    connect(m_model, &BasicSettingsModel::brightnessChanged, onBrightnessChanged);

    onVolumeChanged(m_model->volume());
    onBrightnessChanged(m_model->brightness());

    connect(m_soundSlider, &DCCSlider::valueChanged, m_worker, &BasicSettingsWorker::setVolume);
    connect(m_lightSlider, &DCCSlider::valueChanged, m_worker, &BasicSettingsWorker::setBrightness);
}

}