/*
  Unittest for gob_tree
*/

#include <gob_tree.hpp>
#include "gtest/gtest.h"

namespace
{

#if 0
class NoLessOperator : public goblib::Node {};
class CompilerErrorTree : public goblib::FamilyTree<std::int32_t> {}; // NG
class CompilerErrorTree2 : public goblib::FamilyTree<NoLessOperator> {}; // NG
#endif

class IntNode : public goblib::Node
{
  public:
    IntNode() : Node(), _value(0) {}
    explicit IntNode(std::int32_t v) : Node(), _value(v) {}
    bool operator<(const IntNode& o) { return _value < o._value; }

    std::int32_t value() const { return _value; }
  private:
    std::int32_t _value;
};

class TestTree: public goblib::FamilyTree<IntNode>
{
  public:
    TestTree() : goblib::FamilyTree<IntNode>() {}
    void print()
    {
        callback_with_depth(_print_node);
    }

  private:
    static void _print_node(const IntNode* c,const std::uint32_t depth)
    {
        printf("%*c[%04x]\n", depth*2, ' ', c->value());
    }
};


IntNode c_1000(0x1000);
IntNode c_2000(0x2000);
IntNode c_2100(0x2100);
IntNode c_2200(0x2200);
IntNode c_2300(0x2300);
IntNode c_2310(0x2310);
IntNode c_3000(0x3000);
IntNode c_3100(0x3100);
IntNode c_3110(0x3110);
IntNode c_3111(0x3111);
IntNode c_3112(0x3112);
IntNode c_3200(0x3200);
IntNode c_3300(0x3300);
}

/*
  root
  |
  c_1000 - c2000 --------------- c3000
  |                     |
  c2100 - c2200 - c2300 |
  |     |
  c2310 |
  c3100 - c3200 - c3300
  |
  c3110
  |
  c3111 - c3112
*/

TEST(Tree, Basic)
{
    TestTree tree;

    EXPECT_EQ(tree.size(), 0);
    EXPECT_FALSE(tree.exists(&c_2000));
    
    tree.reserveInsertNode(&c_2000);
    tree.insertReservedNodes();

    EXPECT_TRUE(tree.exists(&c_2000));
    EXPECT_EQ(tree.size(), 1);
    
    tree.reserveInsertNode(&c_3000);
    tree.reserveInsertNode(&c_1000);

    tree.reserveInsertNode(&c_2300,&c_2000);
    tree.reserveInsertNode(&c_2200,&c_2000);
    tree.reserveInsertNode(&c_2100,&c_2000);
    tree.insertReservedNodes();
    EXPECT_EQ(tree.size(), 6);
    
    EXPECT_TRUE(tree.exists(&c_2300));
    EXPECT_TRUE(tree.exists(&c_2300, &c_2000));
    EXPECT_TRUE(tree.exists(&c_2300, &c_1000));
    EXPECT_FALSE(tree.exists(&c_2300, &c_3000));
    
    tree.reserveInsertNode(&c_3100,&c_3000);
    tree.reserveInsertNode(&c_3110,&c_3100);
    tree.reserveInsertNode(&c_3111,&c_3110);
    tree.reserveInsertNode(&c_3112,&c_3110);

    tree.reserveInsertNode(&c_3200,&c_3000);
    tree.reserveInsertNode(&c_3300,&c_3000);
        
    tree.insertReservedNodes();
    EXPECT_EQ(tree.size(), 12);

    tree.removeNode(&c_2300);
    EXPECT_FALSE(tree.exists(&c_2300));
    EXPECT_EQ(tree.size(), 11);

    tree.removeNode(&c_3111);
    EXPECT_FALSE(tree.exists(&c_3111));
    EXPECT_EQ(tree.size(), 10);

    tree.removeNode(&c_2000);
    EXPECT_FALSE(tree.exists(&c_2000));
    EXPECT_EQ(tree.size(), 9);


    tree.removeNode(&c_3112);
    EXPECT_FALSE(tree.exists(&c_3112));
    EXPECT_EQ(tree.size(), 8);

    
    auto sz = tree.size();
    IntNode nolink(1234);
    tree.removeNode(&nolink);
    EXPECT_EQ(sz, tree.size());

    tree.clear();
    EXPECT_EQ(0, tree.size());
    
}

