#include <iostream>
#include <cstdio>
#include <cmath>
#include <cstring>
#include <string>
#include <fstream>
#include "BPT.hpp"

const int inf = 0x3f3f3f3f;
struct date {
    int month, day, totDays;
    void calcTotDays() {
        totDays = day - 1;
        if (month > 6) totDays += 30;
        if (month > 7) totDays += 31;
        if (month > 8) totDays += 31;
        if (month < 6) totDays = -inf;
        if (month > 9) totDays = inf;
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
    void update() {
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
    date &operator+=(int dd) {
        totDays += dd;
        update();
    }
    date &operator-=(int dd) {
        totDays -= dd;
        update();
    }
    void print() { printf("%d%d-%d%d", month / 10, month % 10, day / 10, day % 10); }
};
struct realtime {
    date d;
    int hour, minute, totMins;
    void calcTotMins() {
        d.calcTotDays();
        totMins = d.totDays * 1440 + hour * 60 + minute;
    }
    realtime() : d() {
        hour = 0;
        minute = 0;
        totMins = 0;
    }
    realtime(const realtime &other)
      : d(other.d), hour(other.hour), minute(other.minute) { calcTotMins(); }
    realtime(const date &_d, int _hour, int _minute)
      : d(_d), hour(_hour), minute(_minute) { calcTotMins(); }
    realtime(int _month, int _day, int _hour, int _minute)
      : d(_month, _day), hour(_hour), minute(_minute) { calcTotMins(); }
    realtime &operator+=(int dmin) {
        minute += dmin;
        if (minute >= 60) {
            hour += minute / 60;
            minute %= 60;
            if (hour >= 24) {
                d += hour / 24;
                hour %=24;
            }
        }
        totMins += dmin;
        return *this;
    }
    realtime &operator-=(int dmin) {
        minute -= dmin;
        if (minute < 0) {
            int tar = minute % 60;
            if (tar < 0) tar += 60;
            hour -= (tar - minute) / 60;
            minute = tar;
            if (hour < 0) {
                tar = hour % 24;
                if (tar < 0) tar += 24;
                d -= (tar - hour) / 24;
                hour = tar;
            }
        }
        totMins -= dmin;
        return *this;
    }
    int operator-(const realtime &rhs) const { return totMins - rhs.totMins; }
    void print() {
        d.print();
        printf(" %d%d:%d%d", hour / 10, hour % 10, minute / 10, minute % 10);
    }
};
bool cmpHourMin(const realtime &lhs, const realtime &rhs) {
    if (lhs.hour != rhs.hour) return lhs.hour < rhs.hour;
    return lhs.minute < rhs.minute;
}

const int StrLen = 121;
struct string {
    char ch[StrLen];
    string() {
        ch[0] = 0;
    }
    string(const string &other) {
        memcpy(ch, other.ch, StrLen);
    }
    string(char other[]) {
        memcpy(ch, other, StrLen);
    }
    string &operator=(const string &rhs) {
        memcpy(ch, rhs.ch, StrLen);
        return *this;
    }
    bool operator<(const string &rhs) const {
        for (int i = 0; ch[i] || rhs.ch[i]; ++i)
            if (ch[i] != rhs.ch[i]) return ch[i] < rhs.ch[i];
        return 0;
    }
    bool operator==(const string &rhs) const {
        for (int i = 0; ch[i] || rhs.ch[i]; ++i)
            if (ch[i] != rhs.ch[i]) return 0;
        return 1;
    }
    bool operator!=(const string &rhs) const {
        for (int i = 0; ch[i] || rhs.ch[i]; ++i)
            if (ch[i] != rhs.ch[i]) return 1;
        return 0;
    }
};
struct station_train {
    char ch[52];
    station_train() {
        ch[0] = ch[31] = 0;
    }
    station_train(const station_train &other) {
        memcpy(ch, other.ch, 52);
    }
    station_train(const string &other) {
        memcpy(ch, other.ch, 31);
        ch[31] = 0;
    }
    station_train(const string &other1, const string &other2) {
        memcpy(ch, other1.ch, 31);
        memcpy(ch + 31, other2.ch, 21);
    }
    station_train &operator=(const station_train &rhs) {
        memcpy(ch, rhs.ch, 52);
        return *this;
    }
    bool operator<(const station_train &rhs) const {
        for (int i = 0; ch[i] || rhs.ch[i]; ++i)
            if (ch[i] != rhs.ch[i]) return ch[i] < rhs.ch[i];
        for (int i = 31; ch[i] || rhs.ch[i]; ++i)
            if (ch[i] != rhs.ch[i]) return ch[i] < rhs.ch[i];
        return 0;
    }
    bool operator==(const station_train &rhs) const {
        for (int i = 0; ch[i] || rhs.ch[i]; ++i)
            if (ch[i] != rhs.ch[i]) return 0;
        for (int i = 31; ch[i] || rhs.ch[i]; ++i)
            if (ch[i] != rhs.ch[i]) return 0;
        return 1;
    }
    bool operator!=(const station_train &rhs) const {
        for (int i = 0; ch[i] || rhs.ch[i]; ++i)
            if (ch[i] != rhs.ch[i]) return 1;
        for (int i = 31; ch[i] || rhs.ch[i]; ++i)
            if (ch[i] != rhs.ch[i]) return 1;
        return 0;
    }
};
bool cmpStation(const station_train &lhs, const station_train &rhs) {
    for (int i = 0; lhs.ch[i] || rhs.ch[i]; ++i)
        if (lhs.ch[i] != rhs.ch[i]) return 0;
    return 1;
}
int cmpTrain(const station_train &lhs, const station_train &rhs) {
    for (int i = 31; lhs.ch[i] || rhs.ch[i]; ++i) {
        if (lhs.ch[i] < rhs.ch[i]) return -1;
        if (lhs.ch[i] > rhs.ch[i]) return 1;
    }
    return 0;
}
struct user_time {
    int timestamp;
    char ch[21];
    user_time() {
        ch[0] = 0;
        timestamp = inf;
    }
    user_time(const user_time &other) {
        memcpy(ch, other.ch, 21);
        timestamp = other.timestamp;
    }
    user_time(const string &other1, int other2) {
        memcpy(ch, other1.ch, 21);
        timestamp = other2;
    }
    user_time &operator=(const user_time &rhs) {
        memcpy(ch, rhs.ch, 21);
        timestamp = rhs.timestamp;
        return *this;
    }
    bool operator<(const user_time &rhs) const {
        for (int i = 0; ch[i] || rhs.ch[i]; ++i)
            if (ch[i] != rhs.ch[i]) return ch[i] < rhs.ch[i];
        return timestamp > rhs.timestamp;
    }
    bool operator==(const user_time &rhs) const {
        for (int i = 0; ch[i] || rhs.ch[i]; ++i)
            if (ch[i] != rhs.ch[i]) return 0;
        return timestamp == rhs.timestamp;
    }
    bool operator!=(const user_time &rhs) const {
        for (int i = 0; ch[i] || rhs.ch[i]; ++i)
            if (ch[i] != rhs.ch[i]) return 1;
        return timestamp != rhs.timestamp;
    }
};
bool cmpUser(const user_time &lhs, const user_time &rhs) {
    for (int i = 0; lhs.ch[i] || rhs.ch[i]; ++i)
        if (lhs.ch[i] != rhs.ch[i]) return 0;
    return 1;
}
struct train_date_time {
    int timestamp;
    char ch[22];
    train_date_time() {
        memset(ch, 0, 22);
        timestamp = 0;
    }
    train_date_time(const train_date_time &other) {
        memcpy(ch, other.ch, 22);
        timestamp = other.timestamp;
    }
    train_date_time(const string &other1, int other2, int other3) {
        memcpy(ch + 1, other1.ch, 21);
        ch[0] = other2;
        timestamp = other3;
    }
    train_date_time &operator=(const train_date_time &rhs) {
        memcpy(ch, rhs.ch, 22);
        timestamp = rhs.timestamp;
        return *this;
    }
    bool operator<(const train_date_time &rhs) const {
        for (int i = 0; ch[i] || rhs.ch[i]; ++i)
            if (ch[i] != rhs.ch[i]) return ch[i] < rhs.ch[i];
        return timestamp < rhs.timestamp;
    }
    bool operator==(const train_date_time &rhs) const {
        for (int i = 0; ch[i] || rhs.ch[i]; ++i)
            if (ch[i] != rhs.ch[i]) return 0;
        return timestamp == rhs.timestamp;
    }
    bool operator!=(const train_date_time &rhs) const {
        for (int i = 0; ch[i] || rhs.ch[i]; ++i)
            if (ch[i] != rhs.ch[i]) return 1;
        return timestamp != rhs.timestamp;
    }
};
bool cmpTrainDate(const train_date_time &lhs, const train_date_time &rhs) {
    for (int i = 0; lhs.ch[i] || rhs.ch[i]; ++i)
        if (lhs.ch[i] != rhs.ch[i]) return 0;
    return 1;
}
bool cmp(const char *lhs, const char *rhs) {
    for (int i = 0; *(lhs + i) || *(rhs + i); ++i)
        if (*(lhs + i) != *(rhs + i)) return 0;
    return 1;
}

struct query_ticket_info {
    char trainID[21];
    realtime stime, ttime;
    int totTime, price, seat;
};
bool cmpTime(const query_ticket_info &lhs, const query_ticket_info &rhs) {
    if (lhs.totTime != rhs.totTime) return lhs.totTime < rhs.totTime;
    for (int i = 0; lhs.trainID[i] || rhs.trainID[i]; ++i)
        if (lhs.trainID[i] != rhs.trainID[i]) return lhs.trainID[i] < rhs.trainID[i];
    return 0;
}
bool cmpCost(const query_ticket_info &lhs, const query_ticket_info &rhs) {
    if (lhs.price != rhs.price) return lhs.price < rhs.price;
    for (int i = 0; lhs.trainID[i] || rhs.trainID[i]; ++i)
        if (lhs.trainID[i] != rhs.trainID[i]) return lhs.trainID[i] < rhs.trainID[i];
    return 0;
}
struct query_transfer_info {
    char trainID1[21], trainID2[21], station[31];
    realtime stime1, ttime1, stime2, ttime2;
    int price1, price2, seat1, seat2, totTime, totPrice;
    query_transfer_info() {totTime = totPrice = inf;}
    void update() {
        totTime = ttime2 - stime1;
        totPrice = price1 + price2;
    }
    bool cmpTrainID() {
        for (int i = 0; trainID1[i] || trainID2[i]; ++i)
            if (trainID1[i] != trainID2[i]) return 0;
        return 1;
    }
};
bool cmpTrainID12(const query_transfer_info &lhs, const query_transfer_info &rhs) {
    for (int i = 0; lhs.trainID1[i] || rhs.trainID1[i]; ++i)
        if (lhs.trainID1[i] != rhs.trainID1[i]) return lhs.trainID1[i] < rhs.trainID1[i];
    for (int i = 0; lhs.trainID2[i] || rhs.trainID2[i]; ++i)
        if (lhs.trainID2[i] != rhs.trainID2[i]) return lhs.trainID2[i] < rhs.trainID2[i];
    return 0;
}
bool cmpTime(const query_transfer_info &lhs, const query_transfer_info &rhs) {
    if (lhs.totTime != rhs.totTime) return lhs.totTime < rhs.totTime;
    if (lhs.totPrice != rhs.totPrice) return lhs.totPrice < rhs.totPrice;
    return cmpTrainID12(lhs, rhs);
}
bool cmpCost(const query_transfer_info &lhs, const query_transfer_info &rhs) {
    if (lhs.totPrice != rhs.totPrice) return lhs.totPrice < rhs.totPrice;
    if (lhs.totTime != rhs.totTime) return lhs.totTime < rhs.totTime;
    return cmpTrainID12(lhs, rhs);
}
template<class RandomIt, class Compare>
void sort(RandomIt *st, RandomIt *ed, Compare cmp) {
    if (st >= ed) return;
    RandomIt *l = st;
    RandomIt *r = ed - 1;
    RandomIt bs = *(st + 998244353 % (ed - st));
    while (l <= r) {
        while (l <= r && cmp(*l, bs)) ++l;
        while (l <= r && cmp(bs, *r)) --r;
        if (l > r) break;
        std::swap(*l, *r);
        ++l;
        --r;
    }
    sort(st, r + 1, cmp);
    sort(l, ed, cmp);
}

const int BlockInfo = 4096, BlockTicketNum = 512;

bool isFirstUser, isFirstTrain;
BPT <string, string> users("user", 21, 80, isFirstUser);
BPT <string, int> trainaddr("trainaddr", 21, 4, isFirstTrain);
BPT <station_train, int> stations("stationtrain", 52, 4, isFirstTrain);
BPT <station_train, string> tags("tag", 52, 16, isFirstTrain);
BPT <user_time, string> orders("order", 25, 101, isFirstTrain);
BPT <train_date_time, string> pendingQueue("pending", 26, 121, isFirstTrain);
int trainNum, traindateNum;
std::fstream trainIO, ticketNumIO;

void readOp(char &op) {
    while ((op = getchar()) != '-') {
        if (op == '\n') return;
    }
    op = getchar();
}
void time_to_str(char *lhs, const realtime &rhs) {
    *lhs = rhs.d.month;
    *(lhs + 1) = rhs.d.day;
    *(lhs + 2) = rhs.hour;
    *(lhs + 3) = rhs.minute;
}
void str_to_time(realtime &lhs, const char *rhs) {
    lhs.d.month = *rhs;
    lhs.d.day = *(rhs + 1);
    lhs.hour = *(rhs + 2);
    lhs.minute = *(rhs + 3);
    lhs.calcTotMins();
}
void init() {
    trainIO.open("traininfo", std::ios::in | std::ios::out | std::ios::binary);
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
    ticketNumIO.open("ticketnum", std::ios::in | std::ios::out | std::ios::binary);
    if (!ticketNumIO) {
        traindateNum = 1;
        std::ofstream tmp("ticketnum");
        tmp.close();
        ticketNumIO.open("ticketnum", std::ios::in | std::ios::out | std::ios::binary);
    }
    else {
        ticketNumIO.seekg(0);
        ticketNumIO.read(reinterpret_cast<char *>(&traindateNum), 4);
    }
}

int main() {
    init();
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
            memcpy(info + 3934, reinterpret_cast<const char *>(&traindateNum), 4);
            trainIO.seekp(tmp.second * BlockInfo);
            trainIO.write(info, BlockInfo);
            date bd(info[28], info[29]), ed(info[30], info[31]);
            int dday = ed - bd;
            char restSeats[BlockTicketNum];
            for (int i = 0; i < info[21] - 1; ++i) memcpy(restSeats + i * 4, info + 22, 4);
            for (int i = 0; i <= dday; ++i) {
                ticketNumIO.seekp((traindateNum + i) * BlockTicketNum);
                ticketNumIO.write(restSeats, BlockTicketNum);
            }
            traindateNum += dday + 1;
            station_train stationtrain;
            memcpy(stationtrain.ch + 31, info, 21);
            for (int i = 0; i < info[21]; ++i) {
                memcpy(stationtrain.ch, info + 34 + i * 31, 31);
                stations.insert(stationtrain, tmp.second);
            }
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
                int ticketNumAddr = 0;
                memcpy(reinterpret_cast<char *>(&ticketNumAddr), info + 3934, 4);
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
                    int dprice = 0, dmin = 0;
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
                        int dmin = 0;
                        memcpy(reinterpret_cast<char *>(&dmin), info + 3734 + (i - 1) * 2, 2);
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
            station_train sstation, tstation;
            date d;
            bool (*p)(const query_ticket_info &, const query_ticket_info &) = cmpTime;
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
                    if (tmp[0] == 'c') p = cmpCost;
                }
                if (op == '\n') break;
            }
            printf("[%d] ", timestamp);
            BPT <station_train, int>::iterator its, itt;
            stations.lower_bound(its, sstation);
            stations.lower_bound(itt, tstation);
            station_train scur, tcur;
            int num = 0;
            query_ticket_info ans[1000];
            if (its.first(scur) && itt.first(tcur)
              && cmpStation(scur, sstation) && cmpStation(tcur, tstation)) {
                while (1) {
                    int flag = cmpTrain(scur, tcur);
                    if (!flag) {
                        int addr;
                        its.second(addr);
                        char info[BlockInfo];
                        trainIO.seekg(addr * BlockInfo);
                        trainIO.read(info, BlockInfo);
                        realtime stime(info[28], info[29], info[26], info[27]);
                        int i = 0;
                        while (i < info[21]) {
                            if (cmp(info + 34 + i * 31, sstation.ch)) break;
                            int tmp = 0;
                            memcpy(reinterpret_cast<char *>(&tmp), info + 3534 + i * 2, 2);
                            stime += tmp;
                            tmp = 0;
                            memcpy(reinterpret_cast<char *>(&tmp), info + 3734 + i * 2, 2);
                            stime += tmp;
                            ++i;
                        }
                        date bd(info[28], info[29]), ed(info[30], info[31]);
                        int dd1 = ed - bd, dd2 = d - stime.d;
                        if (dd2 >= 0 && dd2 <= dd1) {
                            realtime ttime(stime);
                            int tmp = 0, price = 0, seat = 0;
                            bool isAns = 0;
                            memcpy(reinterpret_cast<char *>(&tmp), info + 3534 + i * 2, 2);
                            ttime += tmp;
                            memcpy(reinterpret_cast<char *>(&price), info + 3134 + i * 4, 4);
                            char ticketNum[BlockTicketNum];
                            int ticketNumAddr = 0;
                            memcpy(reinterpret_cast<char *>(&ticketNumAddr), info + 3934, 4);
                            ticketNumAddr += dd2;
                            ticketNumIO.seekg(ticketNumAddr * BlockTicketNum);
                            ticketNumIO.read(ticketNum, BlockTicketNum);
                            memcpy(reinterpret_cast<char *>(&seat), ticketNum + i * 4, 4);
                            ++i;
                            while (i < info[21]) {
                                if (cmp(info + 34 + i * 31, tstation.ch)) {
                                    isAns = 1;
                                    break;
                                }
                                tmp = 0;
                                memcpy(reinterpret_cast<char *>(&tmp), info + 3734 + (i - 1) * 2, 2);
                                ttime += tmp;
                                tmp = 0;
                                memcpy(reinterpret_cast<char *>(&tmp), info + 3534 + i * 2, 2);
                                ttime += tmp;
                                tmp = 0;
                                memcpy(reinterpret_cast<char *>(&tmp), info + 3134 + i * 4, 4);
                                price += tmp;
                                tmp = 0;
                                memcpy(reinterpret_cast<char *>(&tmp), ticketNum + i * 4, 4);
                                seat = std::min(seat, tmp);
                                ++i;
                            }
                            if (isAns) {
                                ++num;
                                memcpy(ans[num].trainID, info, 21);
                                ans[num].stime = stime;
                                ans[num].stime.d += dd2;
                                ans[num].stime.calcTotMins();
                                ans[num].ttime = ttime;
                                ans[num].ttime.d += dd2;
                                ans[num].ttime.calcTotMins();
                                ans[num].totTime = ttime - stime;
                                ans[num].price = price;
                                ans[num].seat = seat;
                            }
                        }
                    }
                    if (flag <= 0) {
                        its.inc();
                        if (!its.first(scur) || !cmpStation(scur, sstation)) break;
                    }
                    if (flag >= 0) {
                        itt.inc();
                        if (!itt.first(tcur) || !cmpStation(tcur, tstation)) break;
                    }
                }
            }
            sort(ans + 1, ans + num + 1, p);
            printf("%d\n", num);
            for (int i = 1; i <= num; ++i) {
                printf("%s %s ", ans[i].trainID, sstation.ch);
                ans[i].stime.print();
                printf(" -> %s ", tstation.ch);
                ans[i].ttime.print();
                printf(" %d %d\n", ans[i].price, ans[i].seat);
            }
            continue;
        }
        if (Cmd == "query_transfer") {
            station_train sstation, tstation;
            date d;
            bool (*p)(const query_transfer_info &, const query_transfer_info &) = cmpTime;
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
                    if (tmp[0] == 'c') p = cmpCost;
                }
                if (op == '\n') break;
            }
            printf("[%d] ", timestamp);
            query_transfer_info ans;
            BPT <station_train, int>::iterator it;
            stations.lower_bound(it, sstation);
            station_train cur;
            tags.clear();
            while (it.first(cur) && cmpStation(sstation, cur)) {
                int addr;
                it.second(addr);
                char info[BlockInfo];
                trainIO.seekg(addr * BlockInfo);
                trainIO.read(info, BlockInfo);
                realtime stime(info[28], info[29], info[26], info[27]);
                int i = 0;
                while (i < info[21]) {
                    if (cmp(info + 34 + i * 31, sstation.ch)) break;
                    int tmp = 0;
                    memcpy(reinterpret_cast<char *>(&tmp), info + 3534 + i * 2, 2);
                    stime += tmp;
                    tmp = 0;
                    memcpy(reinterpret_cast<char *>(&tmp), info + 3734 + i * 2, 2);
                    stime += tmp;
                    ++i;
                }
                date bd(info[28], info[29]), ed(info[30], info[31]);
                int dd1 = ed - bd, dd2 = d - stime.d;
                if (dd2 >= 0 && dd2 <= dd1) {
                    string tagInfo;
                    station_train curStation;
                    memcpy(curStation.ch + 31, info, 21);
                    stime.d = d;
                    stime.calcTotMins();
                    time_to_str(tagInfo.ch, stime);
                    int tmp = 0, price = 0, seat = 0;
                    memcpy(reinterpret_cast<char *>(&tmp), info + 3534 + i * 2, 2);
                    stime += tmp;
                    memcpy(reinterpret_cast<char *>(&price), info + 3134 + i * 4, 4);
                    char ticketNum[BlockTicketNum];
                    int ticketNumAddr = 0;
                    memcpy(reinterpret_cast<char *>(&ticketNumAddr), info + 3934, 4);
                    ticketNumAddr += dd2;
                    ticketNumIO.seekg(ticketNumAddr * BlockTicketNum);
                    ticketNumIO.read(ticketNum, BlockTicketNum);
                    memcpy(reinterpret_cast<char *>(&seat), ticketNum + i * 4, 4);
                    ++i;
                    while (i < info[21]) {
                        memcpy(curStation.ch, info + 34 + i * 31, 31);
                        time_to_str(tagInfo.ch + 4, stime);
                        memcpy(tagInfo.ch + 8, reinterpret_cast<const char *>(&price), 4);
                        memcpy(tagInfo.ch + 12, reinterpret_cast<const char *>(&seat), 4);
                        tags.insert(curStation, tagInfo);
                        tmp = 0;
                        memcpy(reinterpret_cast<char *>(&tmp), info + 3734 + (i - 1) * 2, 2);
                        stime += tmp;
                        tmp = 0;
                        memcpy(reinterpret_cast<char *>(&tmp), info + 3534 + i * 2, 2);
                        stime += tmp;
                        tmp = 0;
                        memcpy(reinterpret_cast<char *>(&tmp), info + 3134 + i * 4, 4);
                        price += tmp;
                        tmp = 0;
                        memcpy(reinterpret_cast<char *>(&tmp), ticketNum + i * 4, 4);
                        seat = std::min(seat, tmp);
                        ++i;
                    }
                }
                it.inc();
            }
            stations.lower_bound(it, tstation);
            while (it.first(cur) && cmpStation(tstation, cur)) {
                query_transfer_info tmpAns;
                int addr;
                it.second(addr);
                char info[BlockInfo];
                trainIO.seekg(addr * BlockInfo);
                trainIO.read(info, BlockInfo);
                memcpy(tmpAns.trainID2, info, 21);
                realtime ttime(info[28], info[29], info[26], info[27]);
                int i = 0;
                while (i < info[21]) {
                    if (cmp(info + 34 + i * 31, tstation.ch)) break;
                    int tmp = 0;
                    memcpy(reinterpret_cast<char *>(&tmp), info + 3534 + i * 2, 2);
                    ttime += tmp;
                    tmp = 0;
                    memcpy(reinterpret_cast<char *>(&tmp), info + 3734 + i * 2, 2);
                    ttime += tmp;
                    ++i;
                }
                --i;
                int tmp = 0, price = 0;
                memcpy(reinterpret_cast<char *>(&tmp), info + 3734 + i * 2, 2);
                ttime -= tmp;
                realtime stime(ttime);
                tmp = 0;
                memcpy(reinterpret_cast<char *>(&tmp), info + 3534 + i * 2, 2);
                stime -= tmp;
                memcpy(reinterpret_cast<char *>(&price), info + 3134 + i * 4, 4);
                while (i >= 0) {
                    station_train curStation, tmpStation;
                    memcpy(curStation.ch, info + 34 + i * 31, 31);
                    memcpy(tmpAns.station, curStation.ch, 31);
                    BPT <station_train, string>::iterator tagIt;
                    tags.lower_bound(tagIt, curStation);
                    while (tagIt.first(tmpStation) && cmpStation(curStation, tmpStation)) {
                        string tagInfo;
                        tagIt.second(tagInfo);
                        str_to_time(tmpAns.ttime1, tagInfo.ch + 4);
                        date bd(info[28], info[29]), ed(info[30], info[31]);
                        int dd1 = ed - bd, dd2 = tmpAns.ttime1.d - stime.d;
                        if (cmpHourMin(stime, tmpAns.ttime1)) ++dd2;
                        if (dd2 < 0) dd2 = 0;
                        if (dd2 >= 0 && dd2 <= dd1) {
                            memcpy(tmpAns.trainID1, tmpStation.ch + 31, 21);
                            if (!tmpAns.cmpTrainID()) {
                                str_to_time(tmpAns.stime1, tagInfo.ch);
                                tmpAns.price1 = tmpAns.seat1 = 0;
                                memcpy(reinterpret_cast<char *>(&tmpAns.price1), tagInfo.ch + 8, 4);
                                memcpy(reinterpret_cast<char *>(&tmpAns.seat1), tagInfo.ch + 12, 4);
                                tmpAns.stime2 = stime;
                                tmpAns.stime2.d += dd2;
                                tmpAns.stime2.calcTotMins();
                                tmpAns.ttime2 = ttime;
                                tmpAns.ttime2.d += dd2;
                                tmpAns.ttime2.calcTotMins();
                                tmpAns.price2 = price;
                                tmpAns.seat2 = dd2;
                                tmpAns.update();
                                if (p(tmpAns, ans)) ans = tmpAns;
                            }
                        }
                        tagIt.inc();
                    }
                    --i;
                    tmp = 0;
                    memcpy(reinterpret_cast<char *>(&tmp), info + 3734 + i * 2, 2);
                    stime -= tmp;
                    tmp = 0;
                    memcpy(reinterpret_cast<char *>(&tmp), info + 3534 + i * 2, 2);
                    stime -= tmp;
                    tmp = 0;
                    memcpy(reinterpret_cast<char *>(&tmp), info + 3134 + i * 4, 4);
                    price += tmp;
                }
                it.inc();
            }
            if (ans.totTime == inf) puts("0");
            else {
                pair <int, int> tmp = trainaddr.find(ans.trainID2);
                char info[BlockInfo];
                trainIO.seekg(tmp.second * BlockInfo);
                trainIO.read(info, BlockInfo);
                char ticketNum[BlockTicketNum];
                int ticketNumAddr = 0;
                memcpy(reinterpret_cast<char *>(&ticketNumAddr), info + 3934, 4);
                ticketNumAddr += ans.seat2;
                ticketNumIO.seekg(ticketNumAddr * BlockTicketNum);
                ticketNumIO.read(ticketNum, BlockTicketNum);
                ans.seat2 = inf;
                int i = 0;
                while (i < info[21]) {
                    if (cmp(info + 34 + i * 31, ans.station)) break;
                    ++i;
                }
                while (i < info[21]) {
                    if (cmp(info + 34 + i * 31, tstation.ch)) break;
                    int tmp = 0;
                    memcpy(reinterpret_cast<char *>(&tmp), ticketNum + i * 4, 4);
                    ans.seat2 = std::min(ans.seat2, tmp);
                    ++i;
                }
                printf("%s %s ", ans.trainID1, sstation.ch);
                ans.stime1.print();
                printf(" -> %s ", ans.station);
                ans.ttime1.print();
                printf(" %d %d\n%s %s ", ans.price1, ans.seat1, ans.trainID2, ans.station);
                ans.stime2.print();
                printf(" -> %s ", tstation.ch);
                ans.ttime2.print();
                printf(" %d %d\n", ans.price2, ans.seat2);
            }
            continue;
        }
        if (Cmd == "buy_ticket") {
            string username, trainID;
            date d;
            int n, q = 0;
            station_train sstation, tstation;
            while (1) {
                readOp(op);
                if (op == 'u') scanf("%s", username.ch);
                if (op == 'i') scanf("%s", trainID.ch);
                if (op == 'd') {
                    scanf("%d-%d", &d.month, &d.day);
                    d.calcTotDays();
                }
                if (op == 'n') scanf("%d", &n);
                if (op == 'f') scanf("%s", sstation.ch);
                if (op == 't') scanf("%s", tstation.ch);
                if (op == 'q') {
                    char tmp[6];
                    scanf("%s", tmp);
                    if (tmp[0] == 't') q = 1;
                }
                if (op == '\n') break;
            }
            printf("[%d] ", timestamp);
            pair <int, string> tmp1 = users.find(username);
            if (tmp1.first == -1 || !tmp1.second.ch[79]) {
                puts("-1");
                continue;
            }
            pair <int, int> tmp2 = trainaddr.find(trainID);
            if (tmp2.first == -1) {
                puts("-1");
                continue;
            }
            char info[BlockInfo];
            trainIO.seekg(tmp2.second * BlockInfo);
            trainIO.read(info, BlockInfo);
            if (!info[33]) {
                puts("-1");
                continue;
            }
            int maxSeat = 0;
            memcpy(reinterpret_cast<char *>(&maxSeat), info + 22, 4);
            if (n > maxSeat) {
                puts("-1");
                continue;
            }
            int i = 0;
            realtime stime(info[28], info[29], info[26], info[27]);
            while (i < info[21]) {
                if (cmp(info + 34 + i * 31, sstation.ch)) break;
                int tmp = 0;
                memcpy(reinterpret_cast<char *>(&tmp), info + 3534 + i * 2, 2);
                stime += tmp;
                tmp = 0;
                memcpy(reinterpret_cast<char *>(&tmp), info + 3734 + i * 2, 2);
                stime += tmp;
                ++i;
            }
            if (i >= info[21]) {
                puts("-1");
                continue;
            }
            date bd(info[28], info[29]), ed(info[30], info[31]);
            int dd1 = ed - bd, dd2 = d - stime.d;
            if (dd2 < 0 || dd2 > dd1) {
                puts("-1");
                continue;
            }
            stime.d += dd2;
            stime.calcTotMins();
            char ticketNum[BlockTicketNum];
            int ticketNumAddr = 0;
            memcpy(reinterpret_cast<char *>(&ticketNumAddr), info + 3934, 4);
            ticketNumAddr += dd2;
            ticketNumIO.seekg(ticketNumAddr * BlockTicketNum);
            ticketNumIO.read(ticketNum, BlockTicketNum);
            realtime ttime(stime);
            int tmp = 0, price = 0, seat = 0;
            int j = i;
            memcpy(reinterpret_cast<char *>(&tmp), info + 3534 + i * 2, 2);
            ttime += tmp;
            memcpy(reinterpret_cast<char *>(&price), info + 3134 + i * 4, 4);
            memcpy(reinterpret_cast<char *>(&seat), ticketNum + i * 4, 4);
            ++i;
            while (i < info[21]) {
                if (cmp(info + 34 + i * 31, tstation.ch)) break;
                tmp = 0;
                memcpy(reinterpret_cast<char *>(&tmp), info + 3734 + (i - 1) * 2, 2);
                ttime += tmp;
                tmp = 0;
                memcpy(reinterpret_cast<char *>(&tmp), info + 3534 + i * 2, 2);
                ttime += tmp;
                tmp = 0;
                memcpy(reinterpret_cast<char *>(&tmp), info + 3134 + i * 4, 4);
                price += tmp;
                tmp = 0;
                memcpy(reinterpret_cast<char *>(&tmp), ticketNum + i * 4, 4);
                seat = std::min(seat, tmp);
                ++i;
            }
            if (i >= info[21]) {
                puts("-1");
                continue;
            }
            if (seat < n && !q) {
                puts("-1");
                continue;
            }
            user_time cur(username, timestamp);
            string orderInfo;
            memcpy(orderInfo.ch + 1, trainID.ch, 21);
            memcpy(orderInfo.ch + 22, sstation.ch, 31);
            time_to_str(orderInfo.ch + 53, stime);
            memcpy(orderInfo.ch + 57, tstation.ch, 31);
            time_to_str(orderInfo.ch + 88, ttime);
            memcpy(orderInfo.ch + 92, reinterpret_cast<const char *>(&price), 4);
            memcpy(orderInfo.ch + 96, reinterpret_cast<const char *>(&n), 4);
            if (seat >= n) {
                orderInfo.ch[0] = 0;
                for (int k = j; k < i; ++k) {
                    int tmp = 0;
                    memcpy(reinterpret_cast<char *>(&tmp), ticketNum + k * 4, 4);
                    tmp -= n;
                    memcpy(ticketNum + k * 4, reinterpret_cast<const char *>(&tmp), 4);
                }
                ticketNumIO.seekp(ticketNumAddr * BlockTicketNum);
                ticketNumIO.write(ticketNum, BlockTicketNum);
                printf("%lld\n", 1ll * price * n);
            }
            else {
                train_date_time curTrain(trainID, dd2 + 1, timestamp);
                orderInfo.ch[0] = 1;
                memcpy(orderInfo.ch + 100, username.ch, 21);
                pendingQueue.insert(curTrain, orderInfo);
                puts("queue");
            }
            orderInfo.ch[100] = dd2 + 1;
            orders.insert(cur, orderInfo);
            continue;
        }
        if (Cmd == "query_order") {
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
            user_time tmpusername(username, inf);
            BPT <user_time, string>::iterator it;
            orders.lower_bound(it, tmpusername);
            user_time cur;
            int tot = 0;
            while (it.first(cur) && cmpUser(cur, tmpusername)) {
                ++tot;
                it.inc();
            }
            printf("%d\n",tot);
            orders.lower_bound(it, tmpusername);
            while (tot--) {
                string orderInfo;
                it.second(orderInfo);
                if (!orderInfo.ch[0]) printf("[success] ");
                else if (orderInfo.ch[0] == 1) printf("[pending] ");
                else printf("[refunded] ");
                printf("%s %s ", orderInfo.ch + 1, orderInfo.ch + 22);
                realtime tmptime;
                str_to_time(tmptime, orderInfo.ch + 53);
                tmptime.print();
                printf(" -> %s ", orderInfo.ch + 57);
                str_to_time(tmptime, orderInfo.ch + 88);
                tmptime.print();
                int price = 0, num = 0;
                memcpy(reinterpret_cast<char *>(&price), orderInfo.ch + 92, 4);
                memcpy(reinterpret_cast<char *>(&num), orderInfo.ch + 96, 4);
                printf(" %d %d\n", price, num);
                it.inc();
            }
            continue;
        }
        if (Cmd == "refund_ticket") {
            string username;
            int n = 1;
            while (1) {
                readOp(op);
                if (op == 'u') scanf("%s", username.ch);
                if (op == 'n') scanf("%d", &n);
                if (op == '\n') break;
            }
            printf("[%d] ", timestamp);
            pair <int, string> tmp = users.find(username);
            if (tmp.first == -1 || !tmp.second.ch[79]) {
                puts("-1");
                continue;
            }
            user_time tmpusername(username, inf);
            BPT <user_time, string>::iterator it;
            orders.lower_bound(it, tmpusername);
            --n;
            while (n--) it.inc();
            user_time cur;
            if (!it.first(cur) || !cmpUser(cur, tmpusername)) {
                puts("-1");
                continue;
            }
            string orderInfo;
            it.second(orderInfo);
            if (orderInfo.ch[0] == 2) {
                puts("-1");
                continue;
            }
            if (orderInfo.ch[0] == 1) {
                train_date_time tmpTrain;
                tmpTrain.timestamp = cur.timestamp;
                tmpTrain.ch[0] = orderInfo.ch[100];
                memcpy(tmpTrain.ch + 1, orderInfo.ch + 1, 21);
                pendingQueue.erase(tmpTrain);
            }
            else {
                string trainID;
                memcpy(trainID.ch, orderInfo.ch + 1, 21);
                station_train sstation, tstation;
                memcpy(sstation.ch, orderInfo.ch + 22, 31);
                memcpy(tstation.ch, orderInfo.ch + 57, 31);
                pair <int, int> tmp = trainaddr.find(trainID);
                char info[BlockInfo];
                trainIO.seekg(tmp.second * BlockInfo);
                trainIO.read(info, BlockInfo);
                realtime stime(info[28], info[29], info[26], info[27]);
                int i = 0;
                while (i < info[21]) {
                    if (cmp(sstation.ch, info + 34 + i * 31)) break;
                    int tmp = 0;
                    memcpy(reinterpret_cast<char *>(&tmp), info + 3534 + i * 2, 2);
                    stime += tmp;
                    tmp = 0;
                    memcpy(reinterpret_cast<char *>(&tmp), info + 3734 + i * 2, 2);
                    stime += tmp;
                    ++i;
                }
                date d(orderInfo.ch[53], orderInfo.ch[54]);
                int dd = d - stime.d;
                char ticketNum[BlockTicketNum];
                int ticketNumAddr = 0;
                memcpy(reinterpret_cast<char *>(&ticketNumAddr), info + 3934, 4);
                ticketNumAddr += dd;
                ticketNumIO.seekg(ticketNumAddr * BlockTicketNum);
                ticketNumIO.read(ticketNum, BlockTicketNum);
                int n = 0;
                memcpy(reinterpret_cast<char *>(&n), orderInfo.ch + 96, 4);
                while (i < info[21]) {
                    if (cmp(tstation.ch, info + 34 + i * 31)) break;
                    int tmp = 0;
                    memcpy(reinterpret_cast<char *>(&tmp), ticketNum + i * 4, 4);
                    tmp += n;
                    memcpy(ticketNum + i * 4, reinterpret_cast<const char *>(&tmp), 4);
                    ++i;
                }
                train_date_time trainDate(trainID, dd + 1, 0);
                BPT <train_date_time, string>::iterator it;
                pendingQueue.lower_bound(it, trainDate);
                train_date_time cur;
                while (it.first(cur) && cmpTrainDate(cur, trainDate)) {
                    string orderInfo;
                    it.second(orderInfo);
                    memcpy(sstation.ch, orderInfo.ch + 22, 31);
                    memcpy(tstation.ch, orderInfo.ch + 57, 31);
                    int n = 0, seat = inf, i = 0;
                    memcpy(reinterpret_cast<char *>(&n), orderInfo.ch + 96, 4);
                    while (i < info[21]) {
                        if (cmp(sstation.ch, info + 34 + i * 31)) break;
                        ++i;
                    }
                    int j = i;
                    while (i < info[21]) {
                        if (cmp(tstation.ch, info + 34 + i * 31)) break;
                        int tmp = 0;
                        memcpy(reinterpret_cast<char *>(&tmp), ticketNum + i * 4, 4);
                        seat = std::min(seat, tmp);
                        ++i;
                    }
                    if (seat < n) {
                        it.inc();
                        continue;
                    }
                    int price = 0;
                    memcpy(reinterpret_cast<char *>(&price), orderInfo.ch + 92, 4);
                    for (int k = j; k < i; ++k) {
                        int tmp = 0;
                        memcpy(reinterpret_cast<char *>(&tmp), ticketNum + k * 4, 4);
                        tmp -= n;
                        memcpy(ticketNum + k * 4, reinterpret_cast<const char *>(&tmp), 4);
                    }
                    user_time tmpUser;
                    tmpUser.timestamp = cur.timestamp;
                    memcpy(tmpUser.ch, orderInfo.ch + 100, 21);
                    BPT <user_time, string>::iterator userIt;
                    orders.lower_bound(userIt, tmpUser);
                    string tmpInfo;
                    userIt.second(tmpInfo);
                    tmpInfo.ch[0] = 0;
                    userIt.modify(tmpInfo);
                    BPT <train_date_time, string>::iterator tmpIt = it;
                    train_date_time tmpTrain1, tmpTrain2;
                    it.first(tmpTrain1);
                    if (tmpIt.inc() && tmpIt.first(tmpTrain2)) {
                        pendingQueue.erase(tmpTrain1);
                        pendingQueue.lower_bound(it, tmpTrain2);
                    }
                    else {
                        pendingQueue.erase(tmpTrain1);
                        break;
                    }
                }
                ticketNumIO.seekp(ticketNumAddr * BlockTicketNum);
                ticketNumIO.write(ticketNum, BlockTicketNum);
            }
            orderInfo.ch[0] = 2;
            it.modify(orderInfo);
            puts("0");
            continue;
        }
        if (Cmd == "clean") {
            while (1) {
                readOp(op);
                if (op == '\n') break;
            }
            isFirstUser = 1;
            users.clear();
            trainaddr.clear();
            stations.clear();
            tags.clear();
            orders.clear();
            pendingQueue.clear();
            trainNum = 0;
            traindateNum = 1;
            printf("[%d] 0\n", timestamp);
            continue;
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