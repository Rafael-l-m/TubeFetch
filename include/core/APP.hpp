#pragma once

#include <core/MessageCenter.hpp>
#include <QCoreApplication>

namespace APP {
    inline MessageCenter* messageCenter() { return qApp->findChild<MessageCenter*>(); }
}
