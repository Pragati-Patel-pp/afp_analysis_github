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
  // config.setProperty("afpTool.numberOfAdditionalBc", 1).ignore(); // for run 348002



  // --- create and configure cluster reconstruction tool for each station ---
  config.createPrivateTool ("afpTool.clusterToolSt0", "AFP::ClusteringTool").ignore();
  config.createPrivateTool ("afpTool.clusterToolSt1", "AFP::ClusteringTool").ignore();
  config.createPrivateTool ("afpTool.clusterToolSt2", "AFP::ClusteringTool").ignore();
  config.createPrivateTool ("afpTool.clusterToolSt3", "AFP::ClusteringTool").ignore();



  // --- create and configure track finding tool for each station ---
  config.createPrivateTool ("afpTool.trackFindToolSt0", "AFP::TrackFindSimpleClusteringTool").ignore();
  config.createPrivateTool ("afpTool.trackFindToolSt1", "AFP::TrackFindSimpleClusteringTool").ignore();
  config.createPrivateTool ("afpTool.trackFindToolSt2", "AFP::TrackFindSimpleClusteringTool").ignore();
  config.createPrivateTool ("afpTool.trackFindToolSt3", "AFP::TrackFindSimpleClusteringTool").ignore();
  config.setProperty("afpTool.trackFindToolSt0.allowedDistanceBetweenClustersInTrack", 1.0).ignore(); // set distance between tracks
  config.setProperty("afpTool.trackFindToolSt1.allowedDistanceBetweenClustersInTrack", 1.0).ignore(); // set distance between tracks
  config.setProperty("afpTool.trackFindToolSt2.allowedDistanceBetweenClustersInTrack", 0.8).ignore(); // set distance between tracks
  config.setProperty("afpTool.trackFindToolSt3.allowedDistanceBetweenClustersInTrack", 0.8).ignore(); // set distance between tracks



  // --- create and configure track reconstruction tool for each station ---
  config.createPrivateTool ("afpTool.trackRecoToolSt0", "AFP::TrackRecoLinRegTool").ignore();
  config.createPrivateTool ("afpTool.trackRecoToolSt1", "AFP::TrackRecoLinRegTool").ignore();
  config.createPrivateTool ("afpTool.trackRecoToolSt2", "AFP::TrackRecoLinRegTool").ignore();
  config.createPrivateTool ("afpTool.trackRecoToolSt3", "AFP::TrackRecoLinRegTool").ignore();



  // --- create and configure proton reconstruction tool for each side ---
  /// ProtonRecoAnalytical:
  config.createPrivateTool ("afpTool.protonRecoToolSide0", "AFP::ProtonRecoAnalytical").ignore();
  config.createPrivateTool ("afpTool.protonRecoToolSide1", "AFP::ProtonRecoAnalytical").ignore();

  // Side A
  config.afpTool.setProperty("protonRecoToolSide0.parametrizationFileName", "parameterization_beta_0.40_b2_2017.txt").ignore();

  // Side C
  config.afpTool.setProperty("protonRecoToolSide1.parametrizationFileName", "parameterization_beta_0.40_b1_2017.txt").ignore();


  // /// ProtonRecoChi2Minuit:
  // config.createPrivateTool ("afpTool.protonRecoToolSide0", "AFP::ProtonRecoChi2Minuit").ignore();
  // config.createPrivateTool ("afpTool.protonRecoToolSide1", "AFP::ProtonRecoChi2Minuit").ignore();
  //
  // // Side A
  // config.createPrivateTool ("afpTool.protonRecoToolSide0.transportToolAFP", "AFP::TransportParametrization").ignore();
  // config.setProperty("afpTool.protonRecoToolSide0.transportToolAFP.parametrizationFileNameBeam1", "parameterization_beta_0.40_b1_2017.txt").ignore();
  // config.setProperty("afpTool.protonRecoToolSide0.transportToolAFP.parametrizationFileNameBeam2", "parameterization_beta_0.40_b2_2017.txt").ignore();
  //
  // // Side C
  // config.createPrivateTool ("afpTool.protonRecoToolSide1.transportToolAFP", "AFP::TransportParametrization").ignore();
  // config.setProperty("afpTool.protonRecoToolSide1.transportToolAFP.parametrizationFileNameBeam1", "parameterization_beta_0.40_b1_2017.txt").ignore();
  // config.setProperty("afpTool.protonRecoToolSide1.transportToolAFP.parametrizationFileNameBeam2", "parameterization_beta_0.40_b2_2017.txt").ignore();


  // /// ProtonRecoWithALFA:
  // config.createPrivateTool ("afpTool.protonRecoToolSide0", "AFP::ProtonRecoWithALFA").ignore();
  // config.createPrivateTool ("afpTool.protonRecoToolSide1", "AFP::ProtonRecoWithALFA").ignore();
  //
  // // Side A
  // config.createPrivateTool ("afpTool.protonRecoToolSide0.transportToolAFP", "AFP::TransportParametrization").ignore();
  // config.setProperty("afpTool.protonRecoToolSide0.transportToolAFP.parametrizationFileNameBeam1", "param_run348002_AFP_b1.txt").ignore();
  // config.setProperty("afpTool.protonRecoToolSide0.transportToolAFP.parametrizationFileNameBeam2", "param_run348002_AFP_b2.txt").ignore();
  //
  // config.createPrivateTool ("afpTool.protonRecoToolSide0.transportToolALFA", "AFP::TransportParametrization").ignore();
  // config.setProperty("afpTool.protonRecoToolSide0.transportToolALFA.parametrizationFileNameBeam1", "param_run348002_ALFA_b1.txt").ignore();
  // config.setProperty("afpTool.protonRecoToolSide0.transportToolALFA.parametrizationFileNameBeam2", "param_run348002_ALFA_b2.txt").ignore();
  // config.setProperty("afpTool.protonRecoToolSide0.transportToolALFA.nearStationPosition", 237.398).ignore();
  // config.setProperty("afpTool.protonRecoToolSide0.transportToolALFA.farStationPosition", 245.657).ignore();
  //
  // // Side C
  // config.createPrivateTool ("afpTool.protonRecoToolSide1.transportToolAFP", "AFP::TransportParametrization").ignore();
  // config.setProperty("afpTool.protonRecoToolSide1.transportToolAFP.parametrizationFileNameBeam1", "param_run348002_AFP_b1.txt").ignore();
  // config.setProperty("afpTool.protonRecoToolSide1.transportToolAFP.parametrizationFileNameBeam2", "param_run348002_AFP_b2.txt").ignore();
  //
  // config.createPrivateTool ("afpTool.protonRecoToolSide1.transportToolALFA", "AFP::TransportParametrization").ignore();
  // config.setProperty("afpTool.protonRecoToolSide1.transportToolALFA.parametrizationFileNameBeam1", "param_run348002_ALFA_b1.txt").ignore();
  // config.setProperty("afpTool.protonRecoToolSide1.transportToolALFA.parametrizationFileNameBeam2", "param_run348002_ALFA_b2.txt").ignore();
  // config.setProperty("afpTool.protonRecoToolSide1.transportToolALFA.nearStationPosition", 237.398).ignore();
  // config.setProperty("afpTool.protonRecoToolSide1.transportToolALFA.farStationPosition", 245.656).ignore();


  // // &&&&& END OPTIONAL CONFIGURATION &&&&&

  job.algsAdd(config);


  EL::DirectDriver driver;

  driver.submit( job, submitDir );
  return 0;
}
