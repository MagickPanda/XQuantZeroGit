#ifndef CCONFIGHANDLER_H
#define CCONFIGHANDLER_H

#include <QJsonDocument>
#include <QJsonParseError>
#include <QFile>
#include <QJsonObject>
#include <QDebug>
#include <QJsonArray>
#include <QMap>


class CConfigHandler
{
public:
    enum {
        CONFIG_CTP = 0,
        CONFIG_SIMNOW = 1,
        CONFIG_USERINFO = 2,
    };

    const QString default_filename = "xq_conig.json";

    // config tags count / 配置标签数量
    static const qint32 sections_count = 3;

    // hard-coded QString name for config tabs ## 硬coded QString配置名称
    const QString sections[sections_count] = {
        "ctp_server", "simnow_server", "userinfo"
    };


    // config tags count / 配置标签数量
    static const qint32 count_ctp_server = 6;

    // hard-coded QString name for config tags ## 硬coded QString配置名称
    const QString tags_ctp_server[count_ctp_server] = {
        "trade_front", "trade_port", "trade_broker_id",
        "market_front", "market_port", "market_broker_id",
    };

    // config tags count / 配置标签数量
    static const qint32 count_simnow_server = 6;

    // hard-coded QString name for config tags ## 硬coded QString配置名称
    const QString tags_simnow_server[count_simnow_server] = {
        "trade_front", "trade_port", "trade_broker_id",
        "market_front", "market_port", "market_broker_id",
    };

    // config tags count / 配置标签数量
    static const qint32 count_userinfo = 3;

    // hard-coded QString name for config tags ## 硬coded QString配置名称
    const QString tags_userinfo[count_userinfo] = {
        "username", "password", "test",
    };


    CConfigHandler();

    static CConfigHandler &getSingleton() {
        static CConfigHandler m_Instance;
        return m_Instance;
    }

    bool loadConfig(QString filename);
    bool saveConfig(QString filename);

    QString getValue(qint32 config_type, QString value_name);
private:
    QMap<QString, QString>  m_ConfigCTP;
    QMap<QString, QString>  m_ConfigSimnow;
    QMap<QString, QString>  m_ConfigUserinfo;
};

#endif // CCONFIGHANDLER_H
