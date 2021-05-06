
#include <AfpAnalysis/AfpAnalysisExample.h>

#include <xAODRootAccess/Init.h>
#include <xAODRootAccess/TEvent.h>
#include <xAODEventInfo/EventInfo.h>
#include <xAODForward/AFPSiHitContainer.h>
#include <xAODForward/AFPSiHit.h>

#include <AfpAnalysisTools/AfpAnalysisTool.h>
#include <PATInterfaces/SystematicsUtil.h>

#include <utility> 
#include <iostream>


AfpAnalysisExample :: AfpAnalysisExample (const std::string& name, ISvcLocator* pSvcLocator) : 
  EL::AnaAlgorithm (name, pSvcLocator),
  m_afpTool("AFP::AfpAnalysisTool/afpTool", this)
{
  m_afpTool.declarePropertyFor(this, "afpTool", "Tool providing AFP information");
  m_afpTool.setTypeAndName ("AFP::AfpAnalysisTool/afpTool");
}



StatusCode AfpAnalysisExample :: initialize ()
{
  ANA_CHECK (m_afpTool.retrieve ());

  ANA_MSG_INFO(m_afpTool->configInfo());

  // Systematics for AFP (optional)
  /* {
    CP::SystematicSet recommendedSystematics;

    // Add recommended systematics for AFP
    recommendedSystematics.insert(m_afpTool->recommendedSystematics());

    m_systematicsList = CP::make_systematics_vector(recommendedSystematics);

    std::cout << "List of avaliable systematics:\n";
    for (auto& systematic : m_systematicsList)
      std::cout << systematic.name() << '\n';
  } */

#ifdef XAOD_STANDALONE
  ANA_MSG_DEBUG(" this is AfpAnalysisExample initialize with XAOD_STANDALONE (i.e. with AnalysisBase)");
#else
  ANA_MSG_DEBUG(" this is AfpAnalysisExample initialize without XAOD_STANDALONE (i.e. with AthAnalysis)");
#endif

#ifdef XAOD_ANALYSIS
  ANA_MSG_DEBUG(" this is AfpAnalysisExample initialize with XAOD_ANALYSIS");
#else
  ANA_MSG_DEBUG(" this is AfpAnalysisExample initialize without XAOD_ANALYSIS");
#endif

  return StatusCode::SUCCESS;
}



StatusCode AfpAnalysisExample :: execute ()
{
  ANA_CHECK_SET_TYPE (StatusCode); // set type of return code you are expecting (add to top of each function once)
  const xAOD::EventInfo* eventInfo = nullptr;

#ifdef XAOD_STANDALONE
  xAOD::TEvent* event = wk()->xaodEvent();
#else
  auto event = evtStore();
#endif
  
  ANA_CHECK(event->retrieve( eventInfo, "EventInfo") );
  ANA_MSG_INFO("");
  ANA_MSG_INFO("");
  ANA_MSG_INFO("New event: " << eventInfo->eventNumber());
  ANA_MSG_INFO("run: " << eventInfo->runNumber() << " lumi block: " << eventInfo->lumiBlock());

  // Systematics for AFP (optional)
  /* {
    // Define systematic by name
    std::string affectingSystematics = "CLUST_NEIGHBOUR";

    // Find systematic in systematics list
    auto systematic = std::find_if(m_systematicsList.begin(), m_systematicsList.end(),
        [&affectingSystematics](auto& sys) { return sys.name() == affectingSystematics; });

    // Apply systematic to afpTool
    if (systematic != m_systematicsList.end()) {
      m_afpTool->applySystematicVariation(*systematic);

      ANA_MSG_INFO("Systematic: " << (*systematic).name());
    }
  } */

  // MC does not contain hits nor clusters, we omit them
  if (!eventInfo->eventType(xAOD::EventInfo::IS_SIMULATION)) {
    // Hits
    ANA_MSG_INFO("===== HITS =====");
    ANA_MSG_INFO("Event contains " << m_afpTool->hits()->size() << " AFP Si hits");

    int nh[4][4] = {0};
    int nhs[4] = {0};

    for(auto hit : *(m_afpTool->hits())){
      int s = hit->stationID();
      int l = hit->pixelLayerID();
      nh[s][l]++;
      nhs[s]++;
    }

    for(int station=0; station<4; station++) {
      ANA_MSG_INFO("Event contains " << nhs[station] << " AFP Si hits in station " << station);
      for(int layer=0; layer<4; layer++)
        ANA_MSG_INFO("Event contains " << nh[station][layer] << " AFP Si hits in station " << station << " layer " << layer);
    }

    // Clusters
    ANA_MSG_INFO("===== CLUSTERS =====");
    ANA_MSG_INFO("Event contains " << m_afpTool->clusters()->size() << " AFP Si clusters");
    
    int nc[4][4] = {0};
    int ncs[4] = {0};
    for(auto c : *(m_afpTool->clusters())){
      ATH_MSG_INFO("\t" << c->xLocal()<< ", "<<c->yLocal()<<", "<<c->zLocal());
      int s = c->stationID();
      int l = c->pixelLayerID();
      nc[s][l]++;
      ncs[s]++;
    }

    for(int station=0; station<4; station++) {
      ANA_MSG_INFO("Event contains " << ncs[station] << " AFP Si clusters in station " << station);
      for(int layer=0; layer<4; layer++)
        ANA_MSG_INFO("Event contains " << nc[station][layer] << " AFP Si clusters in station " << station << " layer " << layer);
    }
  }

  // Tracks
  ANA_MSG_INFO("===== TRACKS =====");
  ANA_MSG_INFO("Event contains " << m_afpTool->tracks()->size() << " AFP tracks");

  int nt[4] = {0};
  for(auto trk : *(m_afpTool->tracks())){
    int s = trk->stationID();
    ATH_MSG_INFO("\t" << trk->stationID()<<" "<<trk->xLocal()<<" "<<trk->yLocal());
    nt[s]++;
  }

  for(int station=0; station<4; station++)
    ANA_MSG_INFO("Event contains " << nt[station] << " AFP tracks in station " << station);

  
  ANA_MSG_INFO("===== STORE GATE TRACKS =====");

  const xAOD::AFPTrackContainer* afpTracks = nullptr;
  ANA_CHECK(event->retrieve( afpTracks, "AFPTrackContainer") );
  for(auto t : *afpTracks)
    ATH_MSG_INFO("\t" << t->stationID()<<" "<<t->xLocal()<<" "<<t->yLocal());

  
  // Protons
  ANA_MSG_INFO("===== PROTONS =====");
  ANA_MSG_INFO("Event contains " << m_afpTool->protons()->size() << " AFP protons");

  for(auto proton : *(m_afpTool->protons())) {

    const int nTracks = proton->nTracks();

    ATH_MSG_INFO("\t" << proton->px() << ", " << proton->py() << ", " << proton->pz() << ", " << proton->e() << ", chi2: " << proton->chi2() << ", nTracks: " << nTracks);

    for (int it = 0; it < nTracks; it++) {
      auto track = proton->track(it);
      ATH_MSG_INFO("\t\t" << track->stationID()<<" "<<track->xLocal()<<" "<<track->yLocal());
    }
  }

  // Systematics for AFP (optional)
  /* {
    // Reset afpTool to default state
    m_afpTool->reset();
  } */

  return StatusCode::SUCCESS;
}



StatusCode AfpAnalysisExample :: finalize ()
{


  return StatusCode::SUCCESS;
}



// vim: expandtab tabstop=8 shiftwidth=2 softtabstop=2
