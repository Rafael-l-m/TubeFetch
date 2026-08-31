#pragma once

#include <QObject>

enum class YoutubeActionsType {
    GET_INFO,
    SHOW_INFO,
    DOWNLOAD_INFO
};

inline int youtubeActionsTypeToInt(const YoutubeActionsType& youtubeActionsType) {
    switch (youtubeActionsType) {
        case YoutubeActionsType::GET_INFO:      return 0;
        case YoutubeActionsType::SHOW_INFO:     return 1;
        case YoutubeActionsType::DOWNLOAD_INFO: return 2;
        default:                                return 0;
    }
}

inline YoutubeActionsType parseYoutubeActionsType(const int value) {
    switch (value) {
        case 0:  return YoutubeActionsType::GET_INFO;
        case 1:  return YoutubeActionsType::SHOW_INFO;
        case 2:  return YoutubeActionsType::DOWNLOAD_INFO;
        default: return YoutubeActionsType::GET_INFO;
    }
}
