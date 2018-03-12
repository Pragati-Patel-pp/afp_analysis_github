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

  const char* inputFilePath = gSystem->ExpandPathName ("/eos/user/r/rafal/AFP/data17_13TeV.00331975.physics_Main.merge.AOD.r10203_p3399/");

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

  // // ===== OPTIONAL CONFIGURATION =====
  // // --- create main analysis tool ---
  // config.createPrivateTool ("afpTool","AFP::AfpAnalysisTool").ignore();
  // config.setProperty("afpTool.hitsContainerName", "AFPSiHitContainer").ignore();
  // config.setProperty("afpTool.clusterContainerName", "AFPSiHitsClusterContainer").ignore();
  // config.setProperty("afpTool.tracksContainerName", "AFPTrackContainer").ignore();
  // config.setProperty("afpTool.protonsContainerName", "my_proton_name").ignore();

  // // --- create and configure proton reconstruction tool ---
  // config.createPrivateTool ("afpTool.clusterTool","AFP::ClusteringTool").ignore();

  // // --- create and configure track finding tool ---
  // config.createPrivateTool ("afpTool.trackFindTool","AFP::TrackFindSimpleClusteringTool").ignore();
  // config.setProperty("afpTool.trackFindTool.allowedDistanceBetweenClustersInTrack", 42).ignore(); // set distance between tracks

  // // --- create and configure track reconstruction tool ---
  // config.createPrivateTool ("afpTool.trackRecoTool","AFP::TrackRecoLinRegTool").ignore();

  // // --- create and configure proton reconstruction tool ---
  // config.createPrivateTool ("afpTool.protonRecoTool","AFP::ProtonRecoChi2Minuit").ignore();
  // // config.createPrivateTool ("afpTool.protonRecoTool","AFP::ProtonRecoAnalytical").ignore();
  // // config.setProperty("afpTool.protonRecoTool.parametrizationFileNameBeam1", "parameterization_beta_0.40_b1_2017.txt").ignore();
  // // config.setProperty("afpTool.protonRecoTool.parametrizationFileNameBeam2", "parameterization_beta_0.40_b2_2017.txt").ignore();

  // // &&&&& END OPTIONAL CONFIGURATION &&&&&

  job.algsAdd(config);


  EL::DirectDriver driver;

  driver.submit( job, submitDir );
  return 0;
}
