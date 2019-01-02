#include "cconfighandler.h"

CConfigHandler::CConfigHandler()
{

}

bool CConfigHandler::loadConfig(QString filename) {
    QFile loadFile(filename);

    if(!loadFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "could't open config json";
        return false;
    }

    QByteArray allData = loadFile.readAll();
    loadFile.close();

    QJsonParseError json_error;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(allData, &json_error));

    if(json_error.error != QJsonParseError::NoError)
    {
        qDebug() << "json error!";
        return false;
    }

    QJsonObject rootObj = jsonDoc.object();

    qint32 i, j;

    for (i = 0;i < sections_count;i++) {
        if (rootObj.contains( sections[i] )) {

            QJsonObject subObj = rootObj.value( sections[i] ).toObject();

            if (sections[i] == "simnow_server") {
                for (j = 0;j < count_simnow_server;j++) {
                    QString value = subObj[ tags_simnow_server[j] ].toString();
                    qDebug() << tags_simnow_server[j] << " is:" << value;
                    m_ConfigSimnow[ tags_simnow_server[j] ] = value;
                }
                continue;
                //find match continue ## ÕÒµ½Æ¥ÅäÅäÖÃ£¬¼ÌÐøÑ­»·
            }

            if (sections[i] == "ctp_server") {
                for (j = 0;j < count_simnow_server;j++) {
                    QString value = subObj[ tags_ctp_server[j] ].toString();
                    qDebug() << tags_ctp_server[j] << " is:" << value;
                    m_ConfigCTP[ tags_ctp_server[j] ] = value;
                }
                continue;
                //find match continue ## ÕÒµ½Æ¥ÅäÅäÖÃ£¬¼ÌÐøÑ­»·
            }

            if (sections[i] == "userinfo") {
                for (j = 0;j < count_userinfo;j++) {
                    QString value = subObj[ tags_userinfo[j] ].toString();
                    qDebug() << tags_userinfo[j] << " is:" << value;
                    m_ConfigUserinfo[ tags_simnow_server[j] ] = value;
                }
                continue;
                //find match continue ## ÕÒµ½Æ¥ÅäÅäÖÃ£¬¼ÌÐøÑ­»·
            }

        }
    }

    return true;
}

bool CConfigHandler::saveConfig(QString filename) {
    return true;
}

QString CConfigHandler::getValue(qint32 config_type, QString value_name) {
    if (config_type == CONFIG_CTP) {
        return m_ConfigCTP[value_name];
    }

    if (config_type == CONFIG_SIMNOW) {
        return m_ConfigSimnow[value_name];
    }

    if (config_type == CONFIG_USERINFO) {
        return m_ConfigUserinfo[value_name];
    }

    return nullptr;
}
