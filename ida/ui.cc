#include "third_party/zynamics/bindiff/ida/ui.h"

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#endif

#include <cmath>
#include <limits>

#include "third_party/absl/base/macros.h"
#include "third_party/zynamics/bindiff/utility.h"

uint32_t GetMatchColor(double value) {
  // Google Material colors: Deep Orange (500) -> Google Yellow (A700) ->
  // Light Green (A400)
  // Generated with
  // http://gka.github.io/palettes/#colors=#FF5722,#FF9E00,#84FA02|steps=256|bez=1|coL=1
  // All of these literals are written as 0xRRGGBB, which mean that on little
  // endian machines, they need to be reversed to be used. This is done for
  // copy-and-paste convenience.
  constexpr const uint32_t kColorRamp[] = {
      0xff5722, 0xff5722, 0xff5922, 0xff5922, 0xff5a22, 0xff5b21, 0xff5b21,
      0xff5c21, 0xff5d21, 0xff5e21, 0xff5f21, 0xff5f21, 0xff5f21, 0xff6120,
      0xff6120, 0xff6220, 0xff6220, 0xff6320, 0xff6420, 0xff6520, 0xff661f,
      0xff671f, 0xff661f, 0xff671f, 0xff681f, 0xff691f, 0xff691f, 0xff6a1e,
      0xff6b1e, 0xff6c1e, 0xfe6c1e, 0xfe6d1e, 0xfe6f1e, 0xfe6f1e, 0xfe701d,
      0xfe711d, 0xfe701d, 0xfe721d, 0xfe721d, 0xfe731d, 0xfe731d, 0xfe741d,
      0xfd751c, 0xfe751c, 0xfd761c, 0xfd771c, 0xfd771c, 0xfd791b, 0xfd791b,
      0xfd7a1b, 0xfd7a1b, 0xfd7a1b, 0xfc7c1b, 0xfc7d1b, 0xfc7d1a, 0xfc7e1a,
      0xfc7f1a, 0xfc7f1a, 0xfc7f1a, 0xfb811a, 0xfb8119, 0xfb8119, 0xfb8319,
      0xfb8219, 0xfb8419, 0xfb8419, 0xfa8518, 0xfa8618, 0xfa8718, 0xfa8718,
      0xfa8818, 0xfa8718, 0xf98a17, 0xf98917, 0xf98b17, 0xf98a17, 0xf98c17,
      0xf88d17, 0xf88c17, 0xf88d16, 0xf88e16, 0xf78f16, 0xf79016, 0xf79015,
      0xf79115, 0xf79215, 0xf69215, 0xf69215, 0xf69315, 0xf69415, 0xf59514,
      0xf59514, 0xf59614, 0xf49714, 0xf49714, 0xf49813, 0xf49813, 0xf49913,
      0xf39a13, 0xf39a13, 0xf39b13, 0xf29c12, 0xf29d12, 0xf29d12, 0xf19e12,
      0xf19e11, 0xf19f11, 0xf0a011, 0xf1a011, 0xf0a011, 0xf0a111, 0xefa210,
      0xefa210, 0xefa410, 0xeea410, 0xeea510, 0xeea50f, 0xeda60f, 0xeda70f,
      0xeda80f, 0xeca90e, 0xeca80e, 0xeca90e, 0xeba90e, 0xebaa0e, 0xebab0e,
      0xeaac0d, 0xeaac0d, 0xe9ad0d, 0xe9ad0d, 0xe8ae0c, 0xe8af0c, 0xe8af0c,
      0xe7b00b, 0xe7b10b, 0xe6b20b, 0xe6b20b, 0xe6b20b, 0xe5b30a, 0xe5b30a,
      0xe4b50a, 0xe4b50a, 0xe3b609, 0xe3b709, 0xe3b709, 0xe2b709, 0xe1b809,
      0xe1b908, 0xe1ba08, 0xe1b908, 0xdfbb08, 0xdfbb08, 0xdebc07, 0xdebc07,
      0xdebe07, 0xdebd07, 0xddbe07, 0xddbe07, 0xdbc006, 0xdbc006, 0xdac206,
      0xdac106, 0xdac206, 0xd9c205, 0xd8c405, 0xd8c405, 0xd7c405, 0xd7c504,
      0xd7c504, 0xd6c604, 0xd5c804, 0xd5c704, 0xd4c904, 0xd3c903, 0xd3ca03,
      0xd2cb03, 0xd2ca03, 0xd1cc03, 0xd0cd03, 0xd0cc03, 0xd0cc03, 0xcfcd02,
      0xcece02, 0xcdcf02, 0xcbd002, 0xcbd102, 0xcbd002, 0xcad202, 0xcad102,
      0xc9d301, 0xc8d401, 0xc7d401, 0xc7d501, 0xc5d601, 0xc5d601, 0xc4d701,
      0xc3d800, 0xc3d700, 0xc2d800, 0xc2d800, 0xc0da00, 0xbfda00, 0xbfda00,
      0xbedc00, 0xbedb00, 0xbcdd00, 0xbbde00, 0xbbdd00, 0xbade00, 0xbade00,
      0xb8e000, 0xb7e100, 0xb7e100, 0xb5e100, 0xb5e100, 0xb4e300, 0xb4e200,
      0xb2e400, 0xb1e400, 0xb1e400, 0xafe600, 0xaee600, 0xade600, 0xace800,
      0xace700, 0xaae900, 0xaae800, 0xa8e900, 0xa6eb00, 0xa6ea00, 0xa4eb00,
      0xa4ec00, 0xa3ed00, 0xa1ee00, 0xa1ee00, 0xa1ed00, 0x9fee00, 0x9def00,
      0x9def00, 0x9bf000, 0x98f200, 0x98f200, 0x96f300, 0x96f300, 0x94f301,
      0x94f401, 0x92f401, 0x8ff601, 0x8ff601, 0x8df601, 0x8cf701, 0x8bf701,
      0x88f802, 0x88f902, 0x85fa02, 0x84fa02};
  // Mark manual matches in blue.
  constexpr uint32_t kManualMatchColor = 0x01579B;  // Light Blue (900)

  uint32_t reverse_color = value != kManualMatch
                             ? kColorRamp[static_cast<int>(
                                   value * (ABSL_ARRAYSIZE(kColorRamp) - 1))]
                             : kManualMatchColor;
  return (reverse_color << 16) | (reverse_color & 0xff00) |
         (reverse_color >> 16);
}

void CopyToClipboard(const string& data) {
#ifdef WIN32
  if (!OpenClipboard(0)) {
    throw std::runtime_error(GetLastOsError());
  }
  struct ClipboardCloser {
    ~ClipboardCloser() { CloseClipboard(); }
  } deleter;

  if (!EmptyClipboard()) {
    throw std::runtime_error(GetLastOsError());
  }

  HGLOBAL buffer_handle =
      GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, data.size());
  if (!buffer_handle) {
    throw std::runtime_error(GetLastOsError());
  }

  bool fail = true;
  char* buffer = static_cast<char*>(GlobalLock(buffer_handle));
  if (buffer) {
    memcpy(buffer, data.c_str(), data.size());
    if (GlobalUnlock(buffer) &&
        SetClipboardData(CF_TEXT, buffer_handle /* Transfer ownership */)) {
      fail = false;
    }
  }
  if (fail) {
    // Only free on failure, as SetClipboardData() takes ownership.
    GlobalFree(buffer_handle);
    throw std::runtime_error(GetLastOsError());
  }
#else
  // TODO(cblichmann): Implement copy to clipboard for Linux/macOS.
  //                   Linux: Pipe to "xsel -b"
  //                   macOS: Pipe to "pbcopy"
#endif
}
