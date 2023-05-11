#include <iostream>
#include <cstdio>
#include <cmath>
#include <cstring>
#include <string>
#include <fstream>
#include "BPT.hpp"

const int StrLen = 80;
struct string {
    char ch[StrLen];
    string() {
        ch[0] = 0;
    }
    string(const string &other) {
        for (int i = 0; i < StrLen; ++i) ch[i] = other.ch[i];
    }
    string(char other[]) {
        for (int i = 0; i < StrLen; ++i) ch[i] = other[i];
    }
    string &operator=(const string &rhs) {
        for (int i = 0; i < StrLen; ++i) ch[i] = rhs.ch[i];
        return *this;
    }
    bool operator<(const string &rhs) const {
        for (int i = 0; i < StrLen && (ch[i] || rhs.ch[i]); ++i)
            if (ch[i] != rhs.ch[i]) return ch[i] < rhs.ch[i];
        return 0;
    }
    bool operator==(const string &rhs) const {
        for (int i = 0; i < StrLen && (ch[i] || rhs.ch[i]); ++i)
            if (ch[i] != rhs.ch[i]) return 0;
        return 1;
    }
    bool operator!=(const string &rhs) const {
        for (int i = 0; i < StrLen && (ch[i] || rhs.ch[i]); ++i)
            if (ch[i] != rhs.ch[i]) return 1;
        return 0;
    }
};

void readOp(char &op) {
    while ((op = getchar()) != '-') {
        if (op == '\n') return;
    }
    op = getchar();
}

bool isFirstUser;
BPT <string, string> users("user", 21, 80, isFirstUser);

int main() {
    while (1) {
        int timestamp;
        char cmd[20];
        char op;
        scanf("[%d] %s", &timestamp, cmd);
        std::string Cmd;
        for (int i = 0; cmd[i]; i++) Cmd += cmd[i];
        if (Cmd == "add_user") {
            string cur_username, username, info;
            while (1) {
                readOp(op);
                if (op == 'c') scanf("%s", cur_username.ch);
                if (op == 'u') scanf("%s", username.ch);
                if (op == 'p') scanf("%s", info.ch);
                if (op == 'n') scanf("%s", info.ch + 31);
                if (op == 'm') scanf("%s", info.ch + 47);
                if (op == 'g') {
                    int tmp;
                    scanf("%d", &tmp);
                    if (isFirstUser) tmp = 10;
                    info.ch[78] = tmp;
                }
                if (op == '\n') break;
            }
            printf("[%d] ", timestamp);
            info.ch[79] = 0;
            if (!isFirstUser) {
                pair <int, string> tmp = users.find(cur_username);
                if (tmp.first == -1 || !tmp.second.ch[79] || info.ch[78] >= tmp.second.ch[78]) {
                    puts("-1");
                    continue;
                }
                tmp = users.find(username);
                if (tmp.first != -1) {
                    puts("-1");
                    continue;
                }
            }
            users.insert(username, info);
            puts("0");
            isFirstUser = 0;
            continue;
        }
        if (Cmd == "login") {
            string username, password;
            while (1) {
                readOp(op);
                if (op == 'u') scanf("%s", username.ch);
                if (op == 'p') scanf("%s", password.ch);
                if (op == '\n') break;
            }
            printf("[%d] ", timestamp);
            pair <int, string> tmp = users.find(username);
            if (tmp.first == -1 || tmp.second.ch[79] || tmp.second != password) {
                puts("-1");
                continue;
            }
            tmp.second.ch[79] = 1;
            users.modify(tmp.first + 79, tmp.second.ch + 79, 1);
            puts("0");
            continue;
        }
        if (Cmd == "logout") {
            string username;
            while (1) {
                readOp(op);
                if (op == 'u') scanf("%s", username.ch);
                if (op == '\n') break;
            }
            printf("[%d] ", timestamp);
            pair <int, string> tmp = users.find(username);
            if (tmp.first == -1 || !tmp.second.ch[79]) {
                puts("-1");
                continue;
            }
            tmp.second.ch[79] = 0;
            users.modify(tmp.first + 79, tmp.second.ch + 79, 1);
            puts("0");
            continue;
        }
        if (Cmd == "query_profile") {
            string cur_username, username;
            while (1) {
                readOp(op);
                if (op == 'c') scanf("%s", cur_username.ch);
                if (op == 'u') scanf("%s", username.ch);
                if (op == '\n') break;
            }
            printf("[%d] ", timestamp);
            pair <int, string> tmp1 = users.find(cur_username);
            if (tmp1.first == -1 || !tmp1.second.ch[79]) {
                puts("-1");
                continue;
            }
            pair <int, string> tmp2 = users.find(username);
            if (tmp2.first == -1 || tmp2.second.ch[78] > tmp1.second.ch[78]) {
                puts("-1");
                continue;
            }
            printf("%s %s %s %d\n", username.ch, tmp2.second.ch + 31, tmp2.second.ch + 47, tmp2.second.ch[78]);
            continue;
        }
        if (Cmd == "modify_profile") {
            string cur_username, username, info;
            info.ch[0] = info.ch[31] = info.ch[47] = 0;
            info.ch[78] = -1;
            while (1) {
                readOp(op);
                if (op == 'c') scanf("%s", cur_username.ch);
                if (op == 'u') scanf("%s", username.ch);
                if (op == 'p') scanf("%s", info.ch);
                if (op == 'n') scanf("%s", info.ch + 31);
                if (op == 'm') scanf("%s", info.ch + 47);
                if (op == 'g') {
                    int tmp;
                    scanf("%d", &tmp);
                    info.ch[78] = tmp;
                }
                if (op == '\n') break;
            }
            printf("[%d] ", timestamp);
            pair <int, string> tmp1 = users.find(cur_username);
            if (tmp1.first == -1 || !tmp1.second.ch[79] || info.ch[78] >= tmp1.second.ch[78]) {
                puts("-1");
                continue;
            }
            pair <int, string> tmp2 = users.find(username);
            if (tmp2.first == -1 || tmp2.second.ch[78] > tmp1.second.ch[78]) {
                puts("-1");
                continue;
            }
            if (info.ch[0]) memcpy(tmp2.second.ch, info.ch, 31);
            if (info.ch[31]) memcpy(tmp2.second.ch + 31, info.ch + 31, 16);
            if (info.ch[47]) memcpy(tmp2.second.ch + 47, info.ch + 47, 31);
            if (info.ch[78] != -1) tmp2.second.ch[78] = info.ch[78];
            users.modify(tmp2.first, tmp2.second.ch, 80);
            printf("%s %s %s %d\n", username.ch, tmp2.second.ch + 31, tmp2.second.ch + 47, tmp2.second.ch[78]);
            continue;
        }
        if (Cmd == "exit") {
            printf("[%d] bye\n", timestamp);
            return 0;
        }
    }
}