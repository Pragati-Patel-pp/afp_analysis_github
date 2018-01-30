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
  job.options()->setDouble (EL::Job::optMaxEvents, 500);


  EL::AnaAlgorithmConfig config;
  config.setType ("AfpAnalysisExample");
  config.setName ("AfpAnalysisExampleAlg");


  job.algsAdd(config);

  EL::DirectDriver driver;
  driver.submit( job, submitDir );
  return 0;
}
