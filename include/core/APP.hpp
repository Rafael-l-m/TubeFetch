#pragma once

#include "MessageCenter.hpp"
#include "QCoreApplication"

namespace APP {
    inline MessageCenter* messageCenter() {
        return qApp->findChild<MessageCenter*>();
    }
}
