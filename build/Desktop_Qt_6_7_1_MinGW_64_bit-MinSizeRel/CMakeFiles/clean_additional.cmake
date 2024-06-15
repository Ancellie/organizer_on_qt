# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "MinSizeRel")
  file(REMOVE_RECURSE
  "CMakeFiles\\organizer_on_qt_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\organizer_on_qt_autogen.dir\\ParseCache.txt"
  "QCustomPlot-library\\lib\\CMakeFiles\\qcustomplot_autogen.dir\\AutogenUsed.txt"
  "QCustomPlot-library\\lib\\CMakeFiles\\qcustomplot_autogen.dir\\ParseCache.txt"
  "QCustomPlot-library\\lib\\qcustomplot_autogen"
  "organizer_on_qt_autogen"
  )
endif()
