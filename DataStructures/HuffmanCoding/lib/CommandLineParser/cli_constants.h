#ifndef CLI_CONSTANTS_H
#define CLI_CONSTANTS_H

#include <string>

namespace CLI {
  const std::string USAGE_MESSAGE =
    "Usage: huffman (-c[ompress] | -d[ecompress]) -i <input_file> -o <output_file>";
  
  const std::string COMPRESS_SHORT   = "-c";
  const std::string COMPRESS_LONG    = "-compress";
  const std::string DECOMPRESS_SHORT = "-d";
  const std::string DECOMPRESS_LONG  = "-decompress";
  const std::string INPUT_SHORT      = "-i";
  const std::string INPUT_LONG       = "-input";
  const std::string OUTPUT_SHORT     = "-o";
  const std::string OUTPUT_LONG      = "-output";
};

#endif
