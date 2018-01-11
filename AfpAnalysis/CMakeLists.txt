atlas_subdir (AfpAnalysis)

atlas_add_root_dictionary (AfpAnalysisLib
  AfpAnalysisDictSource
  ROOT_HEADERS AfpAnalysis/*.h src/LinkDef.h
  EXTERNAL_PACKAGES ROOT )

atlas_add_library (AfpAnalysisLib
  AfpAnalysis/*.h AfpAnalysis/*.ihh src/*.cxx ${AfpAnalysisDictSource}
  PUBLIC_HEADERS AfpAnalysis
  INCLUDE_DIRS
  LINK_LIBRARIES EventLoop xAODRootAccess xAODForward AsgTools xAODEventInfo AfpAnalysisToolboxLib
  PRIVATE_LINK_LIBRARIES )

atlas_add_executable (runAfpAnalysisExample util/runAfpAnalysisExample.cxx
  LINK_LIBRARIES AfpAnalysisLib)

