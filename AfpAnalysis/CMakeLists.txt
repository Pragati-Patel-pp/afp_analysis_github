atlas_subdir (AfpAnalysis)

atlas_depends_on_subdirs(PUBLIC
			 AfpAnalysisTools)

atlas_add_library (AfpAnalysisLib
  AfpAnalysis/*.h AfpAnalysis/*.ihh src/*.cxx ${AfpAnalysisDictSource}
  PUBLIC_HEADERS AfpAnalysis
  INCLUDE_DIRS
  LINK_LIBRARIES EventLoop xAODRootAccess xAODForward AsgTools xAODEventInfo AfpAnalysisToolsLib
  PRIVATE_LINK_LIBRARIES )

atlas_add_executable (runAfpAnalysisExample util/runAfpAnalysisExample.cxx 
  LINK_LIBRARIES AfpAnalysisLib)

# Add the dictionary:
atlas_add_dictionary (AfpAnalysisDict
  AfpAnalysis/AfpAnalysisDict.h
  AfpAnalysis/selection.xml
  LINK_LIBRARIES AfpAnalysisLib)

if (NOT XAOD_STANDALONE)
  # Add a component library for AthAnalysis only:
  atlas_add_component (AfpAnalysis
    src/components/*.cxx
    LINK_LIBRARIES AfpAnalysisLib)
endif ()

#atlas_install_joboptions( share/*.py )