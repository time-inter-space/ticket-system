#ifndef TICKET_BPT_HPP
#define TICKET_BPT_HPP

#include <iostream>
#include <cstring>
#include <cstdio>
#include <cmath>
#include <string>
#include <fstream>

const int Block = 4096;

template <class T1, class T2>
class pair {
public:
	T1 first;
	T2 second;
	constexpr pair() : first(), second() {}
	pair(const pair &other) = default;
	pair(pair &&other) = default;
	pair(const T1 &x, const T2 &y) : first(x), second(y) {}
	template <class U1, class U2>
	pair(U1 &&x, U2 &&y) : first(x), second(y) {}
	template <class U1, class U2>
	pair(const pair <U1, U2> &other) : first(other.first), second(other.second) {}
	template <class U1, class U2>
	pair(pair <U1, U2> &&other) : first(other.first), second(other.second) {}
    pair <T1, T2> &operator=(const pair <T1, T2> &rhs) {
        first = rhs.first;
        second = rhs.second;
        return *this;
    }
};

template <class Key, class T, class Compare = std::less<Key> >
class BPT {
private:
    int KeySize, ValSize, M, L;
    int rt;
    int tot;
    int head;
    Compare cmp;
    Key newKey;
    std::fstream io;
    Key *tmpKey;
    T *tmpVal;
    int *tmpSn;
    int top;
    char buf[Block * 20];
    void incTop(int cur) {
        ++top;
        io.seekg(cur * Block);
        io.read(buf + top * Block, Block);
    }
    void decTop(int cur) {
        io.seekp(cur * Block);
        io.write(buf + top * Block, Block);
        --top;
    }
    void readKey(int addr, Key &tmp) {
        memcpy(reinterpret_cast <char *>(&tmp), buf + top * Block + addr, KeySize);
    }
    void writeKey(int addr, const Key &tmp) {
        memcpy(buf + top * Block + addr, reinterpret_cast <const char *>(&tmp), KeySize);
    }
    void readVal(int addr, T &tmp) {
        memcpy(reinterpret_cast <char *>(&tmp), buf + top * Block + addr, ValSize);
    }
    void writeVal(int addr, const T &tmp) {
        memcpy(buf + top * Block + addr, reinterpret_cast <const char *>(&tmp), ValSize);
    }
    void readInt(int addr, int &tmp) {
        memcpy(reinterpret_cast <char *>(&tmp), buf + top * Block + addr, 4);
    }
    void writeInt(int addr, const int &tmp) {
        memcpy(buf + top * Block + addr, reinterpret_cast <const char *>(&tmp), 4);
    }
    int getNewBlock() {
        return ++tot;
    }
    void insertSon(int pos, const Key &key, int sn, int &n) {
        for (int i = n; i >= pos; --i) {
            Key tmp1;
            int tmp2;
            readKey(8 + (i - 2) * (KeySize + 4) + 4, tmp1);
            writeKey(8 + (i - 1) * (KeySize + 4) + 4, tmp1);
            readInt(8 + (i - 1) * (KeySize + 4), tmp2);
            writeInt(8 + i * (KeySize + 4), tmp2);
        }
        writeKey(8 + (pos - 2) * (KeySize + 4) + 4, key);
        writeInt(8 + (pos - 1) * (KeySize + 4), sn);
        ++n;
        writeInt(0, n);
    }
    void eraseSon(int pos, int &n) {
        for (int i = pos; i < n; ++i) {
            Key tmp1;
            int tmp2;
            readKey(8 + (i - 1) * (KeySize + 4) + 4, tmp1);
            writeKey(8 + (i - 2) * (KeySize + 4) + 4, tmp1);
            readInt(8 + i * (KeySize + 4), tmp2);
            writeInt(8 + (i - 1) * (KeySize + 4), tmp2);
        }
        --n;
        writeInt(0, n);
    }
    void insertData(int pos, const Key &key, const T &val, int &n) {
        for (int i = n; i >= pos; --i) {
            Key tmp1;
            T tmp2;
            readKey(8 + (i - 1) * (KeySize + ValSize), tmp1);
            writeKey(8 + i * (KeySize + ValSize), tmp1);
            readVal(8 + (i - 1) * (KeySize + ValSize) + KeySize, tmp2);
            writeVal(8 + i * (KeySize + ValSize) + KeySize, tmp2);
        }
        writeKey(8 + (pos - 1) * (KeySize + ValSize), key);
        writeVal(8 + (pos - 1) * (KeySize + ValSize) + KeySize, val);
        ++n;
        writeInt(0, n);
    }
    void eraseData(int pos, int &n) {
        for (int i = pos; i < n; ++i) {
            Key tmp1;
            T tmp2;
            readKey(8 + i * (KeySize + ValSize), tmp1);
            writeKey(8 + (i - 1) * (KeySize + ValSize), tmp1);
            readVal(8 + i * (KeySize + ValSize) + KeySize, tmp2);
            writeVal(8 + (i - 1) * (KeySize + ValSize) + KeySize, tmp2);
        }
        --n;
        writeInt(0, n);
    }
    int insert(int cur, const Key &key, const T &val) {
        int n, isLeaf;
        incTop(cur);
        readInt(0, n);
        readInt(4, isLeaf);
        if (isLeaf) {
            int l = 0, r = n;
            while (l < r) {
                int mid = l + r + 1 >> 1;
                Key tmp;
                readKey(8 + (mid - 1) * (KeySize + ValSize), tmp);
                if (cmp(key, tmp)) r = mid - 1;
                else l = mid;
            }
            insertData(l + 1, key, val, n);
            if (n > L) {
                int other = getNewBlock();
                int m = n >> 1;
                for (int i = m + 1; i <= n; ++i) {
                    readKey(8 + (i - 1) * (KeySize + ValSize), tmpKey[i - m]);
                    readVal(8 + (i - 1) * (KeySize + ValSize) + KeySize, tmpVal[i - m]);
                }
                int nxt;
                readInt(Block - 4, nxt);
                writeInt(Block - 4, other);
                writeInt(0, m);
                decTop(cur);
                ++top;
                m = n - m;
                writeInt(0, m);
                writeInt(4, 1);
                newKey = tmpKey[1];
                for (int i = 1; i <= m; ++i) {
                    writeKey(8 + (i - 1) * (KeySize + ValSize), tmpKey[i]);
                    writeVal(8 + (i - 1) * (KeySize + ValSize) + KeySize, tmpVal[i]);
                }
                writeInt(Block - 4, nxt);
                decTop(other);
                return other;
            }
            decTop(cur);
            return 0;
        }
        else {
            int l = 1, r = n;
            while (l < r) {
                int mid = l + r + 1 >> 1;
                Key tmp;
                readKey(8 + (mid - 2) * (KeySize + 4) + 4, tmp);
                if (cmp(key, tmp)) r = mid - 1;
                else l = mid;
            }
            int sn;
            readInt(8 + (l - 1) * (KeySize + 4), sn);
            int other = insert(sn, key, val);
            if (other) {
                insertSon(l + 1, newKey, other, n);
                if (n > M) {
                    other = getNewBlock();
                    int m = n >> 1;
                    for (int i = m + 1; i <= n; ++i) {
                        readKey(8 + (i - 2) * (KeySize + 4) + 4, tmpKey[i - m]);
                        readInt(8 + (i - 1) * (KeySize + 4), tmpSn[i - m]);
                    }
                    writeInt(0, m);
                    decTop(cur);
                    ++top;
                    m = n - m;
                    writeInt(0, m);
                    writeInt(4, 0);
                    for (int i = 1; i <= m; ++i) {
                        if (i == 1) newKey = tmpKey[1];
                        else writeKey(8 + (i - 2) * (KeySize + 4) + 4, tmpKey[i]);
                        writeInt(8 + (i - 1) * (KeySize + 4), tmpSn[i]);
                    }
                    decTop(other);
                    return other;
                }
            }
            decTop(cur);
            return 0;
        }
    }
    int erase(int cur, int bro, int flag, const Key &key, const Key &faKey) {
        int n, isLeaf;
        incTop(cur);
        readInt(0, n);
        readInt(4, isLeaf);
        if (isLeaf) {
            int l = 1, r = n;
            Key tmp;
            while (l < r) {
                int mid = l + r + 1 >> 1;
                readKey(8 + (mid - 1) * (KeySize + ValSize), tmp);
                if (cmp(key, tmp)) r = mid - 1;
                else l = mid;
            }
            readKey(8 + (l - 1) * (KeySize + ValSize), tmp);
            if (tmp != key) {
                --top;
                return 0;
            }
            eraseData(l, n);
            if (cur == rt) {
                decTop(cur);
                if (!n) head = rt = 0;
                return 0;
            }
            if (n < (L + 1 >> 1)) {
                T val;
                int m;
                incTop(bro);
                readInt(0, m);
                if (m > (L + 1 >> 1)) {
                    if (flag) {
                        readKey(8 + (m - 1) * (KeySize + ValSize), newKey);
                        readVal(8 + (m - 1) * (KeySize + ValSize) + KeySize, val);
                        eraseData(m, m);
                        decTop(bro);
                        insertData(1, newKey, val, n);
                    }
                    else {
                        Key tmpKey;
                        readKey(8, tmpKey);
                        readVal(8 + KeySize, val);
                        readKey(8 + (KeySize + ValSize), newKey);
                        eraseData(1, m);
                        decTop(bro);
                        insertData(n + 1, tmpKey, val, n);
                    }
                    decTop(cur);
                    return 1;
                }
                else {
                    if (flag) {
                        std::swap(cur, bro);
                        std::swap(n, m);
                        memcpy(buf + (top + 1) * Block, buf + top * Block, Block);
                        memcpy(buf + top * Block, buf + (top - 1) * Block, Block);
                        memcpy(buf + (top - 1) * Block, buf + (top + 1) * Block, Block);
                    }
                    for (int i = 1; i <= m; ++i) {
                        readKey(8 + (i - 1) * (KeySize + ValSize), tmpKey[i]);
                        readVal(8 + (i - 1) * (KeySize + ValSize) + KeySize, tmpVal[i]);
                    }
                    int nxt;
                    readInt(Block - 4, nxt);
                    decTop(bro);
                    for (int i = 1; i <= m; ++i) {
                        writeKey(8 + (n + i - 1) * (KeySize + ValSize), tmpKey[i]);
                        writeVal(8 + (n + i - 1) * (KeySize + ValSize) + KeySize, tmpVal[i]);
                    }
                    writeInt(Block - 4, nxt);
                    n += m;
                    writeInt(0, n);
                    decTop(cur);
                    return 2;
                }
            }
            decTop(cur);
            return 0;
        }
        else {
            int l = 1, r = n;
            while (l < r) {
                int mid = l + r + 1 >> 1;
                Key tmp;
                readKey(8 + (mid - 2) * (KeySize + 4) + 4, tmp);
                if (cmp(key, tmp)) r = mid - 1;
                else l = mid;
            }
            int sn, other, adj;
            Key curKey;
            readInt(8 + (l - 1) * (KeySize + 4), sn);
            if (l < n) {
                readInt(8 + l * (KeySize + 4), other);
                readKey(8 + (l - 1) * (KeySize + 4) + 4, curKey);
                adj = erase(sn, other, 0, key, curKey);
                if (adj == 1) writeKey(8 + (l - 1) * (KeySize + 4) + 4, newKey);
                ++l;
            }
            else {
                readInt(8 + (l - 2) * (KeySize + 4), other);
                readKey(8 + (l - 2) * (KeySize + 4) + 4, curKey);
                adj = erase(sn, other, 1, key, curKey);
                if (adj == 1) writeKey(8 + (l - 2) * (KeySize + 4) + 4, newKey);
            }
            if (adj == 2) {
                eraseSon(l, n);
                if (cur == rt) {
                    if (n == 1) readInt(8, rt);
                    decTop(cur);
                    return 0;
                }
                if (n < (M + 1 >> 1)) {
                    int m;
                    incTop(bro);
                    readInt(0, m);
                    if (cur != rt && m > (M + 1 >> 1)) {
                        if (flag) {
                            readKey(8 + (m - 2) * (KeySize + 4) + 4, newKey);
                            readInt(8 + (m - 1) * (KeySize + 4), sn);
                            eraseSon(m, m);
                            decTop(bro);
                            readInt(8, other);
                            insertSon(2, faKey, other, n);
                            writeInt(8, sn);
                        }
                        else {
                            readKey(8 + 4, newKey);
                            readInt(8, sn);
                            readInt(8 + (KeySize + 4), other);
                            eraseSon(2, m);
                            writeInt(8, other);
                            decTop(bro);
                            insertSon(n + 1, faKey, sn, n);
                        }
                        decTop(cur);
                        return 1;
                    }
                    else {
                        if (flag) {
                            std::swap(cur, bro);
                            std::swap(n, m);
                            memcpy(buf + (top + 1) * Block, buf + top * Block, Block);
                            memcpy(buf + top * Block, buf + (top - 1) * Block, Block);
                            memcpy(buf + (top - 1) * Block, buf + (top + 1) * Block, Block);
                        }
                        readInt(8, tmpSn[1]);
                        for (int i = 2; i <= m; ++i) {
                            readKey(8 + (i - 2) * (KeySize + 4) + 4, tmpKey[i]);
                            readInt(8 + (i - 1) * (KeySize + 4), tmpSn[i]);
                        }
                        decTop(bro);
                        writeKey(8 + (n - 1) * (KeySize + 4) + 4, faKey);
                        writeInt(8 + n * (KeySize + 4), tmpSn[1]);
                        for (int i = 2; i <= m; ++i) {
                            writeKey(8 + (n + i - 2) * (KeySize + 4) + 4, tmpKey[i]);
                            writeInt(8 + (n + i - 1) * (KeySize + 4), tmpSn[i]);
                        }
                        n += m;
                        writeInt(0, n);
                        decTop(cur);
                        return 2;
                    }
                }
            }
            decTop(cur);
            return 0;
        }
    }
public:
    BPT(std::string fileName, int _KeySize, int _ValSize, bool &isFirst) : KeySize(_KeySize), ValSize(_ValSize) {
        M = (Block - 8 + KeySize) / (KeySize + 4) - 1;
        L = (Block - 12) / (KeySize + ValSize) - 1;
        tmpKey = new Key[std::max(M, L) + 1];
        tmpVal = new T[L + 1];
        tmpSn = new int[M + 1];
        io.open(fileName, std::ios::in | std::ios::out | std::ios::binary);
        top = -1;
        if (!io) {
            std::ofstream outfile;
            outfile.open(fileName);
            outfile.close();
            io.open(fileName, std::ios::in | std::ios::out | std::ios::binary);
            ++top;
            writeInt(0, 0);
            writeInt(4, 0);
            writeInt(Block - 4, 0);
            decTop(0);
        }
        incTop(0);
        readInt(0, rt);
        readInt(4, tot);
        isFirst = !tot;
        readInt(Block - 4, head);
        --top;
    }
    ~BPT() {
        delete [] tmpKey;
        delete [] tmpVal;
        delete [] tmpSn;
        top = 0;
        writeInt(0, rt);
        writeInt(4, tot);
        writeInt(Block - 4, head);
        decTop(0);
        io.close();
    }
    class iterator {
    private:
        int blockAddr, n, i;
        BPT <Key, T> *BPTptr;
        char info[Block];
        friend class BPT <Key, T>;
    public:
        iterator() {
            blockAddr = 0;
            BPTptr = nullptr;
        }
        iterator(const iterator &other) {
            blockAddr = other.blockAddr;
            n = other.n;
            i = other.i;
            BPTptr = other.BPTptr;
            memcpy(info, other.info, Block);
        }
        bool inc() {
            if (!blockAddr) return 0;
            ++i;
            if (i > n) {
                memcpy(reinterpret_cast<char *>(&blockAddr), info + Block - 4, 4);
                if (!blockAddr) return 0;
                BPTptr->io.seekg(blockAddr * Block);
                BPTptr->io.read(info, Block);
                memcpy(reinterpret_cast<char *>(&n), info, 4);
                i = 1;
            }
            return 1;
        }
        bool first(Key &key) {
            if (!blockAddr) return 0;
            memcpy(reinterpret_cast<char *>(&key),
              info + 8 + (i - 1) * (BPTptr->KeySize + BPTptr->ValSize), BPTptr->KeySize);
            return 1;
        }
        bool second(T &val) {
            if (!blockAddr) return 0;
            memcpy(reinterpret_cast<char *>(&val),
              info + 8 + (i - 1) * (BPTptr->KeySize + BPTptr->ValSize) + BPTptr->KeySize, BPTptr->ValSize);
            return 1;
        }
        void modify(const T &val) {
            memcpy(info + 8 + (i - 1) * (BPTptr->KeySize + BPTptr->ValSize) + BPTptr->KeySize,
              reinterpret_cast<const char *>(&val), BPTptr->ValSize);
            BPTptr->io.seekp(blockAddr * Block);
            BPTptr->io.write(info, Block);
        }
    };
    void insert(const Key &key, const T &val) {
        top = -1;
        if (!rt) {
            head = rt = getNewBlock();
            ++top;
            writeInt(0, 1);
            writeInt(4, 1);
            writeKey(8, key);
            writeVal(8 + KeySize, val);
            writeInt(Block - 4, 0);
            decTop(rt);
            return;
        }
        int other = insert(rt, key, val);
        if (other) {
            int newRt = getNewBlock();
            ++top;
            writeInt(0, 2);
            writeInt(4, 0);
            writeInt(8, rt);
            writeInt(8 + (KeySize + 4), other);
            writeKey(8 + 4, newKey);
            rt = newRt;
            decTop(rt);
        }
    }
    void erase(const Key &key) {
        top = -1;
        if (!rt) return;
        erase(rt, 0, 0, key, key);
    }
    pair <int, T> find(const Key &key) {
        top = -1;
        int cur = rt;
        T val;
        pair <int, T> ret(-1, val);
        if (!cur) return ret;
        int n, isLeaf;
        incTop(cur);
        readInt(0, n);
        readInt(4, isLeaf);
        while (!isLeaf) {
            int l = 1, r = n;
            while (l < r) {
                int mid = l + r + 1 >> 1;
                Key tmp;
                readKey(8 + (mid - 2) * (KeySize + 4) + 4, tmp);
                if (tmp < key) l = mid;
                else r = mid - 1;
            }
            readInt(8 + (l - 1) * (KeySize + 4), cur);
            --top;
            incTop(cur);
            readInt(0, n);
            readInt(4, isLeaf);
        }
        int l = 1, r = n;
        Key tmp;
        while (l < r) {
            int mid = l + r + 1 >> 1;
            readKey(8 + (mid - 1) * (KeySize + ValSize), tmp);
            if (tmp < key) l = mid;
            else r = mid - 1;
        }
        readKey(8 + (l - 1) * (KeySize + ValSize), tmp);
        if (tmp != key) {
            if (l < n) ++l;
            else {
                readInt(Block - 4, cur);
                --top;
                incTop(cur);
                if (!cur) return ret;
                readInt(0, n);
                l = 1;
            }
            readKey(8 + (l - 1) * (KeySize + ValSize), tmp);
        }
        if (tmp != key) return ret;
        ret.first = cur * Block + 8 + (l - 1) * (KeySize + ValSize) + KeySize;
        readVal(8 + (l - 1) * (KeySize + ValSize) + KeySize, ret.second);
        return ret;
    }
    void modify(int addr, const char *ch, int len) {
        io.seekp(addr);
        io.write(ch, len);
    }
    void modifyall(int addr, const char *ch, int len) {
        int cur = rt;
        if (!cur) return;
        int n, isLeaf;
        incTop(cur);
        readInt(0, n);
        readInt(4, isLeaf);
        while (!isLeaf) {
            readInt(8, cur);
            --top;
            incTop(cur);
            readInt(0, n);
            readInt(4, isLeaf);
        }
        while (1) {
            for (int i = 1; i <= n; ++i) {
                memcpy(buf + top * Block + 8 + (i - 1) * (KeySize + ValSize) + KeySize + addr, ch, len);
            }
            int tmp = cur;
            readInt(Block - 4, cur);
            decTop(tmp);
            if (!cur) return;
            incTop(cur);
            readInt(0, n);
        }
    }
    void lower_bound(iterator &it, Key key) {
        top = -1;
        int cur = rt;
        T val;
        it.BPTptr = this;
        it.blockAddr = 0;
        if (!cur) return;
        int n, isLeaf;
        incTop(cur);
        readInt(0, n);
        readInt(4, isLeaf);
        while (!isLeaf) {
            int l = 1, r = n;
            while (l < r) {
                int mid = l + r + 1 >> 1;
                Key tmp;
                readKey(8 + (mid - 2) * (KeySize + 4) + 4, tmp);
                if (tmp < key) l = mid;
                else r = mid - 1;
            }
            readInt(8 + (l - 1) * (KeySize + 4), cur);
            --top;
            incTop(cur);
            readInt(0, n);
            readInt(4, isLeaf);
        }
        int l = 1, r = n;
        Key tmp;
        while (l < r) {
            int mid = l + r + 1 >> 1;
            readKey(8 + (mid - 1) * (KeySize + ValSize), tmp);
            if (tmp < key) l = mid;
            else r = mid - 1;
        }
        readKey(8 + (l - 1) * (KeySize + ValSize), tmp);
        if (tmp < key) {
            if (l < n) ++l;
            else {
                readInt(Block - 4, cur);
                --top;
                incTop(cur);
                if (!cur) return;
                readInt(0, n);
                l = 1;
            }
            readKey(8 + (l - 1) * (KeySize + ValSize), tmp);
        }
        it.blockAddr = cur;
        it.n = n;
        it.i = l;
        memcpy(it.info, buf + top * Block, Block);
    }
    void clear() { rt = tot = head = 0; }
    /*void print(const string &ch) {
        top = -1;
        int cur = rt;
        if (!cur) {
            puts("null");
            return;
        }
        int n, isLeaf;
        incTop(cur);
        readInt(0, n);
        readInt(4, isLeaf);
        while (!isLeaf) {
            int l = 1, r = n;
            while (l < r) {
                int mid = l + r + 1 >> 1;
                Key tmp;
                readKey(8 + (mid - 2) * (KeySize + 4) + 4, tmp);
                if (tmp.first < ch) l = mid;
                else r = mid - 1;
            }
            readInt(8 + (l - 1) * (KeySize + 4), cur);
            --top;
            incTop(cur);
            readInt(0, n);
            readInt(4, isLeaf);
        }
        int l = 1, r = n;
        Key tmp;
        while (l < r) {
            int mid = l + r + 1 >> 1;
            readKey(8 + (mid - 1) * (KeySize + ValSize), tmp);
            if (tmp.first < ch) l = mid;
            else r = mid - 1;
        }
        readKey(8 + (l - 1) * (KeySize + ValSize), tmp);
        if (tmp.first != ch) {
            if (l < n) ++l;
            else {
                readInt(Block - 4, cur);
                --top;
                incTop(cur);
                if (!cur) {
                    puts("null");
                    return;
                }
                readInt(0, n);
                l = 1;
            }
            readKey(8 + (l - 1) * (KeySize + ValSize), tmp);
        }
        if (tmp.first != ch) {
            puts("null");
            return;
        }
        while (tmp.first == ch) {
            printf("%d ", tmp.second);
            if (l < n) ++l;
            else {
                readInt(Block - 4, cur);
                --top;
                if (!cur) break;
                incTop(cur);
                readInt(0, n);
                l = 1;
            }
            readKey(8 + (l - 1) * (KeySize + ValSize), tmp);
        }
        puts("");
    }*/
};

#endif