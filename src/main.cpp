#include <iostream>


#include "gtest/gtest.h"

#include "string.h"

#include "parse_engine.h"

#include <vector>

using namespace std;

TEST(TestConstructStr, Cons1)
{
    char* s = "hello?";
    EXPECT_EQ("ell", string(constructStr(s + 1, 3)));
}

TEST(TestRedeemVal, SingleLayerPacket)
{
  //! [6:hello?]
  char* s = "hello?";
  char* stream = (char*) malloc( sizeof(uint32_t) + strlen(s) );
  char* pos = stream;

  auto header = strlen(s);
  memcpy(pos, (char*)&header, sizeof(uint32_t));
  pos += sizeof(uint32_t);

  strcpy(pos, s);
  pos += strlen(s);

  auto end = scopeEnd<uint32_t>(stream);
  EXPECT_EQ("hello?", string(redeemStr<uint32_t>(stream, end)));
}

TEST(TestRedeemVal, OverRedeemVariable)
{
  //! [6:hello?]
  int s = 65535;
  char* stream = (char*) malloc( sizeof(uint32_t) + sizeof(s) );
  char* pos = stream;

  auto header = sizeof(s);
  memcpy(pos, (char*)&header, sizeof(uint32_t));
  pos += sizeof(uint32_t);

  memcpy(pos, (char*)&s, sizeof(s));
  pos += sizeof(s);

  auto begin = scopeBegin<uint32_t>(stream);
  auto end = scopeEnd<uint32_t>(stream);
  EXPECT_EQ(65535, redeemVal<int>(begin, end));
}

TEST(TestRedeemVal, MultiLayerPacket)
{
  //! [10:[6:hello?]]
  char* s = "hello?";
  char* stream = (char*) malloc( sizeof(uint32_t) + sizeof(uint32_t) + strlen(s) );
  char* pos = stream;

  uint32_t header;

  header =
      sizeof(uint32_t) + strlen(s) // first cell
      ;
  memcpy(pos, (char*)&header, sizeof(uint32_t));
  pos += sizeof(uint32_t);

  header = strlen(s);
  memcpy(pos,  (char*)&header, sizeof(uint32_t));
  pos += sizeof(uint32_t);

  strcpy(pos, s);
  pos += strlen(s);

  auto begin = scopeBegin<uint32_t>(stream);
  auto end = scopeEnd<uint32_t>(stream);
  EXPECT_EQ(string("hello?"), string(redeemStr<uint32_t>(begin, end)));
}

TEST(TestRedeemVal, MultiLayerMultiCellPacket)
{
  //! [20:[6:hello,][6:world!]]
    std::vector<char> stream(sizeof(uint32_t));
    size_t pos{};

    Packet pk {{std::string{"hello,"}, std::string{"world!"}, std::string{"duckhacks"}, uint32_t{2019}}, 100};

    *(uint32_t*)&stream[0] = (4 + 6) + (4 + 6) + (4 + 9) + 4; // set main header
    pos += sizeof(uint32_t); // skip main header

    insertStr(stream, pos, pk.data[0].get<std::string>());
    insertStr(stream, pos, pk.data[1].get<std::string>());
    insertStr(stream, pos, pk.data[2].get<std::string>());
    insertVal(stream, pos, pk.data[3].get<uint32_t>());

    auto p = scopeBegin<uint32_t>(stream.data());
    auto end = scopeEnd<uint32_t>(stream.data());
    EXPECT_EQ(string("hello,"), string(redeemStr<uint32_t>(p, end)));
    EXPECT_EQ(string("world!"), string(redeemStr<uint32_t>(p, end)));
    EXPECT_EQ(string("duckhacks"), string(redeemStr<uint32_t>(p, end)));
    EXPECT_EQ(uint32_t{2019}, uint32_t(redeemVal<uint32_t>(p, end)));
}

TEST(TestRedeemVal, MultiLayerOverRedeemString)
{
  //! [19:[6:hello?][5:world]]
  char* s1 = "hello?";
  char* s2 = "world";
  char* stream = (char*) malloc(sizeof(uint32_t) // main header
                                + sizeof(uint32_t) + strlen(s1) // cell1 total
                                + sizeof(uint32_t) + strlen(s2) // cell2 total
                                );
  char* pos = stream;

  uint32_t header;

  header = // main header
      sizeof(uint32_t) + strlen(s1) // cell1 total
      + sizeof(uint32_t) + strlen(s2) // cell2 total
      ;
  memcpy(pos, (char*)&header, sizeof(uint32_t));
  pos += sizeof(header);

  // cell1 header
  header = strlen(s1);
  memcpy(pos, (char*)&header, sizeof(uint32_t));
  pos += sizeof(header);

  // cell1 data
  strcpy(pos, s1);
  pos += strlen(s1);

  // cell2 header
  header = strlen(s2);
  memcpy(pos, (char*)&header, sizeof(uint32_t));
  pos += sizeof(header);

  // cell1 data
  strcpy(pos, s2);
  pos += strlen(s2);


  auto p = scopeBegin<uint32_t>(stream);
  auto end = scopeEnd<uint32_t>(stream);
  for(std::string s : {"hello?", "world", "", "", ""})
    EXPECT_EQ(s, string(redeemStr<uint32_t>(p, end)));

}

TEST(TestPlayground, Playground)
{
  //! JSON example
//  json field1Json = json::array({"time", "uuid", "content", "pubkey"});
//  json packetJson = json::array({0, field1Json});
}


int main(int argc, char **argv  )
{
//        variant_t s = "hello"s;
//        std::string ss = s.get<std::string>();

  ::testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}
