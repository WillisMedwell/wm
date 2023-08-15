#include <Sov.hpp>
#include <gtest/gtest.h>

size_t element_constructions = 0;
size_t element_destructions = 0;

struct Element {
    int64_t dummy;
    Element(int arg = 0)
    {
        dummy = arg;
        ++element_constructions;
    }
    Element(const Element& other)
    {
        dummy = other.dummy;
        ++element_constructions;
    }
    Element(Element&& other)
    {
        dummy = other.dummy;
        ++element_constructions;
    }
    ~Element()
    {
        ++element_destructions;
    }
};

TEST(SovTest, Construction)
{
    Sov<int> sov(2);
    EXPECT_EQ(sov.size(), 0);
}

TEST(SovTest, PushConstRef)
{
    Sov<Element> sov(4);
    Element e0(0);
    Element e1(1);
    Element e2(2);
    Element e3(3);
    sov.pushBack(e0);
    sov.pushBack(e1);
    sov.pushBack(e2);
    sov.pushBack(e3);
    EXPECT_EQ(sov.field<0>()[0].dummy, 0);
    EXPECT_EQ(sov.field<0>()[1].dummy, 1);
    EXPECT_EQ(sov.field<0>()[2].dummy, 2);
    EXPECT_EQ(sov.field<0>()[3].dummy, 3);
}
TEST(SovTest, PushMove)
{
    Sov<Element> sov(4);
    sov.pushBack(Element { 0 });
    sov.pushBack(Element { 1 });
    sov.pushBack(Element { 2 });
    sov.pushBack(Element { 3 });
    EXPECT_EQ(sov.field<0>()[0].dummy, 0);
    EXPECT_EQ(sov.field<0>()[1].dummy, 1);
    EXPECT_EQ(sov.field<0>()[2].dummy, 2);
    EXPECT_EQ(sov.field<0>()[3].dummy, 3);
}

TEST(SovTest, PushConstRefWGrow)
{
    Sov<Element> sov(2);
    Element e0(0);
    Element e1(1);
    Element e2(2);
    Element e3(3);
    sov.pushBack(e0);
    sov.pushBack(e1);
    sov.pushBack(e2);
    sov.pushBack(e3);
    EXPECT_EQ(sov.field<0>()[0].dummy, 0);
    EXPECT_EQ(sov.field<0>()[1].dummy, 1);
    EXPECT_EQ(sov.field<0>()[2].dummy, 2);
    EXPECT_EQ(sov.field<0>()[3].dummy, 3);
}
TEST(SovTest, PushMoveWGrow)
{
    Sov<Element> sov(2);
    sov.pushBack(Element { 0 });
    sov.pushBack(Element { 1 });
    sov.pushBack(Element { 2 });
    sov.pushBack(Element { 3 });
    EXPECT_EQ(sov.field<0>()[0].dummy, 0);
    EXPECT_EQ(sov.field<0>()[1].dummy, 1);
    EXPECT_EQ(sov.field<0>()[2].dummy, 2);
    EXPECT_EQ(sov.field<0>()[3].dummy, 3);
}

TEST(SovTest, ProperElementConstruction)
{
    {
        Sov<Element> sov(2);
        Element e;
        sov.pushBack(e);
        sov.pushBack(Element {});
    }
    EXPECT_EQ(element_constructions, element_destructions);
}

TEST(SovTest, ProperElementConstructionWGrow)
{
    {
        Sov<Element> sov(2);
        Element e;
        sov.pushBack(e);
        sov.pushBack(Element {});
        sov.pushBack(e);
        sov.pushBack(Element {});
    }
    EXPECT_EQ(element_constructions, element_destructions);
}

TEST(SovTest, ElementAccess)
{
    Sov<uint8_t, std::string, uint16_t> sov(1);
    sov.pushBack(0, "first", 1);
    sov.pushBack(0, "second", 2);
    auto [a, b, c] = sov[0];
    EXPECT_EQ(a, 0);
    EXPECT_EQ(b, "first");
    EXPECT_EQ(c, 1);

    auto [d, e, f] = sov[1];
    EXPECT_EQ(d, 0);
    EXPECT_EQ(e, "second");
    EXPECT_EQ(f, 2);

    EXPECT_THROW(sov.at(2), std::out_of_range);
}