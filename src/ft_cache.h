#ifndef FTCACHE_INCLUDED
#define FTCACHE_INCLUDED
#include <vector>
#include <string>
#include <set>
#include <map>
#include <memory>
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_TYPES_H
#include FT_CACHE_H
#include "utils.h"


typedef std::pair<std::string, unsigned int> FaceID;

struct FontInfo {
  std::string family;
  std::string style;
  bool is_italic;
  bool is_bold;
  bool is_monospace;
  bool is_vertical;
  bool has_kerning;
  bool has_color;
  bool is_scalable;
  int n_glyphs;
  int n_sizes;
  int n_charmaps;
  std::vector<long> bbox;
  long max_ascend;
  long max_descend;
  long max_advance_h;
  long max_advance_w;
  long lineheight;
  long underline_pos;
  long underline_size;
};

struct GlyphInfo {
  unsigned index;
  long x_bearing;
  long y_bearing;
  long width;
  long height;
  long x_advance;
  long y_advance;
  std::vector<long> bbox;
};

class FreetypeCache {
public:
  FreetypeCache();
  ~FreetypeCache();

  bool load_font(const char* file, int index, double size, double res);
  FontInfo font_info();
  bool has_glyph(u_int32_t index);
  bool load_glyph(u_int32_t index);
  GlyphInfo glyph_info();
  GlyphInfo cached_glyph_info(u_int32_t index, int& error);
  double string_width(u_int32_t* string, int length, bool add_kern);
  long cur_lineheight();
  long cur_ascender();
  long cur_descender();
  bool apply_kerning(u_int32_t left, u_int32_t right, long &x, long &y);
  int error_code;
  
private:
  FT_Library library;
  FTC_Manager manager;
  FTC_CMapCache charmaps;
  std::map<u_int32_t, GlyphInfo> glyphstore;
  
  FaceID cur_id;
  double cur_size;
  double cur_res;
  bool cur_can_kern;
  unsigned int cur_glyph;
  bool cur_has_size;
  
  FT_Face face;
  FT_Size size;
  FTC_ScalerRec scaler;
  
  std::set<FaceID> id_lookup;
  std::vector< std::unique_ptr<FaceID> > id_pool;
  
  inline bool current_face(FaceID id, double size, double res) {
    return size == cur_size && res == cur_res && id == cur_id;
  };
};

#endif
