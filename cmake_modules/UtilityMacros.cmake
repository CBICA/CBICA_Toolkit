# macro to find all sub-directories
MACRO(SUBDIRLIST result curdir)
  FILE(GLOB children
    RELATIVE ${curdir} ${curdir}/*
    PATTERN "svn" EXCLUDE
  )
  SET(dirlist "")
  FOREACH(child ${children})
    IF(IS_DIRECTORY ${curdir}/${child})
      LIST(APPEND dirlist ${child})
    ENDIF()
  ENDFOREACH()
  SET(${result} ${dirlist})
ENDMACRO()
