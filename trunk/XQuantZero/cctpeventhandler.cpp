#include "cctpeventhandler.h"

CCTPEventHandler::CCTPEventHandler() {

}

void CCTPEventHandler::run() {
    processList();
}

void CCTPEventHandler::processList(void) {
    while (!m_List.empty()) {
        // process m_List items;
    }
}
