#include "base64.hpp"


base64::base64(std::string& plaintext) : plaintext {plaintext} {
  this->encode();
}

void base64::encode(){
  char mask0 = 0xFc;
  char mask1 = 0x03;
  char mask2 = 0xF0;
  char mask3 = 0x0F;
  char mask4 = 0xC0;
  char mask5 = 0x3F;
  std::string base64_alpha = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

  /*
To encode in base 64:
1. read in 3 bytes
2. go through the bits as follows
   a. byte0_idx = mask0 & buffer0;
   b. byte1_idx = (mask1 & buffer0 << 2)  + mask2 & buffer1
   c. byte3_idx = mask3 & buffer1 + ((mask4 & buffer2) << 4)
   d. byte4_idx = mask5 & buffer2
3. Use the indices to index into the appropriate character of the base64 alphabet
   */

  // Create an index to access the base64 alphabet
  size_t idx = 0;
  size_t ii = 0;
  for(;ii < plaintext.length(); ++ii){
    char current_char = plaintext[ii];
      switch(ii%3){
    case(0):
      // Read the most significant 6 bits in
      idx = (current_char & mask0) >> 2;
      encoded_text += base64_alpha[idx];
      // Since we have the six most significant bytes, we can overwrite idx
      // with the next 2 bits
      idx = (current_char & mask1) <<  4;
    break;
    case(1):
      // We already have the most significant 2 bits of the index. Get the next 4
      idx += (current_char & mask2) >> 4;
      encoded_text += base64_alpha[idx];
      // Now, overwrite idx with the most significant 4 bits of the next
      idx = (current_char & mask3) << 2;
      break;
    case(2):
      // We have most significant 4 bits. Get the next 2.
      idx += (current_char & mask4) >> 6;
      this->encoded_text += base64_alpha[idx];
      // Now get the next 6 bits
      idx = (current_char & mask5);
      encoded_text += base64_alpha[idx];
      break;
    }
  }
  // Now we need to handle padding
  
  size_t rem = ii % 3;
  if (rem == 0)
    return;
  encoded_text += base64_alpha[idx];
  
  std::string padding = std::string(3 - rem, '=');
  encoded_text += padding;
}

std::string base64::str() {
  return this->encoded_text;
}

std::string hex2ascii(const std::string& str){
  // TODO Make this safe. Right now, it just increments twice to get one byte
  std::string res {};
  for(size_t ii = 0; ii < str.length(); ){
    char new_char = std::stoi(str.substr(ii,2),nullptr,16);
    res += new_char;
    ++(++ii);
  }
  return res;
}

		      

int main(){
  // Cryptopals Example
  std::string inpt = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";
  std::string inptAscii = hex2ascii(inpt);
  std::cout << inptAscii << std::endl;
  base64 b64 {inptAscii};
  std::cout << b64.str() << std::endl;
  // WIKIPEDIA EXAMPLES
  inptAscii = "leasure.";
  base64 b64_pad {inptAscii};
  std::cout << b64_pad.str() << std::endl;
  inptAscii = "easure.";
  base64 b64_pad2 {inptAscii};
  std::cout << b64_pad2.str() << std::endl;
  return 0;
}


