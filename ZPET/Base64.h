//
//  Base64.h
//  ZPET
//
//  Created by James Duffy on 20/11/2020.
//  Copyright © 2020 James Duffy. All rights reserved.
//

#ifndef _MACARON_BASE64_H_
#define _MACARON_BASE64_H_

/**
 * The MIT License (MIT)
 * Copyright (c) 2016 tomykaira
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <string>

std::string Encode(const std::string data) {
  static constexpr char sEncodingTable[] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
    'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
    'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
    'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
    'w', 'x', 'y', 'z', '0', '1', '2', '3',
    '4', '5', '6', '7', '8', '9', '+', '/'
  };

  size_t in_len = data.size();
  size_t out_len = 4 * ((in_len + 2) / 3);
  std::string ret(out_len, '\0');
  size_t i;
  char *p = const_cast<char*>(ret.c_str());

  for (i = 0; i < in_len - 2; i += 3) {
    *p++ = sEncodingTable[(data[i] >> 2) & 0x3F];
    *p++ = sEncodingTable[((data[i] & 0x3) << 4) | ((int) (data[i + 1] & 0xF0) >> 4)];
    *p++ = sEncodingTable[((data[i + 1] & 0xF) << 2) | ((int) (data[i + 2] & 0xC0) >> 6)];
    *p++ = sEncodingTable[data[i + 2] & 0x3F];
  }
  if (i < in_len) {
    *p++ = sEncodingTable[(data[i] >> 2) & 0x3F];
    if (i == (in_len - 1)) {
      *p++ = sEncodingTable[((data[i] & 0x3) << 4)];
      *p++ = '=';
    }
    else {
      *p++ = sEncodingTable[((data[i] & 0x3) << 4) | ((int) (data[i + 1] & 0xF0) >> 4)];
      *p++ = sEncodingTable[((data[i + 1] & 0xF) << 2)];
    }
    *p++ = '=';
  }

  return ret;
}

#endif /* _MACARON_BASE64_H_ */
