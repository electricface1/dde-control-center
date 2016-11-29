#ifndef SYSTEMINFOWORK_H
#define SYSTEMINFOWORK_H

#include <QObject>
#include <com_deepin_daemon_systeminfo.h>
#include <com_deepin_daemon_grub2.h>

using SystemInfoInter=com::deepin::daemon::SystemInfo;
using GrubDbus=com::deepin::daemon::Grub2;

class SystemInfoWork : public QObject
{
    Q_OBJECT

public:
    explicit SystemInfoWork(QObject* parent = 0);
    QString version() const;
    qlonglong type() const;
    QString processor() const;
    qlonglong memory() const;
    qlonglong disk() const;
    QStringList entryLists() const;
    QString defaultEntry() const;

signals:
    void DiskCapChanged(qulonglong  value) const;
    void MemoryCapChanged(qulonglong  value) const;
    void ProcessorChanged(const QString & value) const;
    void SystemTypeChanged(qlonglong  value) const;
    void VersionChanged(const QString & value) const;

public slots:
    void setBootDelay(bool value);
    void setEnableTheme(bool value);
    void setDefaultEntry(const QString& entry);

private:
    SystemInfoInter* m_systemInfoInter;
    GrubDbus* m_dbusGrub;
};

#endif // SYSTEMINFOWORK_H
