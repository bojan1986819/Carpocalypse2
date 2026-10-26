# Embed a text resource as a C byte array.
#   cmake -DSOURCE_DIR=<dir> -DFILE=<name.ext> -P EmbedResource.cmake
# writes <name.ext>.h into the current binary dir with  static const char NAME_EXT[] = { ... };
# (NAME_EXT = file name upper-cased, '.' -> '_'), as used by the glrend shader_*.c files (sizeof gives the length).
file(READ "${SOURCE_DIR}/${FILE}" _hex HEX)
string(REGEX REPLACE "([0-9a-f][0-9a-f])" "0x\\1," _bytes "${_hex}")
string(TOUPPER "${FILE}" _name)
string(REPLACE "." "_" _name "${_name}")
file(WRITE "${FILE}.h" "/* generated from ${FILE} */\nstatic const char ${_name}[] = {${_bytes}};\n")
