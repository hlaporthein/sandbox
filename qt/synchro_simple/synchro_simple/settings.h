#ifndef SETTINGS_H
#define SETTINGS_H

#define CONF_SRC_DIR "src_dir"
#define CONF_DST_DIR "dst_dir"
#define CONF_MAX_OP "max_op"
#define CONF_OPTIONS_GEOMETRY "geometry"
#define CONF_USE_PERIOD "use_period"
#define CONF_PERIOD "period"
#define CONF_FILTERS "filters"
#define CONF_FILTERS_LABEL "label"
#define CONF_FILTERS_VALUE "value"
#define CONF_FILTERS_IS_DIR "is_dir"

#define CONF_DEF_MAX_OP 2000
#define CONF_DEF_USE_PERIOD false
#define CONF_DEF_PERIOD 120
#define CONF_DEF_FILTERS_IS_DIR false

#ifdef __WIN32__
#include "windows.h"
#define CONF_PRIORITY "priority"
#define CONF_DEF_PRIORITY BELOW_NORMAL_PRIORITY_CLASS
#endif

#endif // SETTINGS_H
