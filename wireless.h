#ifndef WIRELESS_H
#define WIRELESS_H

#include <stdint.h>

struct ieee80211_radiotap_header {
    uint8_t it_version;
    uint8_t it_pad;
    uint16_t it_len;
    uint32_t it_present;
};

struct ieee80211_deauthentication_frame {
    uint16_t type;
    uint16_t duration;
    uint8_t recv_addr[6];
    uint8_t send_addr[6];
    uint8_t bssid[6];
    uint16_t frag_seq_num;
    uint16_t reason_code;
};

#endif // WIRELESS_H
