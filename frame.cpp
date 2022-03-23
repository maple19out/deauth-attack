#include "frame.h"

static int hex(char c) {
    int ret;

    switch(c) {
    case 'a':
    case 'A':
        ret = 0xa;
        break;

    case 'b':
    case 'B':
        ret = 0xb;
        break;

    case 'c':
    case 'C':
        ret = 0xc;
        break;

    case 'd':
    case 'D':
        ret = 0xd;
        break;

    case 'e':
    case 'E':
        ret = 0xe;
        break;

    case 'f':
    case 'F':
        ret = 0xf;
        break;

    default:
        ret = c - '0';
        break;
    }

    return ret;
}

Frame::Frame()
{
    radio.it_len = radio.it_pad = radio.it_present = radio.it_version = 0;
    deauth.duration = deauth.frag_seq_num = deauth.reason_code = 0;
    deauth.type = 0x00c0;
    deauth.reason_code = 0x0007;
}

void Frame::pack_ap(char *addr)
{
    for (int i=0; i<6; i++) {
        uint8_t value = 0;
        value += hex(addr[3*i]) << 4;
        value |= hex(addr[3*i+1]);
        this->deauth.bssid[i] = this->deauth.send_addr[i] = value;
    }
}

void Frame::pack_station(char *addr)
{
    for (int i=0; i<6; i++) {
        uint8_t value = 0;
        value += hex(addr[3*i]) << 4;
        value |= hex(addr[3*i+1]);
        this->deauth.recv_addr[i] = value;
    }
}
