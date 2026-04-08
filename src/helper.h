#ifndef helper_h
#define helper_h
std::string utf8_to_iso_8859_1(const std::string &utf8_str) {
  std::string latin1_str;
  for (size_t i = 0; i < utf8_str.size(); ++i) {
    uint8_t ch = static_cast<uint8_t>(utf8_str[i]);

    // If the character is 1-byte ASCII, just copy it
    if (ch < 0x80) {
      latin1_str.push_back(static_cast<char>(ch));
    }
    // If it's a 2-byte UTF-8 sequence (starting with 0xC0 - 0xDF)
    else if ((ch & 0xE0) == 0xC0) {
      uint8_t second_byte = static_cast<uint8_t>(utf8_str[++i]);
      // Combine the two UTF-8 bytes to form the Latin-1 character
      uint8_t latin1_char = ((ch & 0x1F) << 6) | (second_byte & 0x3F);
      latin1_str.push_back(static_cast<char>(latin1_char));
    }
    // For safety: skip any invalid characters (not part of 2-byte UTF-8 range)
    else {
      // You can choose to handle errors here
      continue;
    }
  }
  return latin1_str;
}

#endif
