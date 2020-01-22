#include <iostream>
#include <sstream>
#include <string>


/*
Basic API Design:

Base64 b64 = Base64 {};

"hello world" >> b64; Push bytes onto b64. Encode.
std::cout << b64; Push encoded string onto cout.

 */

class base64 {
  std::string encoded_text;
  std::string plaintext;

  void encode();

  
public:
  std::string str(); // return the ciphertext
  base64(std::string& plaintext);
  ~base64() = default;

};


