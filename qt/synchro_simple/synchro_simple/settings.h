#ifndef SETTINGS_H
#define SETTINGS_H

#define CONF_SRC_DIR "src_dir"
#define CONF_DST_DIR "dst_dir"
#define CONF_MAX_OP "max_op"
#define CONF_OPTIONS_GEOMETRY "geometry"

#define CONF_DEF_MAX_OP 2000

#ifdef __WIN32__
#include "windows.h"
#define CONF_PRIORITY "priority"
#define CONF_DEF_PRIORITY BELOW_NORMAL_PRIORITY_CLASS
#endif

#endif // SETTINGS_H
