/*
  Unittest for gob_m5s_faces
*/
#include <gob_m5s_faces.hpp>
using goblib::m5s::FaceGB;
#include <gob_utility.hpp> // goblib::size

#include "gtest/gtest.h"

constexpr FaceGB::Button BtnUp = FaceGB::Button::Up;
constexpr FaceGB::Button BtnDown = FaceGB::Button::Down;
constexpr FaceGB::Button BtnLeft = FaceGB::Button::Left;
constexpr FaceGB::Button BtnRight = FaceGB::Button::Right;
constexpr FaceGB::Button BtnA = FaceGB::Button::A;
constexpr FaceGB::Button BtnB = FaceGB::Button::B;
constexpr FaceGB::Button BtnSelect = FaceGB::Button::Select;
constexpr FaceGB::Button BtnStart = FaceGB::Button::Start;



TEST(Faces, Gameboy)
{
    FaceGB gb;
    gb.setup();

    FaceGB::ButtonType stub_data[] =
            {
                BtnA | BtnUp | BtnSelect,
                BtnA | BtnUp | BtnRight,
                BtnA | BtnUp,
                BtnA | BtnUp,
                BtnA,
                BtnA,
                BtnStart,
                0,
                0
            };
    gb.setRepeatCycleAll(2);
    gb.setRepeatCycle(BtnA, 4);

    gb.setStubData(stub_data, goblib::size(stub_data));

    // previous pump().
    EXPECT_FALSE(gb.isPressed(FaceGB::MASK_ALL));
    EXPECT_FALSE(gb.wasPressed(FaceGB::MASK_ALL));
    EXPECT_FALSE(gb.wasReleased(FaceGB::MASK_ALL));
    EXPECT_FALSE(gb.wasRepeated(FaceGB::MASK_ALL));
    EXPECT_FALSE(gb.isPressedEqual(FaceGB::MASK_ALL));
    EXPECT_FALSE(gb.wasPressedEqual(FaceGB::MASK_ALL));
    EXPECT_FALSE(gb.wasReleasedEqual(FaceGB::MASK_ALL));
    EXPECT_FALSE(gb.wasRepeatedEqual(FaceGB::MASK_ALL));

    // BtnA | BtnUp | BtnSelect
    {
        constexpr FaceGB::ButtonType btn_0 = (BtnA | BtnUp | BtnSelect);
        gb.pump();

        EXPECT_TRUE(gb.isPressed(btn_0));
        EXPECT_TRUE(gb.isPressed(BtnA));
        EXPECT_TRUE(gb.isPressed(BtnUp));
        EXPECT_TRUE(gb.isPressed(BtnSelect));
        EXPECT_FALSE(gb.isPressed(btn_0 ^ 0xFF));

        EXPECT_TRUE(gb.isPressedEqual(btn_0));
        EXPECT_FALSE(gb.isPressedEqual(BtnA));
        EXPECT_FALSE(gb.isPressedEqual(BtnUp));
        EXPECT_FALSE(gb.isPressedEqual(BtnSelect));

        EXPECT_TRUE(gb.wasPressed(btn_0));
        EXPECT_TRUE(gb.wasPressed(BtnA));
        EXPECT_TRUE(gb.wasPressed(BtnUp));
        EXPECT_TRUE(gb.wasPressed(BtnSelect));
        EXPECT_FALSE(gb.wasPressed(btn_0 ^ 0xFF));

        EXPECT_TRUE(gb.wasPressedEqual(btn_0));
        EXPECT_FALSE(gb.wasPressedEqual(BtnA));
        EXPECT_FALSE(gb.wasPressedEqual(BtnUp));
        EXPECT_FALSE(gb.wasPressedEqual(BtnSelect));

        EXPECT_FALSE(gb.wasReleased(btn_0));
        EXPECT_FALSE(gb.wasReleased(BtnA));
        EXPECT_FALSE(gb.wasReleased(BtnUp));
        EXPECT_FALSE(gb.wasReleased(BtnSelect));

        EXPECT_FALSE(gb.wasReleasedEqual(btn_0));
        EXPECT_FALSE(gb.wasReleasedEqual(BtnA));
        EXPECT_FALSE(gb.wasReleasedEqual(BtnUp));
        EXPECT_FALSE(gb.wasReleasedEqual(BtnSelect));

        EXPECT_TRUE(gb.wasRepeated(btn_0));
        EXPECT_TRUE(gb.wasRepeated(BtnA));
        EXPECT_TRUE(gb.wasRepeated(BtnUp));
        EXPECT_TRUE(gb.wasRepeated(BtnSelect));
        EXPECT_FALSE(gb.wasRepeated(btn_0 ^ 0xFF));

        EXPECT_TRUE(gb.wasRepeatedEqual(btn_0));
        EXPECT_FALSE(gb.wasRepeatedEqual(BtnA));
        EXPECT_FALSE(gb.wasRepeatedEqual(BtnUp));
        EXPECT_FALSE(gb.wasRepeatedEqual(BtnSelect));
    }

    // BtnA | BtnUp | BtnRight
    {
        constexpr FaceGB::ButtonType btn_1 = (BtnA | BtnUp | BtnRight);
        gb.pump();

        //  printf("N:%X E:%X ER:%X RP:%X\n", gb.getNow(), gb.getEdge(), gb.getReleaseEdge(), gb.getRepeat());
    
        EXPECT_TRUE(gb.isPressed(btn_1));
        EXPECT_TRUE(gb.isPressed(BtnA));
        EXPECT_TRUE(gb.isPressed(BtnUp));
        EXPECT_TRUE(gb.isPressed(BtnRight));
        EXPECT_FALSE(gb.isPressed(btn_1 ^ 0xFF));

        EXPECT_TRUE(gb.isPressedEqual(btn_1));
        EXPECT_FALSE(gb.isPressedEqual(BtnA));
        EXPECT_FALSE(gb.isPressedEqual(BtnUp));
        EXPECT_FALSE(gb.isPressedEqual(BtnRight));

        EXPECT_TRUE(gb.wasPressed(btn_1));
        EXPECT_FALSE(gb.wasPressed(BtnA));
        EXPECT_FALSE(gb.wasPressed(BtnUp));
        EXPECT_TRUE(gb.wasPressed(BtnRight));
        EXPECT_FALSE(gb.wasPressed(btn_1 ^ 0xFF));

        EXPECT_FALSE(gb.wasReleased(btn_1));
        EXPECT_FALSE(gb.wasReleased(BtnA));
        EXPECT_FALSE(gb.wasReleased(BtnUp));
        EXPECT_FALSE(gb.wasReleased(BtnRight));
        EXPECT_TRUE(gb.wasReleased(btn_1 ^ 0xFF));

        EXPECT_FALSE(gb.wasReleasedEqual(btn_1));
        EXPECT_FALSE(gb.wasReleasedEqual(BtnA));
        EXPECT_FALSE(gb.wasReleasedEqual(BtnUp));
        EXPECT_FALSE(gb.wasReleasedEqual(BtnRight));
        EXPECT_TRUE(gb.wasReleasedEqual(BtnSelect));

        EXPECT_TRUE(gb.wasRepeated(btn_1));
        EXPECT_FALSE(gb.wasRepeated(BtnA));
        EXPECT_FALSE(gb.wasRepeated(BtnUp));
        EXPECT_TRUE(gb.wasRepeated(BtnRight));
        EXPECT_FALSE(gb.wasRepeated(BtnSelect));

        EXPECT_FALSE(gb.wasRepeatedEqual(btn_1));
        EXPECT_FALSE(gb.wasRepeatedEqual(BtnA));
        EXPECT_FALSE(gb.wasRepeatedEqual(BtnUp));
        EXPECT_TRUE(gb.wasRepeatedEqual(BtnRight));
        EXPECT_FALSE(gb.wasRepeatedEqual(BtnSelect));
    }

    // BtnA | BtnUp 
    {
        constexpr FaceGB::ButtonType btn_2 = (BtnA | BtnUp);
        gb.pump();

        //printf("N:%X E:%X ER:%X RP:%X\n", gb.getNow(), gb.getEdge(), gb.getReleaseEdge(), gb.getRepeat());
    
        EXPECT_TRUE(gb.isPressed(btn_2));
        EXPECT_TRUE(gb.isPressed(BtnA));
        EXPECT_TRUE(gb.isPressed(BtnUp));
        EXPECT_FALSE(gb.isPressed(btn_2 ^ 0xFF));

        EXPECT_TRUE(gb.isPressedEqual(btn_2));
        EXPECT_FALSE(gb.isPressedEqual(BtnA));
        EXPECT_FALSE(gb.isPressedEqual(BtnUp));

        EXPECT_FALSE(gb.wasPressed(FaceGB::MASK_ALL));

        EXPECT_FALSE(gb.wasReleased(btn_2));
        EXPECT_TRUE(gb.wasReleased(btn_2 ^ 0xFF));
        EXPECT_TRUE(gb.wasReleasedEqual(BtnRight));

        EXPECT_FALSE(gb.wasRepeated(btn_2));
        EXPECT_FALSE(gb.wasRepeated(BtnUp));
        EXPECT_FALSE(gb.wasRepeated(BtnA));
    }

    // BtnA | BtnUp 
    {
        constexpr FaceGB::ButtonType btn_3 = (BtnA | BtnUp);
        gb.pump();

        // printf("N:%X E:%X ER:%X RP:%X\n", gb.getNow(), gb.getEdge(), gb.getReleaseEdge(), gb.getRepeat());
    
        EXPECT_TRUE(gb.isPressed(btn_3));
        EXPECT_TRUE(gb.isPressed(BtnA));
        EXPECT_TRUE(gb.isPressed(BtnUp));
        EXPECT_FALSE(gb.isPressed(btn_3 ^ 0xFF));

        EXPECT_TRUE(gb.isPressedEqual(btn_3));
        EXPECT_FALSE(gb.isPressedEqual(BtnA));
        EXPECT_FALSE(gb.isPressedEqual(BtnUp));

        EXPECT_FALSE(gb.wasPressed(FaceGB::MASK_ALL));
        EXPECT_FALSE(gb.wasReleased(FaceGB::MASK_ALL));

        EXPECT_TRUE(gb.wasRepeated(BtnUp));
        EXPECT_TRUE(gb.wasRepeatedEqual(BtnUp));
        EXPECT_FALSE(gb.wasRepeated(BtnA));
    }

    // BtnA
    {
        constexpr FaceGB::ButtonType btn_4 = (BtnA);
        gb.pump();

        // printf("N:%X E:%X ER:%X RP:%X\n", gb.getNow(), gb.getEdge(), gb.getReleaseEdge(), gb.getRepeat());
    
        EXPECT_TRUE(gb.isPressed(btn_4));
        EXPECT_TRUE(gb.isPressed(BtnA));
        EXPECT_FALSE(gb.isPressed(btn_4 ^ 0xFF));

        EXPECT_TRUE(gb.isPressedEqual(btn_4));

        EXPECT_FALSE(gb.wasPressed(FaceGB::MASK_ALL));

        EXPECT_TRUE(gb.wasReleased(BtnUp));
        EXPECT_FALSE(gb.wasReleased(BtnUp ^ 0xFF));
        EXPECT_TRUE(gb.wasReleasedEqual(BtnUp));

        EXPECT_FALSE(gb.wasRepeated(BtnA));
    }

    // BtnA
    {
        constexpr FaceGB::ButtonType btn_5 = (BtnA);
        gb.pump();

        // printf("N:%X E:%X ER:%X RP:%X\n", gb.getNow(), gb.getEdge(), gb.getReleaseEdge(), gb.getRepeat());
    
        EXPECT_TRUE(gb.isPressed(btn_5));
        EXPECT_TRUE(gb.isPressed(BtnA));
        EXPECT_FALSE(gb.isPressed(btn_5 ^ 0xFF));

        EXPECT_TRUE(gb.isPressedEqual(btn_5));

        EXPECT_FALSE(gb.wasPressed(FaceGB::MASK_ALL));
        EXPECT_FALSE(gb.wasReleased(FaceGB::MASK_ALL));

        EXPECT_TRUE(gb.wasRepeated(BtnA));
        EXPECT_TRUE(gb.wasRepeatedEqual(BtnA));
    }


    // BtnStart
    {
        constexpr FaceGB::ButtonType btn_6 = (BtnStart);
        gb.pump();

        //  printf("N:%X E:%X ER:%X RP:%X\n", gb.getNow(), gb.getEdge(), gb.getReleaseEdge(), gb.getRepeat());
    
        EXPECT_TRUE(gb.isPressed(btn_6));
        EXPECT_FALSE(gb.isPressed(btn_6 ^ 0xFF));

        EXPECT_TRUE(gb.isPressedEqual(btn_6));

        EXPECT_TRUE(gb.wasPressed(btn_6));
        EXPECT_TRUE(gb.wasReleased(btn_6 ^ 0xFF));
        EXPECT_TRUE(gb.wasReleasedEqual(BtnA));

        EXPECT_TRUE(gb.wasRepeated(FaceGB::MASK_ALL));
        EXPECT_TRUE(gb.wasRepeatedEqual(BtnStart));
    }

    // 0
    {
        gb.pump();
    
        EXPECT_FALSE(gb.isPressed(FaceGB::MASK_ALL));
        EXPECT_FALSE(gb.wasPressed(FaceGB::MASK_ALL));
        EXPECT_TRUE(gb.wasReleased(FaceGB::MASK_ALL));
        EXPECT_TRUE(gb.wasReleasedEqual(BtnStart));
        EXPECT_FALSE(gb.wasRepeated(FaceGB::MASK_ALL));
    }

    // 0
    {
        gb.pump();
    
        EXPECT_FALSE(gb.isPressed(FaceGB::MASK_ALL));
        EXPECT_FALSE(gb.wasPressed(FaceGB::MASK_ALL));
        EXPECT_FALSE(gb.wasReleased(FaceGB::MASK_ALL));
        EXPECT_FALSE(gb.wasRepeated(FaceGB::MASK_ALL));
    }

}


