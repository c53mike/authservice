#include "src/common/session/gcm_encryptor.h"

#include "gtest/gtest.h"

namespace transparent_auth {
namespace common {
namespace session {

namespace {
// Test vectors obtained from NIST:
// https://csrc.nist.gov/projects/cryptographic-algorithm-validation-program/cavp-testing-block-cipher-modes
std::vector<unsigned char> key = {
    0x26, 0x8e, 0xd1, 0xb5, 0xd7, 0xc9, 0xc7, 0x30, 0x4f, 0x9c, 0xae,
    0x5f, 0xc4, 0x37, 0xb4, 0xcd, 0x3a, 0xeb, 0xe2, 0xec, 0x65, 0xf0,
    0xd8, 0x5c, 0x39, 0x18, 0xd3, 0xd3, 0xb5, 0xbb, 0xa8, 0x9b,
};
std::vector<unsigned char> iv = {0x9e, 0xd9, 0xd8, 0x18, 0x05, 0x64,
                                 0xe0, 0xe9, 0x45, 0xf5, 0xe5, 0xd4};
std::vector<unsigned char> pt = {
    0xfe, 0x29, 0xa4, 0x0d, 0x8e, 0xbf, 0x57, 0x26, 0x2b, 0xdb, 0x87,
    0x19, 0x1d, 0x01, 0x84, 0x3f, 0x4c, 0xa4, 0xb2, 0xde, 0x97, 0xd8,
    0x82, 0x73, 0x15, 0x4a, 0x0b, 0x7d, 0x9e, 0x2f, 0xdb, 0x80};
std::vector<unsigned char> aad = {};
std::vector<unsigned char> ct = {
    0x79, 0x1a, 0x4a, 0x02, 0x6f, 0x16, 0xf3, 0xa5, 0xea, 0x06, 0x27,
    0x4b, 0xf0, 0x2b, 0xaa, 0xb4, 0x69, 0x86, 0x0a, 0xbd, 0xe5, 0xe6,
    0x45, 0xf3, 0xdd, 0x47, 0x3a, 0x5a, 0xcd, 0xde, 0xec, 0xfc};
std::vector<unsigned char> tag = {0x05, 0xb2, 0xb7, 0x4d, 0xb0, 0x66,
                                  0x25, 0x50, 0x43, 0x5e, 0xf1, 0x90,
                                  0x0e, 0x13, 0x6b, 0x15};
}  // namespace

TEST(GcmEncryptorTest, SealAndOpen) {
  auto encryptor = GcmEncryptor::Create(key);

  // Expected output is 'nonce || ciphertext || tag'
  std::vector<unsigned char> expected;
  expected.insert(expected.end(), iv.begin(), iv.end());
  expected.insert(expected.end(), ct.begin(), ct.end());
  expected.insert(expected.end(), tag.begin(), tag.end());

  auto sealed = encryptor->Seal(pt, iv, aad);
  EXPECT_EQ(sealed, expected);

  auto opened = encryptor->Open(sealed, aad);
  ASSERT_TRUE(opened);
  EXPECT_EQ(*opened, pt);
};

}  // namespace session
}  // namespace common
}  // namespace transparent_auth
