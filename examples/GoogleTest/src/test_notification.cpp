
#include <gob_notification.hpp>

#include "gtest/gtest.h"
using goblib::Observer;
using goblib::Subject;

class ReceiverInt;
class ReceiverString;


// 
struct SenderA : public Subject<SenderA>
{
    explicit SenderA(int v) : int_value(v) {}
    void set(int v) { int_value = v; }
    int get() const { return int_value; }
  private:
    int int_value;
};

struct SenderB : public Subject<SenderB>
{
    explicit SenderB(const char* p) : _ptr(p) {}
    void set(const char* p) { _ptr = p; }
    const char* get() const { return _ptr; }
  private:
    const char* _ptr;
};

//
struct ReceiverA : public Observer<SenderA>
{
    ReceiverA() : v(0) {}
    virtual void onNotify(const SenderA* s,void*) { v = s->get(); }
    int v;
};

struct ReceiverB : public Observer<SenderB>
{
    ReceiverB() : str(nullptr) {}
    virtual void onNotify(const SenderB* s,void*) { str = s->get(); }
    const char* str;
};

struct ReceiverAB : public Observer<SenderA>, Observer<SenderB>
{
    ReceiverAB() : v(0), str(nullptr) {}
    virtual void onNotify(const SenderA* s,void*) { v = s->get(); }
    virtual void onNotify(const SenderB* s,void*) { str = s->get(); }
    int v;
    const char* str;
};


TEST(Notififcation, Basic)
{
    static const char* str = "TEST";
    static const char* str2 = "XYZ";
    
    SenderA sa(55);
    SenderB sb(str);
    
    ReceiverA ra;
    ReceiverB rb;
    ReceiverAB rab;

    EXPECT_EQ(0, ra.v);
    EXPECT_EQ(nullptr, rb.str);
    EXPECT_EQ(0, rab.v);
    EXPECT_EQ(nullptr, rab.str);

    sa.notify();
    sb.notify();
    
    EXPECT_EQ(0, ra.v);
    EXPECT_EQ(nullptr, rb.str);
    EXPECT_EQ(0, rab.v);
    EXPECT_EQ(nullptr, rab.str);
    
    sa.insertObserver(ra);
    sa.insertObserver(rab);
    sa.notify();
    sb.notify();

    EXPECT_EQ(55, ra.v);
    EXPECT_EQ(nullptr, rb.str);
    EXPECT_EQ(55, rab.v);
    EXPECT_EQ(nullptr, rab.str);

    sa.set(123);
    sa.removeObserver(ra);
    sa.notify();
    sb.notify();
    
    EXPECT_EQ(55, ra.v);
    EXPECT_EQ(nullptr, rb.str);
    EXPECT_EQ(123, rab.v);
    EXPECT_EQ(nullptr, rab.str);

    sa.set(456);
    sb.insertObserver(rb);
    sb.insertObserver(rab);
    sa.notify();
    sb.notify();

    EXPECT_EQ(55, ra.v);
    EXPECT_TRUE(strcmp("TEST", rb.str)==0);
    EXPECT_EQ(456, rab.v);
    EXPECT_TRUE(strcmp("TEST", rab.str)==0);

    sb.set(str2);
    sa.set(987);
    sa.clearObservers();
    sb.removeObserver(rab);
    ra.v = 111;
    rab.v = 111;
    sa.notify();
    sb.notify();

    EXPECT_EQ(111, ra.v);
    EXPECT_TRUE(strcmp("XYZ", rb.str)==0);
    EXPECT_EQ(111, rab.v);
    EXPECT_TRUE(strcmp("TEST", rab.str)==0);

    
}
