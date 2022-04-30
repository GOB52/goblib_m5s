/*
 */
#include <gob_task.hpp>
using goblib::Task;
using goblib::TaskTree;
using goblib::TaskMessage;

#include "gtest/gtest.h"

#include <algorithm> // std::all_of
#include <numeric> // std::accumulate

class CounterTask : public Task
{
  public:
    CounterTask() : CounterTask(128,"dc") {}
    CounterTask(Task::PriorityType pri, const char* tag) : Task(pri, tag), _counter(0) {}

    std::int32_t value() const { return _counter; }

  protected:
    virtual bool onInitialize() override { _counter = _release_wait = 0; return true; }
    virtual bool onRelease() override { return _release_wait-- <= 0; }
    virtual void onExecute(const float) override { ++_counter; _release_wait = _counter; }
    virtual void onReceive(const TaskMessage& msg) override
    {
        _counter = static_cast<std::int32_t>(msg.msg);

    }
  private:
    std::int32_t _counter;
    std::int32_t _release_wait;

    template<typename> friend class TaskTree;
};

class CTaskTree : public TaskTree<CounterTask>
{
  public:
    CTaskTree() : TaskTree() {}
};



TEST(Task, Basic)
{
    constexpr auto task_size = 10;
    CTaskTree tree;
    std::vector<CounterTask*> vec;

    auto acc_func = [](int acc, const CounterTask* t) { return acc + (t->isKill() ? 0 : t->value()); };
    
    EXPECT_EQ(0, tree.size());
    for(int i=0;i<task_size;++i)
    {
        vec.push_back(new CounterTask( 256 - i, "test_task"));
    }
    EXPECT_TRUE(std::all_of(vec.begin(), vec.end(), [](CounterTask* t) { return t->status() == Task::Status::Initialize; }));

    for(auto& e : vec)
    {
        tree.insertNode(e);
    }
    EXPECT_EQ(vec.size(), tree.size());

    //tree.print();

    // Do Task::onInitialize
    tree.pump();
    EXPECT_TRUE(std::all_of(vec.begin(), vec.end(), [](CounterTask* t) { return t->status() == Task::Status::Execute; }));
    EXPECT_TRUE(std::all_of(vec.begin(), vec.end(), [](CounterTask* t) { return t->value() == 0; }));

    // Do Task::onExecute
    tree.pump();
    EXPECT_TRUE(std::all_of(vec.begin(), vec.end(), [](CounterTask* t) { return t->value() == 1; }));
    EXPECT_EQ(task_size*1, std::accumulate(vec.begin(), vec.end(), 0, acc_func));
    
    // Pause(local)
    tree.pause();
    tree.pump();
    EXPECT_TRUE(std::all_of(vec.begin(), vec.end(), [](CounterTask* t) { return t->isPause(); }));
    EXPECT_TRUE(std::all_of(vec.begin(), vec.end(), [](CounterTask* t) { return t->value() == 1; }));
    EXPECT_EQ(task_size*1, std::accumulate(vec.begin(), vec.end(), 0, acc_func));
    vec[0]->resume(); // resume 1 task
    EXPECT_FALSE(std::all_of(vec.begin(), vec.end(), [](CounterTask* t) { return t->isPause(); }));
    tree.pump(); // vec[0] : 2
    EXPECT_EQ(task_size*1+1, std::accumulate(vec.begin(), vec.end(), 0, acc_func));
    tree.resume();
    EXPECT_TRUE(std::all_of(vec.begin(), vec.end(), [](CounterTask* t) { return !t->isPause(); }));
    
    // Pause(global))
    tree.pauseGlobal();
    EXPECT_TRUE(std::all_of(vec.begin(), vec.end(), [](CounterTask* t) { return !t->isPause(); }));
    tree.pump();
    EXPECT_EQ(task_size*1+1, std::accumulate(vec.begin(), vec.end(), 0, acc_func));
    vec[0]->resume(); // resume 1 task but pause global
    tree.pump();
    EXPECT_EQ(task_size*1+1, std::accumulate(vec.begin(), vec.end(), 0, acc_func));
    tree.resumeGlobal();

    // vec[0] Restart (release 2 pump, init 1 pump)
    vec[0]->restart();
    tree.pump();
    EXPECT_EQ((task_size-1)*2 + 2, std::accumulate(vec.begin(), vec.end(), 0, acc_func));
    EXPECT_EQ(vec[0]->status(), Task::Status::Restart);
    tree.pump();
    EXPECT_EQ(vec[0]->status(), Task::Status::Restart);
    EXPECT_EQ((task_size-1)*3 + 2, std::accumulate(vec.begin(), vec.end(), 0, acc_func));
    tree.pump(); // vec[0] oninit
    EXPECT_EQ(0, vec[0]->value());
    EXPECT_EQ((task_size-1)*4, std::accumulate(vec.begin(), vec.end(), 0, acc_func));
    EXPECT_EQ(vec[0]->status(), Task::Status::Execute);

    // Do vec[0] onRelease (need 1 pump)
    vec[0]->release();
    tree.pump();
    EXPECT_EQ(0, vec[0]->value());
    EXPECT_TRUE(vec[0]->isKill());
    EXPECT_EQ((task_size-1)*5, std::accumulate(vec.begin(), vec.end(), 0, acc_func));

    // unlink vec[0] (onRelease -> kill)
    tree.pump();
    EXPECT_EQ(task_size-1, tree.size());
    EXPECT_EQ((task_size-1)*6, std::accumulate(vec.begin(), vec.end(), 0, acc_func));

    // Kill unlink vec[5]
    vec[5]->kill();
    tree.pump();
    EXPECT_EQ(task_size-2, tree.size());
    EXPECT_EQ((task_size-2)*7, std::accumulate(vec.begin(), vec.end(), 0, acc_func));

    // Clear
    tree.clear();
    EXPECT_EQ(0, tree.size());

    for(auto& e : vec)
    {
        delete e;
    }
}


TEST(Task, Message)
{
    constexpr auto task_size = 10;
    CTaskTree tree;
    std::vector<CounterTask*> vec;

    auto acc_func = [](int acc, const CounterTask* t) { return acc + t->value(); };
    
    EXPECT_EQ(0, tree.size());
    for(int i=0;i<task_size;++i)
    {
        vec.push_back(new CounterTask(256 - i, "test_task"));
    }
    EXPECT_TRUE(std::all_of(vec.begin(), vec.end(), [](CounterTask* t) { return t->status() == Task::Status::Initialize; }));

    int i = 0;
    for(auto& e : vec)
    {
        tree.insertNode(e, (i&1)==0 ? nullptr : vec.front());
        ++i;
    }
    //    tree.print();
    EXPECT_EQ(vec.size(), tree.size());

    tree.pump();
    EXPECT_TRUE(std::all_of(vec.begin(), vec.end(), [](CounterTask* t) { return t->status() == Task::Status::Execute; }));
    EXPECT_TRUE(std::all_of(vec.begin(), vec.end(), [](CounterTask* t) { return t->value() == 0; }));
    
    TaskMessage value_zero(0);
    TaskMessage value_100(100);

    tree.pump();
    EXPECT_EQ(task_size, std::accumulate(vec.begin(), vec.end(), 0, acc_func));

    tree.sendMessage(value_zero, vec[4]);
    tree.postMessage(value_100, vec[4]);
    EXPECT_EQ(1, tree.undelivered());
    EXPECT_EQ((task_size-1)*1, std::accumulate(vec.begin(), vec.end(), 0, acc_func));
    
    tree.pump();
    EXPECT_EQ(0, tree.undelivered());
    EXPECT_EQ((task_size-1)*2 + 101, std::accumulate(vec.begin(), vec.end(), 0, acc_func));

    tree.sendBroadcastMessage(value_100);
    tree.postBroadcastMessage(value_zero);
    EXPECT_EQ(1, tree.undeliveredBroadcast());
    EXPECT_EQ((task_size)*100, std::accumulate(vec.begin(), vec.end(), 0, acc_func));

    tree.pump();
    EXPECT_EQ(0, tree.undeliveredBroadcast());
    EXPECT_EQ(task_size*1, std::accumulate(vec.begin(), vec.end(), 0, acc_func));

    tree.postMessage(value_100, vec[4]);
    vec[4]->pause(true);
    tree.pump();
    EXPECT_EQ(0, tree.undelivered());
    EXPECT_EQ((task_size-1) * 2 + 100, std::accumulate(vec.begin(), vec.end(), 0, acc_func));

    tree.postMessage(value_100, vec[4]);
    tree.postMessage(value_zero, vec[4]);
    EXPECT_EQ(2, tree.undelivered());
    vec[4]->pause(true);
    tree.pump();
    EXPECT_EQ(0, tree.undelivered());
    EXPECT_EQ((task_size-1)*3 + 0, std::accumulate(vec.begin(), vec.end(), 0, acc_func));

    tree.postBroadcastMessage(value_zero);
    tree.pause();
    EXPECT_TRUE(std::all_of(vec.begin(), vec.end(), [](CounterTask* t) { return t->isPause(); }));
    tree.pump();
    EXPECT_EQ(0, tree.undeliveredBroadcast());
    EXPECT_EQ(0, std::accumulate(vec.begin(), vec.end(), 0, acc_func));

    tree.postBroadcastMessage(value_100);
    tree.resume();
    EXPECT_TRUE(std::all_of(vec.begin(), vec.end(), [](CounterTask* t) { return !t->isPause(); }));
    tree.pump();
    EXPECT_EQ(0, tree.undeliveredBroadcast());
    EXPECT_EQ(task_size * 101, std::accumulate(vec.begin(), vec.end(), 0, acc_func));

    
    tree.sendBroadcastMessage(value_zero, vec.front());
    EXPECT_EQ((task_size - 6) * 101, std::accumulate(vec.begin(), vec.end(), 0, acc_func));
    tree.pump();
    EXPECT_EQ((task_size - 6) * 102 + 6 * 1, std::accumulate(vec.begin(), vec.end(), 0, acc_func));

    tree.postBroadcastMessage(value_100, vec.front());
    EXPECT_EQ((task_size - 6) * 102 + 6 * 1, std::accumulate(vec.begin(), vec.end(), 0, acc_func));
    tree.pump();
    EXPECT_EQ((task_size - 6) * 103 + 6 * 101, std::accumulate(vec.begin(), vec.end(), 0, acc_func));
}
