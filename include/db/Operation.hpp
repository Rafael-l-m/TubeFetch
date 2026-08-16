#pragma once

#include <QString>

enum class Operation {
    VerifyState,
    AddInformation,
    GetInformation,
    GetAllInformation,
    UpdateInformation,
    DeleteInformation,
    DeleteAllInformation,
    Unknown
};

inline Operation parseOperation(const QString& op) {
    if (op.compare("VerifyState",          Qt::CaseInsensitive) == 0) return Operation::VerifyState;
    if (op.compare("AddInformation",       Qt::CaseInsensitive) == 0) return Operation::AddInformation;
    if (op.compare("GetInformation",       Qt::CaseInsensitive) == 0) return Operation::GetInformation;
    if (op.compare("GetAllInformation",    Qt::CaseInsensitive) == 0) return Operation::GetAllInformation;
    if (op.compare("UpdateInformation",    Qt::CaseInsensitive) == 0) return Operation::UpdateInformation;
    if (op.compare("DeleteInformation",    Qt::CaseInsensitive) == 0) return Operation::DeleteInformation;
    if (op.compare("DeleteAllInformation", Qt::CaseInsensitive) == 0) return Operation::DeleteAllInformation;

    return Operation::Unknown;
}

inline QString opToString(const Operation& op) {
    switch (op) {
        case Operation::VerifyState:          return "VerifyState";
        case Operation::AddInformation:       return "AddInformation";
        case Operation::GetInformation:       return "GetInformation";
        case Operation::GetAllInformation:    return "GetAllInformation";
        case Operation::UpdateInformation:    return "UpdateInformation";
        case Operation::DeleteInformation:    return "DeleteInformation";
        case Operation::DeleteAllInformation: return "DeleteAllInformation";
        default:                              return "Unknown";
    }
}
