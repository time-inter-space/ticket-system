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

struct realtime {
    int month, day, totDays;
    int hour, minute, totMins;
    void calcTotDays() {
        totDays = day - 1;
        if (month >= 7) totDays += 30;
        if (month >= 8) totDays += 31; 
    }
    void calcTotMins() {
        calcTotDays();
        totMins = totDays * 1440 + hour * 60 + minute;
    }
    realtime() {
        month = 6;
        day = 1;
        hour = 0;
        minute = 0;
        calcTotMins();
    }
    realtime(const realtime &other) {
        month = other.month;
        day = other.day;
        hour = other.hour;
        minute = other.minute;
        calcTotMins();
    }
    realtime(int _month, int _day, int _hour, int _minute) {
        month = _month;
        day = _day;
        hour = _hour;
        minute = _minute;
        calcTotMins();
    }
};

void readOp(char &op) {
    while ((op = getchar()) != '-') {
        if (op == '\n') return;
    }
    op = getchar();
}

int main() {
    freopen("testcases\\basic_1\\1.in", "r", stdin);
    freopen("a.out", "w", stdout);
    bool isFirstUser, isFirstTrain;
    BPT <string, string> users("user", 21, 80, isFirstUser);
    BPT <string, int> trainaddr("trainaddr", 21, 4, isFirstTrain);
    int trainNum;
    std::fstream trainIO("train", std::ios::in | std::ios::out | std::ios::binary);
    if (!trainIO) {
        trainNum = 0;
        std::ofstream tmp("train");
        tmp.close();
        trainIO.open("train", std::ios::in | std::ios::out | std::ios::binary);
    }
    else {
        trainIO.seekg(0);
        trainIO.read(reinterpret_cast<char *>(&trainNum), 4);
    }
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
            isFirstUser = 0;
            users.insert(username, info);
            puts("0");
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
            if (tmp2.first == -1 || (username != cur_username && tmp2.second.ch[78] >= tmp1.second.ch[78])) {
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
            if (tmp2.first == -1 || (username != cur_username && tmp2.second.ch[78] >= tmp1.second.ch[78])) {
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
        if (Cmd == "add_train") {
            string trainID;
            int stationNum, seatNum, h, m, bmon, bday, emon, eday;
            char type;
            char stations[3100], prices[700], travelTimes[600], stopoverTimes[600];
            char info[Block];
            while (1) {
                readOp(op);
                if (op == 'i') scanf("%s", trainID.ch);
                if (op == 'n') scanf("%d", &stationNum);
                if (op == 'm') scanf("%d", &seatNum);
                if (op == 's') scanf("%s", stations);
                if (op == 'p') scanf("%s", prices);
                if (op == 'x') scanf("%d:%d", &h, &m);
                if (op == 't') scanf("%s", travelTimes);
                if (op == 'o') scanf("%s", stopoverTimes);
                if (op == 'd') scanf("%d-%d|%d-%d", &bmon, &bday, &emon, &eday);
                if (op == 'y') {
                    type = getchar();
                    while (type < 'A' || type > 'Z') type = getchar();
                }
                if (op == '\n') break;
            }
            printf("[%d] ", timestamp);
            pair <int, int> tmp = trainaddr.find(trainID);
            if (tmp.first != -1) {
                puts("-1");
                continue;
            }
            trainaddr.insert(trainID, ++trainNum);
            memcpy(info, trainID.ch, 21);
            info[21] = stationNum;
            memcpy(info + 22, reinterpret_cast<const char *>(&seatNum), 4);
            info[26] = h;
            info[27] = m;
            info[28] = bmon;
            info[29] = bday;
            info[30] = emon;
            info[31] = eday;
            info[32] = type;
            info[33] = 0;
            int len = strlen(stations);
            for (int i = 0, j = 0, cur = 34; i <= len; ++i) {
                if (stations[i] == '|' || !stations[i]) {
                    info[cur] = 0;
                    ++j;
                    cur = 33 + j * 31;
                    continue;
                }
                info[cur++] = stations[i];
            }
            len = strlen(prices);
            for (int i = 0, cur = 3134, tmp = 0; i <= len; ++i) {
                if (prices[i] == '|' || !prices[i]) {
                    memcpy(info + cur, reinterpret_cast<const char *>(&tmp), 4);
                    cur += 4;
                    tmp = 0;
                    continue;
                }
                tmp = tmp * 10 + prices[i] - '0';
            }
            len = strlen(travelTimes);
            for (int i = 0, cur = 3534, tmp = 0; i <= len; ++i) {
                if (travelTimes[i] == '|' || !travelTimes[i]) {
                    memcpy(info + cur, reinterpret_cast<const char *>(&tmp), 2);
                    cur += 2;
                    tmp = 0;
                    continue;
                }
                tmp = tmp * 10 + travelTimes[i] - '0';
            }
            len = strlen(stopoverTimes);
            for (int i = 0, cur = 3734, tmp = 0; i <= len; ++i) {
                if (stopoverTimes[i] == '|' || !stopoverTimes[i]) {
                    memcpy(info + cur, reinterpret_cast<const char *>(&tmp), 2);
                    cur += 2;
                    tmp = 0;
                    continue;
                }
                tmp = tmp * 10 + travelTimes[i] - '0';
            }
            trainIO.seekp(trainNum * Block);
            trainIO.write(info, Block);
            puts("0");
            continue;
        }
        if (Cmd == "delete_train") {
            string trainID;
            while (1) {
                readOp(op);
                if (op == 'i') scanf("%s", trainID.ch);
                if (op == '\n') break;
            }
            printf("[%d] ", timestamp);
            pair <int, int> tmp = trainaddr.find(trainID);
            if (tmp == -1) {
                puts("-1");
                continue;
            }
            int isReleased;
            trainIO.seekg(tmp.second * Block + 33);
            trainIO.read(reinterpret_cast<char *>(&isReleased), 1);
            if (isReleased) {
                puts("-1");
                continue;
            }
            trainaddr.erase(trainID);
            puts("0");
            continue;
        }
        if (Cmd == "release_train") {
            string trainID;
            while (1) {
                readOp(op);
                if (op == 'i') scanf("%s", trainID.ch);
                if (op == '\n') break;
            }
            printf("[%d] ", timestamp);
            pair <int, int> tmp = trainaddr.find(trainID);
            if (tmp == -1) {
                puts("-1");
                continue;
            }
            char info[Block];
            trainIO.seekg(tmp.second * Block);
            trainIO.read(info, Block);
            if (info[33]) {
                puts("-1");
                continue;
            }
            info[33] = 1;
            trainIO.seekp(tmp.second * Block + 33);
            trainIO.write(info + 33, 1);
        }
        if (Cmd == "query_train") {
            
        }
        if (Cmd == "exit") {
            int tmp = 0;
            users.modifyall(79, reinterpret_cast<const char *>(&tmp), 1);
            trainIO.seekp(0);
            trainIO.write(reinterpret_cast<const char *>(&trainNum), 4);
            printf("[%d] bye\n", timestamp);
            return 0;
        }
    }
}