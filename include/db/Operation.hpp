#pragma once

#include <QString>

enum class Operation {
    VerifyState,
    AddInformation,
    AddYoutubeAction,
    GetInformation,
    GetAllInformation,
    GetWeights,
    UpdateInformation,
    DeleteInformation,
    DeleteAllInformation,
    DeleteUselessActions,
    Unknown
};

inline Operation parseOperation(const QString& op) {
    if (op.compare("VerifyState",          Qt::CaseInsensitive) == 0) return Operation::VerifyState;
    if (op.compare("AddInformation",       Qt::CaseInsensitive) == 0) return Operation::AddInformation;
    if (op.compare("AddYoutubeAction",     Qt::CaseInsensitive) == 0) return Operation::AddYoutubeAction;
    if (op.compare("GetInformation",       Qt::CaseInsensitive) == 0) return Operation::GetInformation;
    if (op.compare("GetAllInformation",    Qt::CaseInsensitive) == 0) return Operation::GetAllInformation;
    if (op.compare("GetWeights",           Qt::CaseInsensitive) == 0) return Operation::GetWeights;
    if (op.compare("UpdateInformation",    Qt::CaseInsensitive) == 0) return Operation::UpdateInformation;
    if (op.compare("DeleteInformation",    Qt::CaseInsensitive) == 0) return Operation::DeleteInformation;
    if (op.compare("DeleteAllInformation", Qt::CaseInsensitive) == 0) return Operation::DeleteAllInformation;
    if (op.compare("DeleteUselessActions", Qt::CaseInsensitive) == 0) return Operation::DeleteUselessActions;

    return Operation::Unknown;
}

inline QString opToString(const Operation& op) {
    switch (op) {
        case Operation::VerifyState:          return "VerifyState";
        case Operation::AddInformation:       return "AddInformation";
        case Operation::AddYoutubeAction:     return "AddYoutubeAction";
        case Operation::GetInformation:       return "GetInformation";
        case Operation::GetAllInformation:    return "GetAllInformation";
        case Operation::GetWeights:           return "GetWeights";
        case Operation::UpdateInformation:    return "UpdateInformation";
        case Operation::DeleteInformation:    return "DeleteInformation";
        case Operation::DeleteAllInformation: return "DeleteAllInformation";
        case Operation::DeleteUselessActions: return "DeleteUselessActions";
        default:                              return "Unknown";
    }
}
