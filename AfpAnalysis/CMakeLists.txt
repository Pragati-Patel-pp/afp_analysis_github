atlas_subdir (AfpAnalysis)

# atlas_depends_on_subdirs(PUBLIC
#			 AfpAnalysisTools)

if( NOT XAOD_STANDALONE )
  set( extra_libs AthAnalysisBaseCompsLib AthenaBaseComps AthenaKernel SGTools StoreGateLib SGtests AthenaPoolUtilities GaudiKernel )
endif()
if( XAOD_STANDALONE )
  set( extra_libs EventLoop )
endif()

atlas_add_library (AfpAnalysisLib
  AfpAnalysis/*.h AfpAnalysis/*.ihh src/*.cxx ${AfpAnalysisDictSource}
  PUBLIC_HEADERS AfpAnalysis
  INCLUDE_DIRS
  LINK_LIBRARIES xAODJet xAODCore xAODCaloEvent TrigDecisionToolLib TrigConfInterfaces JetCalibToolsLib JetJvtEfficiencyLib JetSelectorToolsLib AssociationUtilsLib AsgAnalysisInterfaces
  xAODTriggerCnvLib  xAODRootAccess xAODForward AsgTools xAODEventInfo AfpAnalysisToolsLib AnaAlgorithmLib ${extra_libs}
  PRIVATE_LINK_LIBRARIES )

if( XAOD_STANDALONE )
  atlas_add_executable (runAfpAnalysisExample util/runAfpAnalysisExample.cxx 
    LINK_LIBRARIES AfpAnalysisLib)
endif()

# Add the dictionary:
atlas_add_dictionary (AfpAnalysisDict
  AfpAnalysis/AfpAnalysisDict.h
  AfpAnalysis/selection.xml
  LINK_LIBRARIES AfpAnalysisLib)

if (NOT XAOD_STANDALONE)
  # Add a component library for AthAnalysis only:
  atlas_add_component (AfpAnalysis
    src/components/*.cxx
    NOCLIDDB
    LINK_LIBRARIES AfpAnalysisLib)
endif ()

atlas_install_scripts( jobOptions/*.py )
atlas_install_python_modules( python/*.py jobOptions/*.py POST_BUILD_CMD ${ATLAS_FLAKE8} )
atlas_install_joboptions( jobOptions/*.py )
