cmake_minimum_required( VERSION 3.7 FATAL_ERROR )

project(AfpAnalysisToolbox_example VERSION 1.0.0)

set( _parentProjectNames Athena AthenaP1 AnalysisBase AthAnalysis
   AthSimulation AthDerivation AnalysisTop )
set( _defaultParentProject AnalysisBase )
foreach( _pp ${_parentProjectNames} )
   if( NOT "$ENV{${_pp}_DIR}" STREQUAL "" )
      set( _defaultParentProject ${_pp} )
      break()
   endif()
endforeach()

set( ATLAS_PROJECT ${_defaultParentProject}
   CACHE STRING "The name of the parent project to build against" )

unset( _parentProjectNames )
unset( _defaultParentProject )

find_package( ${ATLAS_PROJECT} REQUIRED )

atlas_ctest_setup()

atlas_project( UserAnalysis 1.0.0
   USE ${ATLAS_PROJECT} ${${ATLAS_PROJECT}_VERSION} )

lcg_generate_env( SH_FILE ${CMAKE_BINARY_DIR}/${ATLAS_PLATFORM}/env_setup.sh )
install( FILES ${CMAKE_BINARY_DIR}/${ATLAS_PLATFORM}/env_setup.sh
   DESTINATION . )

atlas_cpack_setup()

