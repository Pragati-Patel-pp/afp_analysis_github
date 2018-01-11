#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>
#include <AfpAnalysis/AfpAnalysisExample.h>

#include <xAODRootAccess/Init.h>
#include <xAODRootAccess/TEvent.h>
#include <AsgTools/MessageCheck.h> // ASG status code check
#include <xAODEventInfo/EventInfo.h>
#include <xAODForward/AFPDataContainer.h>
#include <xAODForward/AFPData.h>
#include <xAODForward/AFPSiHitContainer.h>
#include <xAODForward/AFPSiHit.h>

#include <utility> 
#include<iostream>

ClassImp(AfpAnalysisExample)


AfpAnalysisExample :: AfpAnalysisExample ()
{
}



EL::StatusCode AfpAnalysisExample :: setupJob (EL::Job& job)
{
  job.useXAOD ();
  ANA_CHECK_SET_TYPE (EL::StatusCode); 
  ANA_CHECK(xAOD::Init());

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode AfpAnalysisExample :: histInitialize ()
{
    return EL::StatusCode::SUCCESS;
}



EL::StatusCode AfpAnalysisExample :: fileExecute ()
{
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode AfpAnalysisExample :: changeInput (bool firstFile)
{
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode AfpAnalysisExample :: initialize ()
{
  ASG_SET_ANA_TOOL_TYPE( m_AfpTool, AfpTools::AfpTool);
  m_AfpTool.setName("AfpTool");

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode AfpAnalysisExample :: execute ()
{
  ANA_CHECK_SET_TYPE (EL::StatusCode); // set type of return code you are expecting (add to top of each function once)

  xAOD::TEvent* event = wk()->xaodEvent();

  const xAOD::EventInfo* eventInfo = 0;
  ANA_CHECK(event->retrieve( eventInfo, "EventInfo") );
  std::cout << "lb: " << eventInfo->lumiBlock() << "\n";

  m_AfpTool->reco(); // Do AFP reconstruction

  int nh[4][4] = {0};
  int nhs[4] = {0};

  for(auto hit : *(m_AfpTool->getHits())){
    int s = hit->stationID();
    int l = hit->pixelLayerID();
    nh[s][l]++;
    nhs[s]++;
  }

  for(int s=0; s<4; s++)
  for(int l=0; l<4; l++)
  std::cout << "hits: " 
    << " " << s 
    << " " << l
    << " " << nh[s][l]
    << "\n";

  int nc[4][4] = {0};
  int ncs[4] = {0};
  for(auto c : *(m_AfpTool->getClusters())){
    int s = c->stationID;
    int l = c->layerID;
    nc[s][l]++;
    ncs[s]++;
  }

  int nt[4] = {0};

  for(auto track : *(m_AfpTool->getTracks())){
    int s = track->stationID;
    nt[s]++;
  }

  m_AfpTool->clean();

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode AfpAnalysisExample :: postExecute ()
{
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode AfpAnalysisExample :: finalize ()
{


  return EL::StatusCode::SUCCESS;
}



EL::StatusCode AfpAnalysisExample :: histFinalize ()
{
  return EL::StatusCode::SUCCESS;
}

// vim: expandtab tabstop=8 shiftwidth=2 softtabstop=2
