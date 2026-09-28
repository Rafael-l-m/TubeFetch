#pragma once

#include <QString>

enum class Operation {
    VerifyState,
    AddInformation,
    AddUrlInformation,
    GetInformation,
    GetAllInformation,
    GetUrlInformation,
    UpdateInformation,
    UpdateUrlInformation,
    DeleteInformation,
    DeleteAllInformation,
    DeleteUselessUrlInformation,
    DeleteAllDownloadUrlInformation,
    Unknown
};

inline Operation parseOperation(const QString& op) {
    if (op.compare("VerifyState",                     Qt::CaseInsensitive) == 0) return Operation::VerifyState;
    if (op.compare("AddInformation",                  Qt::CaseInsensitive) == 0) return Operation::AddInformation;
    if (op.compare("AddUrlInformation",               Qt::CaseInsensitive) == 0) return Operation::AddUrlInformation;
    if (op.compare("GetInformation",                  Qt::CaseInsensitive) == 0) return Operation::GetInformation;
    if (op.compare("GetAllInformation",               Qt::CaseInsensitive) == 0) return Operation::GetAllInformation;
    if (op.compare("GetUrlInformation",               Qt::CaseInsensitive) == 0) return Operation::GetUrlInformation;
    if (op.compare("UpdateInformation",               Qt::CaseInsensitive) == 0) return Operation::UpdateInformation;
    if (op.compare("UpdateUrlInformation",            Qt::CaseInsensitive) == 0) return Operation::UpdateUrlInformation;
    if (op.compare("DeleteInformation",               Qt::CaseInsensitive) == 0) return Operation::DeleteInformation;
    if (op.compare("DeleteAllInformation",            Qt::CaseInsensitive) == 0) return Operation::DeleteAllInformation;
    if (op.compare("DeleteUselessUrlInformation",     Qt::CaseInsensitive) == 0) return Operation::DeleteUselessUrlInformation;
    if (op.compare("DeleteAllDownloadUrlInformation", Qt::CaseInsensitive) == 0) return Operation::DeleteAllDownloadUrlInformation;

    return Operation::Unknown;
}

inline QString opToString(const Operation& op) {
    switch (op) {
        case Operation::VerifyState:                     return "VerifyState";
        case Operation::AddInformation:                  return "AddInformation";
        case Operation::AddUrlInformation:               return "AddUrlInformation";
        case Operation::GetInformation:                  return "GetInformation";
        case Operation::GetAllInformation:               return "GetAllInformation";
        case Operation::GetUrlInformation:               return "GetUrlInformation";
        case Operation::UpdateInformation:               return "UpdateInformation";
        case Operation::UpdateUrlInformation:            return "UpdateUrlInformation";
        case Operation::DeleteInformation:               return "DeleteInformation";
        case Operation::DeleteAllInformation:            return "DeleteAllInformation";
        case Operation::DeleteUselessUrlInformation:     return "DeleteUselessUrlInformation";
        case Operation::DeleteAllDownloadUrlInformation: return "DeleteAllDownloadUrlInformation";
        default:                                         return "Unknown";
    }
}
