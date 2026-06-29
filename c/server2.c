/* socket/server2.c */
#include <arpa/inet.h>  /* htons */
#include <stdio.h>      /* fprintf, perror */
#include <stdlib.h>     /* exit */
#include <string.h>     /* memset, strlen */
#include <sys/types.h>  /* accept, bind, setsockopt, socket, write */
#include <sys/socket.h> /* accept, bind, listen, setsockopt, socket */
#include <sys/uio.h>    /* write */
#include <unistd.h>     /* close, write */

#define SERVER_ADDR     "127.0.0.1"
#define SERVER_PORT     12345

enum {
    NQUEUESIZE = 5, /* listen のための待ち行列枠数 */
};

int main(void) {
    int s, ws, soval;
    struct sockaddr_in sa, ca;
    socklen_t ca_len;

    /* ソケットを作る */
    if ((s = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    /* アドレス再利用の設定 */
    soval = 1;
    if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &soval, sizeof(soval)) == -1) {
        perror("setsockopt");
        exit(1);
    }

    /* ソケットに名前を付ける */
    memset(&sa, 0, sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_port = htons(SERVER_PORT);
    sa.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(s, (struct sockaddr *) &sa, sizeof(sa)) == -1) {
        perror("bind");
        exit(1);
    }

    /* 待ち行列を準備 */
    if (listen(s, NQUEUESIZE)) {
        perror("listen");
        exit(1);
    }

    fprintf(stderr, "Ready.\n");

    for (;;) {
        char buf[1024];
        ca_len = sizeof(ca);

        /* 接続を受け入れる */
        fprintf(stderr, "Waiting for a connection...\n");
        if ((ws = accept(s, (struct sockaddr *)&ca, &ca_len)) == -1) {
            perror("accept");
            exit(1);
        }
        fprintf(stderr, "Connection established.\n");

        /* クライアント情報を表示 */
        char *client_ip = inet_ntoa(ca.sin_addr);
        int client_port = ntohs(ca.sin_port);
        fprintf(stderr, "Client connected: IP=%s, Port=%d\n", client_ip, client_port);

        /* サーバ側の入力をクライアントに送信 */
        for (;;) {
            fprintf(stderr, "Enter message to send (Ctrl+D to finish):\n");
            if (fgets(buf, sizeof(buf), stdin) == NULL) {
                fprintf(stderr, "End of input.\n");
                break;  // 入力終了
            }
            if (write(ws, buf, strlen(buf)) == -1) {
                perror("write");
                exit(1);
            }
        }

        /* 通信を止める */
        if (shutdown(ws, SHUT_RDWR) == -1) {
            perror("shutdown");
            exit(1);
        }

        /* 接続済みソケットを閉じる */
        if (close(ws) == -1) {
            perror("close");
            exit(1);
        }

        fprintf(stderr, "Connection closed.\n");
        /* 次の要求の受け入れへ */
    }
    /* 無限ループ */
}
