#ifndef CEVENT_H
#define CEVENT_H

#include <QObject>

namespace xq_event {

    enum {
        NONE = 0,

    };

}



class CEvent {
public:
    CEvent();
private:
    qint8 m_Type;
};


#endif // CEVENT_H
