#include "xAODRootAccess/Init.h"
#include "SampleHandler/SampleHandler.h"
#include "SampleHandler/ScanDir.h"
#include "SampleHandler/ToolsDiscovery.h"
#include "EventLoop/Job.h"
#include "EventLoop/DirectDriver.h"
#include "AnaAlgorithm/AnaAlgorithmConfig.h"
#include "SampleHandler/DiskListLocal.h"
#include <TSystem.h>

#include "AfpAnalysis/AfpAnalysisExample.h"

#include <iostream>

#include "AfpAnalysisTools/AfpAnalysisTool.h"

int main( int argc, char* argv[] ) {

  // Take the submit directory from the input if provided:
  std::string submitDir = "submitDir";
  if( argc > 1 ) submitDir = argv[ 1 ];

  xAOD::Init().ignore();

  SH::SampleHandler sh;

  const char* inputFilePath = gSystem->ExpandPathName ("/eos/atlas/atlascerngroupdisk/det-afp/AnalysisData/Tutorial/data17_13TeV.00331975.physics_Main.merge.AOD.r10203_p3399/");

  SH::ScanDir().filePattern("*").scan(sh,inputFilePath);

  sh.setMetaString( "nc_tree", "CollectionTree" );

  sh.print();

  EL::Job job;
  job.sampleHandler( sh );
  job.options()->setDouble (EL::Job::optMaxEvents, 10);


  // --- create configuration object for analysis algorithm ---
  EL::AnaAlgorithmConfig config;
  config.setType ("AfpAnalysisExample");
  config.setName ("AfpAnalysisExampleAlg");

  // ===== OPTIONAL CONFIGURATION =====
  // --- create main analysis tool ---
  config.createPrivateTool ("afpTool","AFP::AfpAnalysisTool").ignore();
  config.setProperty("afpTool.hitsContainerName", "AFPSiHitContainer").ignore();
  config.setProperty("afpTool.clusterContainerName", "AFPSiHitsClusterContainer").ignore();
  config.setProperty("afpTool.tracksContainerName", "AFPTrackContainer").ignore();
  config.setProperty("afpTool.protonsContainerName", "AFPProtonContainer").ignore();



  // Side A
  // config.setProperty("afpTool.protonRecoToolSide0.parametrizationFileName", "parameterization_beta_0.40_b2_2017.txt").ignore();

  // Side C
  // config.setProperty("afpTool.protonRecoToolSide1.parametrizationFileName", "parameterization_beta_0.40_b1_2017.txt").ignore();

  // &&&&& END OPTIONAL CONFIGURATION &&&&&


  job.algsAdd(config);

  EL::DirectDriver driver;
  driver.submit( job, submitDir );

  return 0;
}
