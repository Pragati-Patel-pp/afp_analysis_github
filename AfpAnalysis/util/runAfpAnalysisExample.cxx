#include "xAODRootAccess/Init.h"
#include "SampleHandler/SampleHandler.h"
#include "SampleHandler/ScanDir.h"
#include "SampleHandler/ToolsDiscovery.h"
#include "EventLoop/Job.h"
#include "EventLoop/DirectDriver.h"
#include "SampleHandler/DiskListLocal.h"
#include <TSystem.h>

#include "AfpAnalysis/AfpAnalysisExample.h"

int main( int argc, char* argv[] ) {

  // Take the submit directory from the input if provided:
  std::string submitDir = "submitDir";
  if( argc > 1 ) submitDir = argv[ 1 ];

  xAOD::Init().ignore();

  SH::SampleHandler sh;

  const char* inputFilePath = gSystem->ExpandPathName ("/eos/user/r/rafal/AFP/user.rafal.00310216.r9017.TrigSkim.HLT_noalg_L1AFP_C_AND.v03_skimmed.root/");
  SH::ScanDir().filePattern("*").scan(sh,inputFilePath);

  sh.setMetaString( "nc_tree", "CollectionTree" );

  sh.print();

  EL::Job job;
  job.sampleHandler( sh );
  job.options()->setDouble (EL::Job::optMaxEvents, 1000);

  AfpAnalysisExample *alg = new AfpAnalysisExample;
  alg->msg().setLevel( MSG::DEBUG ); 
  gErrorIgnoreLevel = kWarning;

  job.algsAdd( alg );

  EL::DirectDriver driver;
  driver.submit( job, submitDir );

  return 0;
}
