#ifndef FRAME_H
#define FRAME_H

#include "wireless.h"

#pragma pack(1)
struct Frame
{
    struct ieee80211_radiotap_header radio;
    struct ieee80211_deauthentication_frame deauth;

    Frame();
    void pack_ap(char* addr);
    void pack_station(char* addr);
};
#pragma pack()

#endif // FRAME_H
