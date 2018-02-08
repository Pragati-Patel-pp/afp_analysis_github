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


AfpAnalysisExample :: AfpAnalysisExample (const std::string& name, ISvcLocator* pSvcLocator)
      : EL::AnaAlgorithm (name, pSvcLocator)
{
}



StatusCode AfpAnalysisExample :: initialize ()
{
  ASG_SET_ANA_TOOL_TYPE( m_AfpTool, AFP::AfpTool);
  m_AfpTool.setName("AfpTool");

  return StatusCode::SUCCESS;
}



StatusCode AfpAnalysisExample :: execute ()
{
  ANA_CHECK_SET_TYPE (StatusCode); // set type of return code you are expecting (add to top of each function once)

  xAOD::TEvent* event = wk()->xaodEvent();

  const xAOD::EventInfo* eventInfo = 0;
  ANA_CHECK(event->retrieve( eventInfo, "EventInfo") );
  ANA_MSG_INFO("");
  ANA_MSG_INFO("");
  ANA_MSG_INFO("New event: " << eventInfo->eventNumber());
  ANA_MSG_INFO("run: " << eventInfo->runNumber() << " lumi block: " << eventInfo->lumiBlock());

  ANA_CHECK(m_AfpTool->reco()); // Do AFP reconstruction

  // Hits
  ANA_MSG_INFO("===== HITS =====");
  ANA_MSG_INFO("Event contains " << m_AfpTool->hits()->size() << " AFP Si hits");

  int nh[4][4] = {0};
  int nhs[4] = {0};

  for(auto hit : *(m_AfpTool->hits())){
    int s = hit->stationID();
    int l = hit->pixelLayerID();
    nh[s][l]++;
    nhs[s]++;
  }

  for(int s=0; s<4; s++){
    ANA_MSG_INFO("Event contains " << nhs[s] << " AFP Si hits in station " << s);
    for(int l=0; l<4; l++)
      ANA_MSG_INFO("Event contains " << nh[s][l] << " AFP Si hits in station " << s << " layer " << l);
  }

  // Clusters
  ANA_MSG_INFO("===== CLUSTERS =====");
  ANA_MSG_INFO("Event contains " << m_AfpTool->clusters()->size() << " AFP Si clusters");
  
  int nc[4][4] = {0};
  int ncs[4] = {0};
  for(auto c : *(m_AfpTool->clusters())){
    ATH_MSG_INFO("\t" << c->xLocal()<< ", "<<c->yLocal()<<", "<<c->zLocal());
    int s = c->stationID();
    int l = c->pixelLayerID();
    nc[s][l]++;
    ncs[s]++;
  }

  for(int s=0; s<4; s++){
    ANA_MSG_INFO("Event contains " << ncs[s] << " AFP Si clusters in station " << s);
    for(int l=0; l<4; l++)
      ANA_MSG_INFO("Event contains " << nc[s][l] << " AFP Si clusters in station " << s << " layer " << l);
  }

  // Tracks
  ANA_MSG_INFO("===== TRACKS =====");
  ANA_MSG_INFO("Event contains " << m_AfpTool->tracks()->size() << " AFP tracks");
  
  int nt[4] = {0};
  for(auto t : *(m_AfpTool->tracks())){
    int s = t->stationID();
    ATH_MSG_INFO("\t" << t->stationID()<<" "<<t->xLocal()<<" "<<t->yLocal());
    nt[s]++;
  }

  for(int s=0; s<4; s++)
    ANA_MSG_INFO("Event contains " << nt[s] << " AFP tracks in station " << s);

  
  ANA_MSG_INFO("===== STORE GATE TRACKS =====");

  const xAOD::AFPTrackContainer* afpTracks = 0;
  ANA_CHECK(event->retrieve( afpTracks, "AFPTrackContainer") );
  for(auto t : *afpTracks)
    ATH_MSG_INFO("\t" << t->stationID()<<" "<<t->xLocal()<<" "<<t->yLocal());

  
  // Protons
  ANA_MSG_INFO("===== PROTONS =====");
  ANA_MSG_INFO("Event contains " << m_AfpTool->protons()->size() << " AFP protons");

  for(auto proton : *(m_AfpTool->protons())){
    ATH_MSG_INFO("\t" << proton->px()<<", "<<proton->py()<<", "<<proton->pz()<<", "<<proton->e());
  }

  return StatusCode::SUCCESS;
}



StatusCode AfpAnalysisExample :: finalize ()
{


  return StatusCode::SUCCESS;
}



// vim: expandtab tabstop=8 shiftwidth=2 softtabstop=2
