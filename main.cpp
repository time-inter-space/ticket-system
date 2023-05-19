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

struct date {
    int month, day, totDays;
    void calcTotDays() {
        totDays = day - 1;
        if (month > 6) totDays += 30;
        if (month > 7) totDays += 31;
        if (month > 8) totDays += 31; 
    }
    date() {
        month = 6;
        day = 1;
        totDays = 0;
    }
    date(const date &other) {
        month = other.month;
        day = other.day;
        calcTotDays();
    }
    date(int _month, int _day) : month(_month), day(_day) { calcTotDays(); }
    bool operator<(const date &rhs) const { return totDays < rhs.totDays; }
    bool operator>(const date &rhs) const { return totDays > rhs.totDays; }
    int operator-(const date &rhs) const { return totDays - rhs.totDays; }
    date &operator+=(int dd) {
        totDays += dd;
        int tmp = totDays;
        month = 6;
        if (tmp >= 30) {
            tmp -= 30;
            month = 7;
            if (tmp >= 31) {
                tmp -= 31;
                month = 8;
                if (tmp >= 31) {
                    tmp -= 31;
                    month = 9;
                }
            }
        }
        day = tmp + 1;
    }
    void print() { printf("%d%d-%d%d", month / 10, month % 10, day / 10, day % 10); }
};
struct realtime {
    date d;
    int hour, minute, totMins;
    void calcTotMins() { totMins = d.totDays * 1440 + hour * 60 + minute; }
    realtime() : d() {
        hour = 0;
        minute = 0;
        totMins = 0;
    }
    realtime(const realtime &other) : d(other.d) {
        hour = other.hour;
        minute = other.minute;
        calcTotMins();
    }
    realtime(const date &_d, int _hour, int _minute)
        : d(_d), hour(_hour), minute(_minute) { calcTotMins(); }
    realtime(int _month, int _day, int _hour, int _minute)
        : d(_month, _day), hour(_hour), minute(_minute) { calcTotMins(); }
    realtime &operator+=(int dmin) {
        minute += dmin;
        if (minute >= 60) {
            hour += minute / 60;
            minute %= 60;
        }
        if (hour >= 24) {
            d += hour / 24;
            hour %=24;
        }
        totMins += dmin;
        return *this;
    }
    void print() {
        d.print();
        printf(" %d%d:%d%d", hour / 10, hour % 10, minute / 10, minute % 10);
    }
};

void readOp(char &op) {
    while ((op = getchar()) != '-') {
        if (op == '\n') return;
    }
    op = getchar();
}

const int BlockInfo = 4096, BlockTicketNum = 512;

int main() {
    //freopen("testcases\\basic_1\\1.in", "r", stdin);
    //freopen("a.out", "w", stdout);
    bool isFirstUser, isFirstTrain;
    BPT <string, string> users("user", 21, 80, isFirstUser);
    BPT <string, int> trainaddr("trainaddr", 21, 4, isFirstTrain);
    int trainNum;
    std::fstream trainIO("traininfo", std::ios::in | std::ios::out | std::ios::binary);
    if (!trainIO) {
        trainNum = 0;
        std::ofstream tmp("traininfo");
        tmp.close();
        trainIO.open("traininfo", std::ios::in | std::ios::out | std::ios::binary);
    }
    else {
        trainIO.seekg(0);
        trainIO.read(reinterpret_cast<char *>(&trainNum), 4);
    }
    int traindateNum;
    std::fstream ticketNumIO("ticketnum", std::ios::in | std::ios::out | std::ios::binary);
    if (!ticketNumIO) {
        traindateNum = 0;
        std::ofstream tmp("ticketnum");
        tmp.close();
        ticketNumIO.open("ticketnum", std::ios::in | std::ios::out | std::ios::binary);
    }
    else {
        ticketNumIO.seekg(0);
        ticketNumIO.read(reinterpret_cast<char *>(&traindateNum), 4);
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
            char info[BlockInfo];
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
                    cur = 34 + j * 31;
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
                tmp = tmp * 10 + stopoverTimes[i] - '0';
            }
            trainIO.seekp(trainNum * BlockInfo);
            trainIO.write(info, BlockInfo);
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
            if (tmp.first == -1) {
                puts("-1");
                continue;
            }
            int isReleased;
            trainIO.seekg(tmp.second * BlockInfo + 33);
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
            if (tmp.first == -1) {
                puts("-1");
                continue;
            }
            char info[BlockInfo];
            trainIO.seekg(tmp.second * BlockInfo);
            trainIO.read(info, BlockInfo);
            if (info[33]) {
                puts("-1");
                continue;
            }
            info[33] = 1;
            memcpy(info + 3933, reinterpret_cast<const char *>(&traindateNum), 4);
            trainIO.seekp(tmp.second * BlockInfo);
            trainIO.write(info, BlockInfo);
            date bd(info[28], info[29]), ed(info[30], info[31]);
            int dday = ed - bd;
            char restSeats[BlockTicketNum];
            for (int i = 0; i < info[21] - 1; ++i) memcpy(restSeats + i * 4, info + 22, 4);
            for (int i = 1; i <= dday; ++i) {
                ticketNumIO.seekp((traindateNum + i) * BlockTicketNum);
                ticketNumIO.write(restSeats, BlockTicketNum);
            }
            traindateNum += dday;
            puts("0");
            continue;
        }
        if (Cmd == "query_train") {
            string trainID;
            date d;
            while (1) {
                readOp(op);
                if (op == 'i') scanf("%s", trainID.ch);
                if (op == 'd') {
                    scanf("%d-%d", &d.month, &d.day);
                    d.calcTotDays();
                }
                if (op == '\n') break;
            }
            printf("[%d] ", timestamp);
            pair <int, int> tmp = trainaddr.find(trainID);
            if (tmp.first == -1) {
                puts("-1");
                continue;
            }
            char info[BlockInfo];
            trainIO.seekg(tmp.second * BlockInfo);
            trainIO.read(info, BlockInfo);
            date bd(info[28], info[29]), ed(info[30], info[31]);
            if (d < bd || d > ed) {
                puts("-1");
                continue;
            }
            printf("%s %c\n", info, info[32]);
            int restSeats[100];
            if (!info[33]) {
                memcpy(reinterpret_cast<char *>(restSeats), info + 22, 4);
                for (int i = 1; i < info[21] - 1; ++i) restSeats[i] = restSeats[0];
            }
            else {
                int ticketNumAddr;
                memcpy(reinterpret_cast<char *>(&ticketNumAddr), info + 3933, 4);
                ticketNumAddr += d - bd;
                ticketNumIO.seekg(ticketNumAddr * BlockTicketNum);
                ticketNumIO.read(reinterpret_cast<char *>(restSeats), (info[21] - 1) * 4);
            }
            int curPrice = 0;
            realtime curTime(d, info[26], info[27]);
            for (int i = 0; i < info[21]; ++i) {
                printf("%s ", info + 34 + i * 31);
                if (!i) printf("xx-xx xx:xx");
                else {
                    int dprice, dmin;
                    memcpy(reinterpret_cast<char *>(&dprice), info + 3134 + (i - 1) * 4, 4);
                    memcpy(reinterpret_cast<char *>(&dmin), info + 3534 + (i - 1) * 2, 2);
                    curPrice += dprice;
                    curTime += dmin;
                    curTime.print();
                }
                printf(" -> ");
                if (i == info[21] - 1) printf("xx-xx xx:xx");
                else {
                    if (i) {
                        int dmin;
                        memcpy(reinterpret_cast<char *>(&dmin), info + 3734 + (i - 1) * 4, 2);
                        curTime += dmin;
                    }
                    curTime.print();
                }
                printf(" %d ", curPrice);
                if (i == info[21] - 1) puts("x");
                else printf("%d\n", restSeats[i]);
            }
            continue;
        }
        if (Cmd == "query_ticket") {
            string sstation, tstation;
            date d;
            int p = 1;
            while (1) {
                readOp(op);
                if (op == 's') scanf("%s", sstation.ch);
                if (op == 't') scanf("%s", tstation.ch);
                if (op == 'd') {
                    scanf("%d-%d", &d.month, &d.day);
                    d.calcTotDays();
                }
                if (op == 'p') {
                    char tmp[5];
                    scanf("%s", tmp);
                    if (tmp[0] == 'c') p = 0;
                }
                if (op == '\n') break;
            }
            
        }
        if (Cmd == "exit") {
            int tmp = 0;
            users.modifyall(79, reinterpret_cast<const char *>(&tmp), 1);
            trainIO.seekp(0);
            trainIO.write(reinterpret_cast<const char *>(&trainNum), 4);
            trainIO.close();
            ticketNumIO.seekp(0);
            ticketNumIO.write(reinterpret_cast<const char *>(&traindateNum), 4);
            ticketNumIO.close();
            printf("[%d] bye\n", timestamp);
            return 0;
        }
    }
}