/*
  Unittest for stream
*/

#include <gob_memory_stream.hpp>
#include <gob_utility.hpp> // goblib::size
#include <gob_endianness.hpp> // goblib::endian
#include <gob_pcm_stream.hpp>

#if defined(ARDUINO)
#include <gob_m5s_sd.hpp>
#include <gob_m5s_stream.hpp>
#include <gob_debug.hpp>
#endif

#include "gtest/gtest.h"


TEST(Stream, MemoryStream)
{
    //    printf("little:%d big:%d other:%d\n", goblib::endian::little, goblib::endian::big, goblib::endian::other);
    {
        char buffer[] = "abcdefghijklmnopqrstuvwxyz";
        goblib::MemoryStream ss(buffer, sizeof(buffer)-1);

        EXPECT_TRUE(ss.is_open());

        EXPECT_TRUE(ss.seek(0, goblib::seekdir::end));
        EXPECT_EQ(26, ss.position());

        EXPECT_TRUE(ss.seek(0, goblib::seekdir::beg));
        EXPECT_EQ(0, ss.position());
        EXPECT_FALSE(ss.seek(-123, goblib::seekdir::beg));
        EXPECT_EQ(0, ss.position());
    
        auto d8 = ss.read8();
        EXPECT_EQ(d8, 'a');
        EXPECT_EQ(1, ss.position());

        EXPECT_TRUE(ss.seek(5, goblib::seekdir::cur));
        d8 = ss.read8();
        EXPECT_EQ(d8, 'g');
        EXPECT_EQ(7, ss.position());

        EXPECT_TRUE(ss.seek(-7, goblib::seekdir::cur));
        EXPECT_EQ(0, ss.position());
        EXPECT_FALSE(ss.seek(-7, goblib::seekdir::cur));
        EXPECT_EQ(0, ss.position());

        d8 = ss.read8();
        EXPECT_EQ(d8, 'a');
        EXPECT_EQ(1, ss.position());

        EXPECT_FALSE(ss.seek(1000, goblib::seekdir::cur));
        EXPECT_EQ(1, ss.position());
        EXPECT_TRUE(ss.seek(0, goblib::seekdir::end));
        EXPECT_EQ(26, ss.position());
        EXPECT_FALSE(ss.seek(-10000, goblib::seekdir::end));
        EXPECT_EQ(26, ss.position());
        EXPECT_TRUE(ss.seek(-8, goblib::seekdir::end));
        EXPECT_EQ(26-8, ss.position());
    
        EXPECT_TRUE(ss.seek(10, goblib::seekdir::beg));
        EXPECT_EQ(10, ss.position());
    
        auto d16 = ss.read16();
        if(goblib::endian::little)  { EXPECT_EQ(0x6C6B, d16); }
        else if(goblib::endian::big){ EXPECT_EQ(0x6B6C, d16); }
        EXPECT_EQ(12, ss.position());

        auto d32 = ss.read32();
        if(goblib::endian::little)  { EXPECT_EQ(0x706F6E6D, d32); }
        else if(goblib::endian::big){ EXPECT_EQ(0x6D6E6F70, d32); }
        EXPECT_EQ(16, ss.position());
    }

    //
    {
        std::uint32_t buffer32[8] = {0,1,2,3,4,5,6,7};
        goblib::MemoryStream s32(buffer32, goblib::size(buffer32));
        s32.seek(0, goblib::seekdir::end);
        EXPECT_EQ(sizeof(buffer32), s32.position());
          
        s32.seek(0, goblib::seekdir::beg);
        auto d32 = s32.read32();
        EXPECT_EQ(0, d32);
        d32 = s32.read32();
        EXPECT_EQ(1, d32);

        auto d16 = s32.read16();
        if(goblib::endian::little)  { EXPECT_EQ(2, d16); }
        else if(goblib::endian::big){ EXPECT_EQ(0, d16); }
        d16 = s32.read16();
        if(goblib::endian::little)  { EXPECT_EQ(0, d16); }
        else if(goblib::endian::big){ EXPECT_EQ(2, d16); }
    }
    
    //
    {
        std::array<std::int8_t, 10> c_array = { 0, 11,22,33,44,55,66,77,88,99 };
        std::int8_t c_array_copy[10] = {};
        goblib::MemoryStream sarray(c_array);
        sarray.seek(0, goblib::seekdir::end);
        EXPECT_EQ(sizeof(c_array), sarray.position());

        sarray.seek(0, goblib::seekdir::beg);
        sarray.read(c_array_copy, sizeof(c_array_copy));
        EXPECT_TRUE(std::memcmp(c_array.data(), c_array_copy, sizeof(c_array_copy)) == 0);
    }

    //
    {
        std::vector<std::int16_t> c_vector(16, 42);
        goblib::MemoryStream svector(c_vector);
        std::int16_t c_vector_copy[16] = {};
        svector.seek(0, goblib::seekdir::end);
        EXPECT_EQ(sizeof(decltype(c_vector)::value_type) * c_vector.size(), svector.position());
          
        svector.seek(0, goblib::seekdir::beg);
        svector.read(c_vector_copy, sizeof(c_vector_copy));
        EXPECT_TRUE(std::memcmp(c_vector.data(), c_vector_copy, sizeof(c_vector_copy)) == 0);
    }
    

    {
        int dummy;
        goblib::MemoryStream zero(&dummy, 0);
    
        EXPECT_TRUE(zero.is_open());
        EXPECT_EQ(0, zero.position());

        zero.seek(1, goblib::seekdir::beg);
        EXPECT_EQ(0, zero.position());
        zero.seek(-1, goblib::seekdir::beg);
        EXPECT_EQ(0, zero.position());

        zero.seek(1, goblib::seekdir::cur);
        EXPECT_EQ(0, zero.position());
        zero.seek(-1, goblib::seekdir::cur);
        EXPECT_EQ(0, zero.position());
          
        zero.seek(1, goblib::seekdir::end);
        EXPECT_EQ(0, zero.position());
        zero.seek(-1, goblib::seekdir::end);
        EXPECT_EQ(0, zero.position());
    }


    {
        struct A { std::uint32_t dw[8]; std::uint16_t w[16]; std::uint8_t b[32]; };
        A a = {{1,}, {2,}, {3,} };
        goblib::MemoryStream file(&a, sizeof(A));
    
        A aout;
        EXPECT_EQ(file.read(&aout, sizeof(aout)), sizeof(aout));
        EXPECT_TRUE(std::memcmp(&aout, &a, sizeof(a)) == 0);
    }

    
    // Compoile error
#if 0
    {
        int aaa;
        goblib::MemoryStream err0(aaa,1); // not pointer!
    }
#endif

}

#if defined(ARDUINO)

GOBLIB_MAYBE_UNUSED static void displayCardInformation()
{
    SdFat& sd = goblib::m5s::SD::instance().sd();
    csd_t csd;
    
    printf("CardType:");
    switch(sd.card()->type())
    {
    case SD_CARD_TYPE_SD1: printf("SD1\n"); break;
    case SD_CARD_TYPE_SD2: printf("SD2\n"); break;
    case SD_CARD_TYPE_SDHC:printf("%s\n", sdCardCapacity(&csd) < 70000000 ? "SDHC" : "SDXC"); break;
    default: printf("Unknown\n"); break;
    }
    printf("FAT:%u\n", sd.fatType());

    auto sec = sd.card()->sectorCount();
    auto fsec = sd.vol()->freeClusterCount() * sd.vol()->sectorsPerCluster();
    float cardSize = sec * 0.000512f;
    float freeSize = fsec * 0.000512f;
    float used = cardSize - freeSize;
    printf("CardSize: %fMB used: %fMB\n", cardSize, used);
    /*
      std::uint32_t cs2 = sec * 8 / 15625;
      std::uint32_t fs2 = fsec * 8 / 15625;
      std::uint32_t used2 = cs2 - fs2;
      printf("%uMB : %uMB\n", cs2, used2);
    */
}

GOBLIB_MAYBE_UNUSED static void ls(goblib::m5s::File& dir, std::uint32_t depth, bool recursive = false, bool show_hidden = false)
{
    goblib::m5s::File file;
    while(file.openNext(&dir, O_RDONLY))
    {
        char fname[128];
        file.getName(fname, sizeof(fname));
        std::uint64_t size = file.size();
        std::uint16_t date;
        std::uint16_t time;
        file.getModifyDateTime(&date, &time);
        char buf[sizeof("YYYY-MM-DD hh::mm") ] = {0,};
        char* str = buf + sizeof(buf) - 1;
        if(date) {
            str = fsFmtTime(str, time);
            *--str = ' ';
            str = fsFmtDate(str, date);
        }
        else
        {
            do { *--str = ' '; }while(str > buf);
        }
        bool d = file.isDir();
        if(show_hidden || fname[0] != '.')
        {
            printf("%*c %10llu %s %s%s\n", depth*4, ' ', size, str, fname, d ? "/" : "");
        }

        if(d && recursive && (show_hidden || fname[0] != '.'))
        {
            ls(file, depth + 1, recursive, show_hidden);
        }
        file.close();
    }

}

GOBLIB_MAYBE_UNUSED static void ls(const char* path, bool recursive = false, bool show_hidden = false)
{
    SdFat& sd = goblib::m5s::SD::instance().sd();
    goblib::m5s::File dir, file;

    if(!dir.open(path)) { sd.initErrorHalt(); }
    ls(dir, 0, recursive, show_hidden);
}


static bool prepareFile(const char* path)
{
    goblib::m5s::File f;
    if(f.open(path, O_CREAT | O_TRUNC | O_WRONLY))
    {
        f.write("abcdefghijklmnopqrstuvwxyz");
        f.close();
        return true;
    }
    return false;
}

TEST(Stream, M5S)
{
    //displayCardInformation();
    //ls("/res/td", true, false);

    const char* path = "/unittest.000";
    EXPECT_TRUE(prepareFile(path));
    
    {
        goblib::m5s::FileStream ss(path);
        EXPECT_TRUE(ss.is_open());

        EXPECT_TRUE(ss.seek(0, goblib::seekdir::end));
        EXPECT_EQ(26, ss.position());

        EXPECT_TRUE(ss.seek(0, goblib::seekdir::beg));
        EXPECT_EQ(0, ss.position());
        EXPECT_FALSE(ss.seek(-123, goblib::seekdir::beg));
        EXPECT_EQ(0, ss.position());
    
        auto d8 = ss.read8();
        EXPECT_EQ(d8, 'a');
        EXPECT_EQ(1, ss.position());

        EXPECT_TRUE(ss.seek(5, goblib::seekdir::cur));
        d8 = ss.read8();
        EXPECT_EQ(d8, 'g');
        EXPECT_EQ(7, ss.position());

        EXPECT_TRUE(ss.seek(-7, goblib::seekdir::cur));
        EXPECT_EQ(0, ss.position());
        EXPECT_FALSE(ss.seek(-7, goblib::seekdir::cur));
        EXPECT_EQ(0, ss.position());

        d8 = ss.read8();
        EXPECT_EQ(d8, 'a');
        EXPECT_EQ(1, ss.position());

        EXPECT_FALSE(ss.seek(1000, goblib::seekdir::cur));
        EXPECT_EQ(1, ss.position());
        EXPECT_TRUE(ss.seek(0, goblib::seekdir::end));
        EXPECT_EQ(26, ss.position());
        EXPECT_FALSE(ss.seek(-10000, goblib::seekdir::end));
        EXPECT_EQ(26, ss.position());
        EXPECT_TRUE(ss.seek(-8, goblib::seekdir::end));
        EXPECT_EQ(26-8, ss.position());
    
        EXPECT_TRUE(ss.seek(10, goblib::seekdir::beg));
        EXPECT_EQ(10, ss.position());
    
        auto d16 = ss.read16();
        if(goblib::endian::little)  { EXPECT_EQ(0x6C6B, d16); }
        else if(goblib::endian::big){ EXPECT_EQ(0x6B6C, d16); }
        EXPECT_EQ(12, ss.position());

        auto d32 = ss.read32();
        if(goblib::endian::little)  { EXPECT_EQ(0x706F6E6D, d32); }
        else if(goblib::endian::big){ EXPECT_EQ(0x6D6E6F70, d32); }
        EXPECT_EQ(16, ss.position());
    }
}


TEST(Stream, Pcm)
{
    const char path[] = "/res/td/wav/IC.wav";
    {
        goblib::MemoryStream mem(path, sizeof(path));
        goblib::PcmStream pcm;
        EXPECT_FALSE(pcm.assign(&mem));
    }

    {
        goblib::m5s::FileStream file(path);
        goblib::PcmStream pcm;
        EXPECT_TRUE(pcm.assign(&file));
    }

}

#endif

